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

void Background::paint()
{
	auto rdrWin = RenderMgr::getSingleton()->getMainRenderWindow();
	rdrWin->draw(m_sprite);
}

Level::Level()
{
}


Level::~Level()
{
}

void Level::paint()
{
	for (auto &background : m_backgrounds)
	{
		background->paint();
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
			
			const char* entPath = entity["Path"].GetString();
			if (entity.HasMember("Position"))
			{
				const rapidjson::Value& clones = entity["Position"];
				for (auto& clone : clones.GetArray())
				{
					Entity* ent = EntityMgr::getSingleton()->createEntity(entPath);
					ent->setPosition(Vector2(clone[0].GetFloat(), clone[1].GetFloat()));
					m_entitys.push_back(ent);
				}
			}
			else
			{
				Entity* ent = EntityMgr::getSingleton()->createEntity(entPath);
				m_entitys.push_back(ent);
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
			auto musicPath = music["Path"].GetString();
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
	for (auto &entity : m_entitys)
	{
		EntityMgr::getSingleton()->deleteEntity(entity->getUID());
	}
	m_entitys.clear();
	m_name = "";
	m_size = sf::Vector2f();
}

void Level::registerEntity(Entity* ent)
{
	m_entitys.push_back(ent);
}