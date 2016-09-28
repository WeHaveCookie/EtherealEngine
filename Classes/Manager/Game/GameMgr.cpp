#include "stdafx.h"
#include "GameMgr.h"
#include "EtherealDreamManagers.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Sound/SoundMgr.h"
#include "Thread/LoadingThread.h"
#include "Manager/Persistent/PersistentMgr.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Loading/LoadingMgr.h"
#include "Manager/Action/CommandMgr.h"
#include "Actions/CommandMove.h"

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
	m_nbrPlayers = 0;
}

GameMgr::~GameMgr()
{
	delete m_mainRenderWindow;
}

void GameMgr::init()
{
	m_processTime = sf::Time::Zero;
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	if (vm.height > 1080 || vm.width > 1920)
	{
		vm = sf::VideoMode(1920, 1080);
	}
	m_mainRenderWindow = new sf::RenderWindow(vm, "Ethereal Dream", sf::Style::Default /*| sf::Style::Fullscreen*/);
	setFrameRate(60);
	m_mainRenderWindow->setVerticalSyncEnabled(true);
	srand(time(NULL));

	auto soundMgr = SOUND_MGR;
	strcpy(m_gameName, GAME_NAME);
	m_movementSpeed = 5;
	setNumberPlayer(0);
}

void GameMgr::process(const float dt)
{
	sf::Clock clock;

	auto inputMgr = INPUT_MGR;
	auto entityMgr = ENTITY_MGR;
	auto soundMgr = SOUND_MGR;
	auto loadThread = LOAD_THREAD;
	Vector2 motion = { 0.0f, 0.0f };
	static SaveTask save;
	static std::vector<uint32_t> ids;

	auto ent = entityMgr->getMainCharacter();

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
		int framerate = m_framerate;
		ImGui::InputInt("Framerate", &framerate);
		setFrameRate(framerate);
		ImGui::InputInt("Movement Speed", &m_movementSpeed);
		int nbrPlayers = m_nbrPlayers;
		ImGui::InputInt("Number Player", &nbrPlayers);
		setNumberPlayer(nbrPlayers);
		if (ImGui::CollapsingHeader("Manager"))
		{
			ImGui::Text("EntityMgr : %f ms", EntityMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("PhysicMgr : %f ms", PhysicMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("LevelMgr : %f ms", LevelMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("LoadingMgr : %f ms", LoadingMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("GameMgr : %f ms", getProcessTime().asMicroseconds() / 1000.0f);
			ImGui::Text("SoundMgr : %f ms", SoundMgr::getSingleton()->getProcessTime().asMicroseconds() / 1000.0f);
		}
	}
	ImGui::End();
}

const bool GameMgr::isRunning() const
{
	return m_mainRenderWindow->isOpen();
}

void GameMgr::setFrameRate(uint32_t frameRate)
{
	if (frameRate >= 10)
	{
		m_framerate = frameRate;
		m_mainRenderWindow->setFramerateLimit(frameRate);
		g_DeltaTime = 1.0f / (float)frameRate;
	}
}

Entity* GameMgr::getEntityPlayer(uint32_t id)
{
	return EntityMgr::getSingleton()->getEntity(m_playersId[id]);
}

void GameMgr::setNumberPlayer(uint32_t nbr)
{
	if (m_nbrPlayers != nbr && nbr >= 0 && nbr <= sf::Joystick::Count)
	{
		if (m_nbrPlayers < nbr)
		{
			for (uint32_t i = m_nbrPlayers; i < nbr; i++)
			{
				m_playersId.push_back(EntityMgr::getSingleton()->createEntity("Data/Character/assassinAttack.json")->getUID());
			}
		}
		else
		{
			for (int i = m_nbrPlayers - 1; i >= (int)nbr; i--)
			{
				auto ent = m_playersId[i];
				EntityMgr::getSingleton()->deleteEntity(ent);
				m_playersId.pop_back();
			}
		}
		m_nbrPlayers = nbr;
	}
}