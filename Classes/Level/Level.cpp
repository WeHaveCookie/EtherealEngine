#include "stdafx.h"
#include "Level.h"
#include "Entity/Entity.h"
#include "Manager/Render/RenderMgr.h"
#include "Manager/File/FileMgr.h"
#include "../../External/rapidjson/document.h"
#include "Utils/jsonUtils.h"
#include "Manager/Entity/EntityMgr.h"
#include "Utils/containerUtils.h"
#include "Manager/Action/CommandMgr.h"
#include "Actions/CommandSpawn.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Sound/SoundMgr.h"
#include "Utils/Random.h"
#include "Actions/CommandMove.h"

void Background::paint()
{
	auto rdrWin = RenderMgr::getSingleton()->getMainRenderWindow();
	rdrWin->draw(m_sprite);
}

Level::Level()
{
	m_shakeTime = 0.5f;
	m_timer = 0.0f;
	m_shakeFactor = 10.0f;
	m_maxShakeFactor = 30.0f;
	m_font.loadFromFile("Data/Fonts/wonder.ttf");
	m_sinusTotalDead = 0;
	m_sinusLocalDead = 0;
	m_spikeTotalDead = 0;
	m_spikeLocalDead = 0;
	m_triangleTotalDead = 0;
	m_triangleLocalDead = 0;
	
	m_score = 0;
}


Level::~Level()
{
}

void Level::paint()
{
	for (auto& background : m_backgrounds)
	{
		background->paint();
	}
	for (auto& text : m_texts)
	{
		auto winRdr = GameMgr::getSingleton()->getMainRenderWindow();
		winRdr->draw(text);
	}
}

bool cmpDisplayLevelLTH(Background* a, Background* b)
{
	return a->m_backgroundLevel >= b->m_backgroundLevel;

}

