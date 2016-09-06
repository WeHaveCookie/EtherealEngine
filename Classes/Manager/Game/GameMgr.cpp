#include "stdafx.h"
#include "GameMgr.h"
#include "EtherealDreamManagers.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Sound/SoundMgr.h"
#include "Thread/LoadThread.h"
#include "Manager/Persistent/PersistentMgr.h"

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
	
}

void GameMgr::process(const float dt)
{
	auto inputMgr = INPUT_MGR;
	auto entityMgr = ENTITY_MGR;
	auto soundMgr = SOUND_MGR;
	auto loadThread = LOAD_THREAD;
	sf::Vector2f motion = { 0.0f, 0.0f };
	static SaveTask save;
	

	

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

	if (inputMgr->keyIsJustPressed(KeyType::kbA))
	{
		soundMgr->addSound("Data/Sound/FX/build.ogg");
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbZ))
	{
		soundMgr->addSound("Data/Sound/FX/mumu.ogg");
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbE))
	{
		soundMgr->addSound("Data/Sound/FX/upgrade.ogg");
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbQ))
	{
		soundMgr->addSound("Data/Sound/FX/workDone.ogg");
	}	
	
	if (inputMgr->keyIsJustPressed(KeyType::kbS))
	{
		soundMgr->addSound("Data/Sound/FX/test.ogg");
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbM))
	{
		soundMgr->addMusic("Data/Sound/Ambiant/theme.ogg");
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbL))
	{
		soundMgr->addMusic("Data/Sound/Ambiant/theme.ogg", true);
	}


	auto ent = entityMgr->getEntity(1);
	
	if (inputMgr->keyIsJustPressed(KeyType::kbRight))
	{
		motion.x++;
		ent->setState(EntityAnimationState::Right);
	}
	if (inputMgr->keyIsPressed(KeyType::kbRight))
	{
		motion.x++;
	}

	if (inputMgr->keyIsJustReleased(KeyType::kbRight))
	{
		ent->setState(EntityAnimationState::IdleRight);
	}

	if (inputMgr->keyIsJustPressed(KeyType::kbLeft))
	{
		motion.x--;
		ent->setState(EntityAnimationState::Left);
	}
	if (inputMgr->keyIsPressed(KeyType::kbLeft))
	{
		motion.x--;
	}

	if (inputMgr->keyIsJustReleased(KeyType::kbLeft))
	{
		ent->setState(EntityAnimationState::IdleLeft);
	}

	if (inputMgr->keyIsPressed(KeyType::kbUp))
	{
		motion.y--;
	}

	if (inputMgr->keyIsPressed(KeyType::kbDown))
	{
		motion.y++;
	}
	ent->move(motion);
	

}

void GameMgr::end()
{
}

void GameMgr::paint()
{
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