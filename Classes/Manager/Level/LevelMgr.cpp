#include "stdafx.h"
#include "LevelMgr.h"
#include "Level/Quadtree.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Render/RenderMgr.h"
#include "Level/Level.h"
#include "Manager/File/FileMgr.h"
#include "Utils/wcharUtils.h"

LevelMgr* LevelMgr::s_singleton = NULL;

LevelMgr::LevelMgr()
:Manager(ManagerType::Enum::Level)
{
	s_singleton = this;
	m_queryCount = 0.0f;
}

LevelMgr::~LevelMgr()
{
	delete m_quadtree;
}

void LevelMgr::init()
{
	m_processTime = sf::Time::Zero;
	auto winSize = GameMgr::getSingleton()->getMainRenderWindow()->getSize();
	m_quadtree = new Quadtree();
	m_quadtree->init(0.0f, 0.0f, winSize.x, winSize.y);
	m_quadtree->setNodeCapacity(50);
	m_level = new Level();
	loadLevel("Data/Level/mainMenu.json");
}

void LevelMgr::process(const float dt)
{
	sf::Clock clock;
	m_quadtree->update();
	if (m_level->cameraShake())
	{
		m_level->processShake(dt);
	}
	m_processTime = clock.getElapsedTime();
}

void LevelMgr::end()
{
}

void LevelMgr::paint()
{
	m_level->paint();
	//m_quadtree->paint();
}

void LevelMgr::showImGuiWindow(bool* window)
{
	if (ImGui::Begin("LevelMgr", window))
	{
		if (ImGui::Button("SHAKE !!!!!"))
		{
			m_level->shake(true);
		}

		ImGui::Text("Local Sinus : %i => total %i", m_level->getLocalSinusDead(), m_level->getTotalSinusDead());
		ImGui::Text("Local Spike : %i => total %i", m_level->getLocalSpikeDead(), m_level->getTotalSpikeDead());
		ImGui::Text("Local Triangle : %i => total %i", m_level->getLocalTriangleDead(), m_level->getTotalTriangleDead());
		ImGui::Text("Score total : %i", m_level->getScore());
		auto view = m_level->getCamera().getCenter();
		auto size = m_level->getCamera().getSize();
		ImGui::Text("View : x = %f | y = %f | w = %f | h = %f", view.x, view.y, size.x, size.y);
		if(ImGui::CollapsingHeader("Quadtree"))
		{
			auto elements = m_quadtree->getAllElements();
			for (auto& element : elements)
			{
				ImGui::Text("%i - %s", element->getUID(), element->getName());
				if (ImGui::IsItemClicked())
				{
					element->showInfo();
				}
			}
		}

		ImGui::Text("Current Level : %s", m_level->getName());
		ImGui::Text("x = %f; y = %f", m_level->getSize().x, m_level->getSize().y);
		
		std::vector<std::wstring> files;
		FileMgr::GetFilesInDirectory(files, L"Data/Level", L".json");

		char** filesLabel = (char**)malloc(sizeof(char*) * files.size());

		for (unsigned int i = 0; i < files.size(); i++)
		{
			filesLabel[i] = (char*)malloc(sizeof(char) * files[i].size() + 1); // +1 for null terminated
			strcpy(filesLabel[i], WideChartoAscii(files[i]).c_str());
		}

		static int itemID = 0;

		ImGui::Combo("Level", &itemID, (const char**)filesLabel, files.size());
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			unloadLevel();
			loadLevel(filesLabel[itemID]);
		}
		ImGui::SameLine();
		if (ImGui::Button("Unload"))
		{
			unloadLevel();
		}

		for (unsigned int i = 0; i < files.size(); i++)
		{
			free(filesLabel[i]);
		}
		free(filesLabel);
	}
	ImGui::End();
}

void LevelMgr::registerEntity(Entity* ent)
{
	m_level->registerEntity(ent);
}

void LevelMgr::unregisterEntity(uint32_t id)
{
	m_quadtree->unregisterEntity(id);
}

std::vector<Entity*> LevelMgr::getEntityAround(Entity* ent, sf::FloatRect bound, EntityType::Enum type)
{
	m_queryCount++;
	return m_quadtree->queryRange(ent, bound, type);
}

int LevelMgr::getRegisterCount() 
{ 
	return m_quadtree->getRegisterCount(); 
}

int LevelMgr::getUnregisterCount() 
{ 
	return m_quadtree->getUnregisterCount(); 
}

int LevelMgr::getMasterRegisterCount()
{
	return m_quadtree->getMasterRegisterCount();
}

int LevelMgr::getQueryCount()
{
	return m_quadtree->getQueryCount();
}

bool LevelMgr::loadLevel(char* path)
{
	unloadLevel();
	m_level->load(path);
	if (isPlayableLevel())
	{
		m_level->clearScore();
	}
	return true;
}

uint32_t LevelMgr::loadLevelAsync()
{
	return 1;
}

void LevelMgr::unloadLevel()
{
	m_level->unload();
}

void LevelMgr::registerEntityIntoLevel(Entity* ent)
{
	registerEntity(ent);
	m_level->registerEntity(ent);
}

const char* LevelMgr::getCharacterPath()
{
	return m_level->getCharacterPath();
}

const sf::View LevelMgr::getCamera() const 
{ 
	return m_level->getCamera(); 
}

void LevelMgr::shake(bool b) 
{ 
	m_level->shake(b); 
}

const bool LevelMgr::isPlayableLevel() const
{
	return m_level->isLoaded() && strcmp(m_level->getName(), "mainMenu") != 0 && strcmp(m_level->getName(), "credits") != 0 && strcmp(m_level->getName(), "gameOver") != 0;
}

void LevelMgr::killEnemyType(ShootType::Enum type)
{
	m_level->killEnemyType(type);
}

void LevelMgr::startExtermination()
{
	m_level->startExtermination();
}

void LevelMgr::endExtermination()
{
	m_level->endExtermination();
}