bool Level::load(const char* path)
{
	m_path = path;

	char* json;
	int jsonSize;

	FileMgr::ReadFile(path, (void**)&json, &jsonSize);

	json[jsonSize] = 0;

	rapidjson::Document document;
	document.Parse((char*)json);
	auto error = document.HasParseError();
	auto object = document.IsObject();
	if (error || !object)
	{
		return false;
	}
	assert(!error);
	assert(object);

	auto namePtr = &m_name;
	checkAndAffect(&document, "Name", ValueType::String, (void**)&namePtr, "NoName", sizeof("NoName"));

	auto sizePtr = &m_size;
	checkAndAffect(&document, "Size", ValueType::Vector2, (void**)&sizePtr);
	
	m_view.reset(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), m_size));
	m_shake = false;
	uint32_t nbrPlayer = 0;
	uint32_t* nbrPlayerPtr = &nbrPlayer;
	checkAndAffect(&document, "NbrPlayer", ValueType::Int, (void**)&nbrPlayerPtr);

	if (document.HasMember("DefaultPlayerPath"))
	{
		m_characterPath = document["DefaultPlayerPath"].GetString();
	}
	else
	{
		m_characterPath = "Data/Character/cow.json";
	}

	bool physicsEnable = true;
	bool* physicsEnablePtr = &physicsEnable;
	checkAndAffect(&document, "Physics", ValueType::Bool, (void**)&physicsEnablePtr);
	PhysicMgr::getSingleton()->enable(physicsEnable);

	GameMgr::getSingleton()->setNumberPlayer(0);
	GameMgr::getSingleton()->setNumberPlayer(nbrPlayer);

	if (document.HasMember("Backgrounds"))
	{
		const rapidjson::Value& backgrounds = document["Backgrounds"];

		for (auto& background : backgrounds.GetArray())
		{
			
			assert(background.HasMember("Path") && background.HasMember("BackgroundLevel"));
			Background* back = new Background();
			auto path = background["Path"].GetString();
			auto load = back->m_texture.loadFromFile(path);
			back->m_sprite.setTexture(back->m_texture);
			back->m_backgroundLevel = background["BackgroundLevel"].GetUint();

			if (background.HasMember("Position") && background["Position"].GetArray().Size())
			{
				back->m_sprite.setPosition(sf::Vector2f(background["Position"][0].GetFloat(), background["Position"][1].GetFloat()));
			}
			pushSorted(&m_backgrounds, back, cmpDisplayLevelLTH);

			if (background.HasMember("Commands"))
			{
				const rapidjson::Value& commands = background["Commands"];
				auto cmdMgr = CommandMgr::getSingleton();
				for (auto& command : commands.GetArray())
				{
					assert(command.HasMember("Command"));
					std::string commandName = command["Command"].GetString();
					std::string cmdPath = "Command" + commandName;
					
					sf::Vector2f motion;
					if (command.HasMember("Motion") && command["Motion"].GetArray().Size() == 2)
					{
						motion.x = command["Motion"][0].GetFloat();
						motion.y = command["Motion"][1].GetFloat();
					}

					float timer = 0.0f;

					if (command.HasMember("Timer"))
					{
						timer = command["Timer"].GetFloat();
					}
					int id;
					auto cmd = cmdMgr->getCommand(cmdPath.c_str(), &id);

					if (commandName == "MoveSprite")
					{
						MoveSpriteHandler handler;
						handler.m_motion = motion;
						handler.m_sprite = &back->m_sprite;
						handler.m_timer = timer;
						cmd->init(NULL, (void*)&handler);
					}
					else
					{
						cmd->init(NULL, (void*)&timer);
					}
					cmdMgr->addCommand(cmd);
				}
			}
		}
	}

	if (document.HasMember("Players"))
	{
		const rapidjson::Value& players = document["Players"];
		uint32_t playerID = 0;
		for (auto& player : players.GetArray())
		{	
			assert(player.HasMember("Path"));
			m_characterPath = player["Path"].GetString();
			if (player.HasMember("Position"))
			{
				const rapidjson::Value& clones = player["Position"];

				for (auto& clone : clones.GetArray())
				{
					Entity* ent = EntityMgr::getSingleton()->createEntity(m_characterPath.c_str());
					ent->setPosition(Vector2(clone[0].GetFloat(), clone[1].GetFloat()));
					m_entitys.push_back(ent);
					GameMgr::getSingleton()->setPlayer(playerID++, ent->getUID());
				}
			}
			else
			{
				Entity* ent = EntityMgr::getSingleton()->createEntity(m_characterPath.c_str());
				m_entitys.push_back(ent);
				playerID++;
			}
		}
	}

	if (document.HasMember("Entitys"))
	{
		const rapidjson::Value& entitys = document["Entitys"];

		for (auto& entity : entitys.GetArray())
		{
			assert(entity.HasMember("Path"));
			Entity* ent;
			Command* cmdPtr = nullptr;
			const char* entPath = entity["Path"].GetString();
			
			if (entity.HasMember("OnClick"))
			{
				const rapidjson::Value& OnClick = entity["OnClick"];

				for (auto& cmd : OnClick.GetArray())
				{
					assert(cmd.HasMember("Command"));
					std::string cmdPath = "Command" + (std::string)cmd["Command"].GetString();
					int id;
					cmdPtr = CommandMgr::getSingleton()->getCommand((char*)cmdPath.c_str(), &id);
					if (cmdPath == "CommandLoadLevel")
					{
						std::string levelPath = cmd["Path"].GetString();
						cmdPtr->init(nullptr, (void*)&levelPath);
					}

				}
			}

			

			if (entity.HasMember("Position"))
			{
				const rapidjson::Value& clones = entity["Position"];
				for (auto& clone : clones.GetArray())
				{
					ent = EntityMgr::getSingleton()->createEntity(entPath);
					ent->setPosition(Vector2(clone[0].GetFloat(), clone[1].GetFloat()));
					m_entitys.push_back(ent);
					if (cmdPtr != nullptr)
					{
						ent->setCommandOnClick(cmdPtr);
					}
					
				}
			}
			else
			{
				ent = EntityMgr::getSingleton()->createEntity(entPath);
				m_entitys.push_back(ent);
				if (cmdPtr != nullptr)
				{
					ent->setCommandOnClick(cmdPtr);
				}
			}
			if (entity.HasMember("Type"))
			{
				auto type = entity["Type"].GetString();
				ent->setType(ent->getTypeByName(type));
			}
			
		}
	}

	if (document.HasMember("Commands"))
	{
		const rapidjson::Value& commands = document["Commands"];
		auto cmdMgr = CommandMgr::getSingleton();
		for (auto& command : commands.GetArray())
		{
			assert(command.HasMember("Command"));
			std::string commandeName = command["Command"].GetString();
			if (commandeName == "Spawn")
			{
				assert(command.HasMember("Path"));
				std::string cmdPath = "Command" + commandeName;
				std::string spawnPath = command["Path"].GetString();
				sf::FloatRect region(sf::Vector2f(0.0,0.0),m_size);
				float timer = 0.0;
				if (command.HasMember("SpawnRegion") && command["SpawnRegion"].GetArray().Size() == 4)
				{
					auto left = command["SpawnRegion"][0].GetFloat();
					auto top = command["SpawnRegion"][1].GetFloat();
					auto width = command["SpawnRegion"][2].GetFloat();
					auto height = command["SpawnRegion"][3].GetFloat();
					region = sf::FloatRect(left, top, width, height);
				}

				if (command.HasMember("Timers"))
				{
					const rapidjson::Value& timers = command["Timers"];

					for (auto& timer : timers.GetArray())
					{
						int id;
						auto cmd = cmdMgr->getCommand(cmdPath.c_str(), &id);
						SpawnHandler spawn;
						spawn.path = spawnPath;
						spawn.spawnRegion = region;
						spawn.timer = timer.GetFloat();
						cmd->init(NULL, (void*)&spawn);
						cmdMgr->addCommand(cmd);
					}
				}
			}
		}
	}

	if (document.HasMember("Musics"))
	{
		const rapidjson::Value& musics = document["Musics"];
		auto soundMgr = SoundMgr::getSingleton();
		for (auto& music : musics.GetArray())
		{
			assert(music.HasMember("Path"));
			const rapidjson::Value& musicPaths = music["Path"];
			int musicID = randIntBorned(0, musicPaths.GetArray().Size());
			auto musicPath = musicPaths[musicID].GetString();

			float volume = 100.0;
			uint32_t layer = 0;
			if (music.HasMember("Volume"))
			{
				volume = music["Volume"].GetFloat();
			}

			if (music.HasMember("Layer"))
			{
				layer = music["Layer"].GetUint();
			}

			auto musicUID = soundMgr->addMusic(musicPath, true);
			soundMgr->getMusic(musicUID)->setVolume(volume);
			soundMgr->getMusic(musicUID)->setLayer(layer);
		}
	}

	if (document.HasMember("Texts"))
	{
		const rapidjson::Value& texts = document["Texts"];
		for (auto& text : texts.GetArray())
		{
			assert(text.HasMember("Text") && text.HasMember("Position") && text["Position"].GetArray().Size() == 2);
			std::string textLabel = text["Text"].GetString();
			sf::Vector2f position(text["Position"][0].GetFloat(), text["Position"][1].GetFloat());
			if (text.HasMember("Value"))
			{
				std::string value = text["Value"].GetString();
				if (value == "SinusDead")
				{
					textLabel += std::to_string(m_sinusTotalDead);
				}
				else if (value == "SpikeDead")
				{
					textLabel += std::to_string(m_spikeTotalDead);
				}
				else if (value == "TriangleDead")
				{
					textLabel += std::to_string(m_triangleTotalDead);
				}
				else if (value == "Score")
				{
					textLabel += std::to_string(m_score);
				}
			}
			sf::Text textSF;
			textSF.setFont(m_font);
			textSF.setPosition(position);
			textSF.setString(textLabel);
			textSF.setColor(sf::Color::White);
			textSF.setCharacterSize(24);
			m_texts.push_back(textSF);
		}
	}

	if (document.HasMember("Inputs"))
	{
		const rapidjson::Value& inputs = document["Inputs"];
		for (auto& input : inputs.GetArray())
		{
			assert(input.HasMember("Button") && input.HasMember("Command"));
			auto keyType = input["Button"].GetString();
			auto commandType = "Command" + std::string(input["Command"].GetString());
			std::string commandPath = "";
			if (input.HasMember("Path"))
			{
				commandPath = input["Path"].GetString();
			}
			auto key = InputMgr::getSingleton()->getKeyByName((char*)keyType);
			if (key == NULL)
			{
				continue;
			}
			if (commandType == "Command")
			{
				key->m_command = NULL;
			}
			else
			{
				int id;
				auto cmd = CommandMgr::getSingleton()->getCommand(commandType.c_str(), &id);
				if (commandPath != "")
				{
					cmd->init(NULL, (void**)&commandPath);
				}
				cmd->setExeType(CommandExeType::JustPressed);
				key->m_command = cmd;
			}
		}
	}
	
	FileMgr::CloseFile(json);
	return true;
}

