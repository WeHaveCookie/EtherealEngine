#include "stdafx.h"
#include "GameMgr.h"
#include "EtherealDreamManagers.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Sound/SoundMgr.h"
#include "Thread/LoadingThread.h"
#include "Manager/Persistent/PersistentMgr.h"
#include "Manager/Engine/PhysicMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Loading/LoadingMgr.h"

#include "Utils/Random.h"

#define GAME_NAME "EtherealDream"
#define TEST "{\n\
			\"Name\" : \"Player\",\n\
			\"Width\" : 105,\n\
			\"Height\" : 105,\n\
			\"Speed\" : 1.0,\n\
			\"Position\" : [100.0, 500.0],\n\
			\"State\" : \"RIGHT\",\n\
			\"Animation\" : [\n\
		{\n\
			\"State\" : \"RIGHT\",\n\
				\"Frame\" : 6,\n\
				\"Line\" : 1,\n\
				\"Time\" : 0.2\n\
		},\n\
		{\n\
			\"State\" : \"LEFT\",\n\
			\"Frame\" : 6,\n\
			\"Line\" : 2,\n\
			\"Time\" : 0.2\n\
		},\n\
		{\n\
			\"State\" : \"IDLE_RIGHT\",\n\
			\"Frame\" : 6,\n\
			\"Line\" : 3,\n\
			\"Time\" : 0.2\n\
		},\n\
		{\n\
			\"State\" : \"IDLE_LEFT\",\n\
			\"Frame\" : 6,\n\
			\"Line\" : 4,\n\
			\"Time\" : 0.2\n\
		}\n\
			]\n\
	}"
GameMgr* GameMgr::s_singleton = NULL;

GameMgr::GameMgr()
:Manager(ManagerType::Enum::Game)
{
	s_singleton = this;
}

GameMgr::~GameMgr()
{
}

void GameMgr::init()
{
	m_processTime = sf::Time::Zero;
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	std::cout << vm.height << " " << vm.width << std::endl;
	if (vm.height > 1080 || vm.width > 1920)
	{
		vm = sf::VideoMode(1920, 1080);
	}
	m_mainRenderWindow = new sf::RenderWindow(vm, "Ethereal Dream", sf::Style::Default /*| sf::Style::Fullscreen*/);
	setFrameRate(60);
	m_mainRenderWindow->setVerticalSyncEnabled(true);
	srand(time(NULL));

	sf::Joystick::update();

	auto soundMgr = SOUND_MGR;
	strcpy(m_gameName, GAME_NAME);
	//auto test = EntityMgr::getSingleton()->buildEntity("Data/Character/player.json");
// 	while (!EntityMgr::getSingleton()->entityIsLoaded(test))
// 	{
// 
// 	}
// 	auto player = EntityMgr::getSingleton()->getEntity(test);
// 	player->setPosition(sf::Vector2f(300.0f, 300.0f));

// 	EntityMgr::getSingleton()->entityIsLoaded(test);
// 
// 	PhysicMgr::getSingleton()->registerEntity(test);
}