bool Level::reload()
{
	unload();
	return load(m_path.c_str());
}

void Level::unload()
{
	m_backgrounds.clear();
	m_texts.clear();
	for (auto &entity : m_entitys)
	{
		EntityMgr::getSingleton()->deleteEntity(entity->getUID());
	}
	SoundMgr::getSingleton()->unloadContent();
	CommandMgr::getSingleton()->releaseCommands();
	InputMgr::getSingleton()->SetVibrations(0);
	m_entitys.clear();
	m_name = "";
	m_size = sf::Vector2f();
	EntityMgr::getSingleton()->unload();
}

void Level::registerEntity(Entity* ent)
{
	m_entitys.push_back(ent);
}

void Level::processShake(const float dt)
{
	m_timer += dt;
	if (m_timer > m_shakeTime)
	{
		shake(false);
		m_timer = 0.0f;
	}	
 	m_view.reset(sf::FloatRect(rand() % (int)m_shakeFactor, rand() % (int)m_shakeFactor, m_size.x, m_size.y));
}

void Level::shake(bool b) 
{ 
	auto previousState = m_shake;
	m_shake = b; 
	auto inputMgr = InputMgr::getSingleton();
	if (!m_shake)
	{
		if (inputMgr->getActivePads().size() > 0)
		{
			inputMgr->SetVibrations(0, inputMgr->getActivePads()[0]);
		}
		m_view.reset(sf::FloatRect(0.0f, 0.0f, m_size.x, m_size.y));
	}
	else
	{
		if (m_timer > 0.0f || previousState)
		{
			m_shakeFactor *= 1.10f;
			if (m_shakeFactor > m_maxShakeFactor)
			{
				m_shakeFactor = m_maxShakeFactor;
			}
		}
		else
		{
			m_shakeFactor = 10.0f;
		}
		if (inputMgr->getActivePads().size() > 0)
		{
			inputMgr->SetVibrations(80 + ((m_shakeFactor *6.6) / 10.0), inputMgr->getActivePads()[0]);
		}
		m_timer = 0.0f;
	}
}

const bool Level::isLoaded() const
{
	return m_name != "";
}

void Level::killEnemyType(ShootType::Enum type)
{
	switch (type)
	{
	case ShootType::Sinus:
		m_sinusLocalDead++;
		break;
	case ShootType::Spike:
		m_spikeLocalDead++;
		break;
	case ShootType::Triangle:
		m_triangleLocalDead++;
		break;
	case ShootType::None:
	default:
		break;
	}
}

void Level::startExtermination()
{
	m_spikeLocalDead = 0;
	m_triangleLocalDead = 0;
	m_sinusLocalDead = 0;
}

void Level::endExtermination()
{
	m_triangleTotalDead += m_triangleLocalDead;
	m_sinusTotalDead += m_sinusLocalDead;
	m_spikeTotalDead += m_spikeLocalDead;
	m_score += m_sinusLocalDead + m_spikeLocalDead + m_triangleLocalDead;
	m_score *= 1 + (m_sinusLocalDead / 10.0f);
	m_score *= 1 + (m_spikeLocalDead / 10.0f);
	m_score *= 1 + (m_triangleLocalDead / 10.0f);
	m_triangleLocalDead = 0;
	m_sinusLocalDead = 0;
	m_spikeLocalDead = 0;
}

void Level::clearScore()
{
	m_sinusTotalDead = 0;
	m_sinusLocalDead = 0;
	m_spikeTotalDead = 0;
	m_spikeLocalDead = 0;
	m_triangleTotalDead = 0;
	m_triangleLocalDead = 0;
	m_score = 0;
}