void GameMgr::process(const float dt)
{
	sf::Clock clock;

	auto inputMgr = INPUT_MGR;
	auto entityMgr = ENTITY_MGR;
	auto soundMgr = SOUND_MGR;
	auto loadThread = LOAD_THREAD;
	sf::Vector2f motion = { 0.0f, 0.0f };
	static SaveTask save;
	static std::vector<uint32_t> ids;


	if (inputMgr->keyIsJustPressed(KeyType::kbNum1))
	{
		// Move to loading thread for more perf !!!
		for (int i = 0; i < 1; i++)
		{
			uint32_t ch;
			int num = randIntBorned(1, 5);
			switch (num)
			{
			case 0:
				//ch = entityMgr->buildEntity("Data/Character/player.json");
				break;
			case 1:
				ch = entityMgr->buildEntity("Data/Character/chicken.json");
				break;
			case 2:
				ch = entityMgr->buildEntity("Data/Character/cow.json");
				break;
			case 3:
				ch = entityMgr->buildEntity("Data/Character/pig.json");
				break;
			case 4:
				ch = entityMgr->buildEntity("Data/Character/client.json");
				break;
			case 5:
				ch = entityMgr->buildEntity("Data/Character/entrepreneur.json");
				break;
			default:
				break;
			}

			if (ch != NULL)
			{
				//ch->setPosition(sf::Vector2f(randFloatBorned(0.0f, 1800.0f), randFloatBorned(0.0f, 900.0f)));
				ids.push_back(ch);
			}
		}
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbNum2))
	{
		for (int i = 0; i < 1; i++)
		{
			if (ids.size() > 0)
			{
				entityMgr->removeEntity(ids[ids.size() - 1]);
				ids.pop_back();
			}
		}
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbT))
	{
		save.init(SaveOpenMode::Write);

		save.addWriteTask("Character", TEST, sizeof(TEST));
		save.submit();
		std::cout << "Save : " << save.isFinished() << std::endl;
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbY))
	{
		save.init(SaveOpenMode::Read);

		save.addReadTask("Character");
		save.submit();
		std::cout << "Save : " << save.isFinished() << std::endl;
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbU))
	{
		if (save.isFinished())
		{
			uint8_t* data;
			int sizeRead = -1;
			save.getResult(0, (void**)&data, &sizeRead);

			if (sizeRead > 0)
			{
				std::cout << "Size Read : " << sizeRead << std::endl;
				std::cout << "Content : \n" << data << std::endl;
			}

			PersistentMgr::getSingleton()->releaseData(data);
		}
	}

	auto ent = entityMgr->getEntity(0);
	
	if (inputMgr->keyIsJustPressed(KeyType::kbRight))
	{
		motion.x += 10;
		ent->setState(EntityAnimationState::Right);
	}
	if (inputMgr->keyIsPressed(KeyType::kbRight))
	{
		motion.x += 10;
	}

	if (inputMgr->keyIsJustReleased(KeyType::kbRight))
	{
		ent->setState(EntityAnimationState::IdleRight);
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbLeft))
	{
		motion.x -= 10;
		ent->setState(EntityAnimationState::Left);
	}
	if (inputMgr->keyIsPressed(KeyType::kbLeft))
	{
		motion.x -= 10;
	}

	if (inputMgr->keyIsJustReleased(KeyType::kbLeft))
	{
		ent->setState(EntityAnimationState::IdleLeft);
	}

	if (inputMgr->keyIsPressed(KeyType::kbUp))
	{
		motion.y -= 10;
	}

	if (inputMgr->keyIsPressed(KeyType::kbDown))
	{
		motion.y += 10;
	}
	ent->move(motion);
	
	m_processTime = clock.getElapsedTime();

}

void GameMgr::end()
{
}

void GameMgr::paint()
{
}

void GameMgr::showImGuiWindow(bool* window)
{
	if (ImGui::Begin("GameMgr", window))
	{
		ImGui::Text("Framerate : %f fps", g_Framerate);
		ImGui::Text("Time per frame : %f ms", g_DeltaTime * 1000);

		if (ImGui::CollapsingHeader("Manager"))
		{
			ImGui::Text("EntityMgr : %f ms", EntityMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("PhysicMgr : %f ms", PhysicMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("LevelMgr : %f ms", LevelMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("LoadingMgr : %f ms", LoadingMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("GameMgr : %f ms", getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("SoundMgr : %f ms", SoundMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
		}
		ImGui::End();
	}
}

bool GameMgr::isRunning()
{
	return m_mainRenderWindow->isOpen();
}

void GameMgr::setFrameRate(uint32_t frameRate)
{
	m_mainRenderWindow->setFramerateLimit(frameRate);
	g_DeltaTime = 1.0f / (float)frameRate;
}