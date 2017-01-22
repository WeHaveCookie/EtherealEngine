#include "stdafx.h"
#include "EntityMgr.h"

#include "../../External/rapidjson/document.h"
#include "Entity/EntityPool.h"
#include "Manager/Loading/LoadingMgr.h"
#include "Manager/File/FileMgr.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Utils/wcharUtils.h"
#include "Manager/Input/InputMgr.h"
#include "Utils/Random.h"
#include "Entity/Entity.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Game/GameMgr.h"
#include "EtherealEngineManagers.h"

#define SINUSPATH "Data/FX/redWave.json"
#define SPIKEPATH "Data/FX/greenWave.json"
#define TRIANGLEPATH "Data/FX/blueWave.json"

EntityMgr* EntityMgr::s_singleton = NULL;
uint32_t Entity::newUID = 0;

EntityMgr::EntityMgr()
:Manager(ManagerType::Enum::Entity)
{
	s_singleton = this;
	m_entitys = new EntityPool(1000);
}

EntityMgr::~EntityMgr()
{
	delete m_entitys;
}

void EntityMgr::init()
{
	m_processTime = sf::Time::Zero;
	m_onEdition = false;
	m_timerBlue = 0.0f;
	m_timerGreen = 0.0;
	m_timerRed = 0.0f;
	m_timerGlobal = 0.0f;
	m_delayGreen = 0.5f;
	m_delayBlue = 0.5f;
	m_delayRed = 0.5f;
	m_delayGlobal = 0.2f;
}

void EntityMgr::process(const float dt)
{
	sf::Clock clock;
	m_timerRed -= dt;
	m_timerGreen -= dt;
	m_timerBlue -= dt;
	m_timerGlobal -= dt;

	auto core = getEntity("Core");
	if (core != NULL)
	{
		core->setAngle(core->getAngle() + (0.5 * DEGTORAD));
	}
	auto entitys = m_entitys->getUsedEntitysSortedHTL();
	for (auto& entity : entitys)
	{
		auto mouseCurrentPosition = InputMgr::getSingleton()->getMousePosition();
		if (entity->getLastGlobalBounds().contains(mouseCurrentPosition.sf()) && entity->getType() == EntityType::Button)
		{
			entity->setState(EntityAnimationState::IdleRight);
		}
		else if (entity->getType() == EntityType::Button)
		{
			entity->setState(EntityAnimationState::IdleLeft);
		}
		if (entity->getLastGlobalBounds().contains(mouseCurrentPosition.sf()) && InputMgr::getSingleton()->keyIsJustPressed(KeyType::mouseLeft))
		{
			entity->showInfo();
			entity->onClick();
		}
		if (entity->getLastGlobalBounds().contains(mouseCurrentPosition.sf()) && InputMgr::getSingleton()->keyIsJustPressed(KeyType::mouseWheelButton))
		{
			if(m_onEdition && !entity->isEdition())
			{
				continue;
			}
			entity->edition();
			m_onEdition = entity->isEdition();
			break;
		}
	}
	m_entitys->process(dt);
	m_processTime = clock.getElapsedTime();
}

void EntityMgr::end()
{
	free(m_entitys);
}

void EntityMgr::paint()
{
	m_entitys->paint();
}

void EntityMgr::showImGuiWindow(bool* window)
{
	static bool showAll = false;
	if (ImGui::Begin("EntityMgr", window))
	{
		ImGui::Checkbox("Show all entity", &showAll);
		ImGui::SameLine();
		if (ImGui::Button("Close All"))
		{
			for (auto& entity : m_entitys->getEntitys())
			{
				entity->closeInfo();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Open All"))
		{
			for (auto& entity : m_entitys->getUsedEntitys())
			{
				entity->closeInfo();
				entity->showInfo();
			}
		}
		ImGui::Text("Used entitys : %i", getNumberUsedEntity());

		std::vector<std::wstring> files;
		FileMgr::GetFilesInDirectory(files, L"Data/Character", L".json");

		char** filesLabel = (char**) malloc(sizeof(char*) * files.size());
		
		for (unsigned int i = 0; i < files.size(); i++)
		{
			filesLabel[i] = (char*)malloc(sizeof(char) * files[i].size() + 1); // +1 for null terminated
			strcpy(filesLabel[i], WideChartoAscii(files[i]).c_str());
		}

		static int itemID = 0;

		ImGui::Combo("Entity", &itemID, (const char**)filesLabel, files.size());
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			createAsyncEntity(filesLabel[itemID]);
		}


		for (unsigned int i = 0; i < files.size(); i++)
		{
			free(filesLabel[i]);
		}
		free(filesLabel);

		auto mainChar = getMainCharacter();
		if (mainChar != NULL)
		{
			ImGui::Text("Main character : %i", mainChar->getUID());
		}
		if (ImGui::IsItemClicked())
		{
			getMainCharacter()->showInfo();
		}

		if(ImGui::CollapsingHeader("Entitys"))
		{
			if (mainChar != NULL)
			{
				int mainID = mainChar->getUID();
				int previousMainID = mainID;
				ImGui::InputInt("Main Character", &mainID);
				if (mainID != previousMainID)
				{
					if (mainID >= 0 && mainID < m_entitys->getPoolSize())
					{
						setMainCharacter(mainID);
					}
				}
				ImGui::Text("Main character : %i | %s", getMainCharacter()->getUID(), getMainCharacter()->getName());
			}
			for (auto& entity : m_entitys->getEntitys())
			{
				if (!entity->isAlive() && showAll)
				{
					ImGui::Text("%i : unused", entity->getUID());
				}
				else if (entity->isAlive())
				{
					ImGui::Text("%i : %s", entity->getUID(), entity->getName());
					if (ImGui::IsItemClicked())
					{
						entity->showInfo();
					}
					std::string label = "Delete###" + std::to_string(entity->getUID());
					ImGui::SameLine();
					if (ImGui::Button(label.c_str()))
					{
						deleteEntity(entity->getUID());
					}
				}
			}
		}
	}
	ImGui::End();
}

Entity* EntityMgr::createEntity(const char* path)  const
{
	Entity* newEntity = m_entitys->getNextEntity();
	LoadingMgr::getSingleton()->load(newEntity, path);
	return newEntity;
}

uint32_t EntityMgr::createAsyncEntity(const char* path)  const
{
	return LoadingMgr::getSingleton()->loadAsync(m_entitys->getNextEntity(), path);
}

void EntityMgr::deleteEntity(uint32_t id)
{
	m_entitys->release(id);
}

Entity* EntityMgr::getEntity(uint32_t id)  const
{
	return m_entitys->getEntity(id);
}

Entity* EntityMgr::getEntity(std::string name) const
{
	return m_entitys->getEntity(name);
}

int EntityMgr::getNumberUsedEntity()
{
	return m_entitys->getNumberUsedEntity();
}

const bool EntityMgr::entityIsLoaded(uint32_t id) const
{
	return LoadingMgr::getSingleton()->isLoaded(id);
}

void EntityMgr::displayEntitysInfos()
{
	auto entitys = m_entitys->getUsedEntitys();
	for (auto& entity : entitys)
	{
		entity->showImGuiWindow();
	}
}

const bool EntityMgr::isValidEntity(uint32_t UID) const
{
	return m_entitys->getEntity(UID)->isAlive();
}

void EntityMgr::spawnIntoRegion(uint32_t id, sf::FloatRect region)
{
	auto ent = m_entitys->getEntity(id);
	auto x = randFloatBorned(region.left, region.left + region.width);
	auto y = randFloatBorned(region.top, region.top + region.height );
	ent->setPosition(Vector2(x,y));
}

void EntityMgr::createShoot(ShootType::Enum shootType)
{
	if (m_timerGlobal > 0.0f)
	{
		return;
	}
	m_timerGlobal = m_delayGlobal;
	if (!LevelMgr::getSingleton()->isPlayableLevel())
	{
		return;
	}
	Entity* ent;
	Vector2 direction;
	float angle;
	switch (shootType)
	{
	case ShootType::Sinus:
	{
		if (m_timerRed > 0.0f)
		{
			return;
		}
		m_timerRed = m_delayRed;
		ent = createEntity(SINUSPATH);
		auto bound = ent->getGlobalBounds();
		direction = Vector2(bound.left + (bound.width / 2.0f), -100000.0f) -
			Vector2(bound.left + (bound.width / 2.0f), bound.top + (bound.height / 2.0));

// 		auto direction = Vector2(bound.left, -100000.0f) -
// 			Vector2(bound.left, bound.top);
		direction %= -60.0f * DEGTORAD;
		angle = 25.0f * DEGTORAD;
		break;
	}
	case ShootType::Spike:
	{
		if (m_timerGreen > 0.0f)
		{
			return;
		}
		m_timerGreen = m_delayGreen;
		ent = createEntity(SPIKEPATH);
		auto bound = ent->getGlobalBounds();
		direction = Vector2(bound.left + (bound.width / 2.0f), -100000.0f) -

		//auto direction = Vector2(bound.left + (bound.width / 2.0f), -100000.0f) -
			Vector2(bound.left + (bound.width / 2.0f), bound.top + (bound.height / 2.0));
		direction %= 60.0f * DEGTORAD;
		angle = 220.0f * DEGTORAD;
		break;
	}
	case ShootType::Triangle:
	{
		if (m_timerBlue > 0.0f)
		{
			return;
		}
		m_timerBlue = m_delayBlue;
		ent = createEntity(TRIANGLEPATH);
		auto bound = ent->getGlobalBounds();
		direction = Vector2(bound.left + (bound.width / 2.0f), -100000.0f) - 
			Vector2(bound.left + (bound.width / 2.0f), bound.top + (bound.height / 2.0));
		direction %= 180.0f * DEGTORAD;
		angle = 180.0f * DEGTORAD;
		
		break;
	}
	default:
		return;
		break;
	}
	
	if (ent != nullptr)
	{
		auto player = getMainCharacter();
		if (player != NULL)
		{
			direction %= player->getAngle();
			ent->setTarget(direction);
			ent->setAngle(player->getAngle() + angle);
			PhysicMgr::getSingleton()->registerEntity(ent);
			LevelMgr::getSingleton()->registerEntity(ent);
		}
	}
}

const bool EntityMgr::playerIsDead() const
{
	auto player = getEntity("Core");
	if (player == NULL)
	{
		return false;
	}
	else
	{
		return player->isDead();
	}
}

Entity* EntityMgr::getMainCharacter() const
{
	auto ent = getEntity(m_mainCharacterID);
	if (ent != NULL && ent->isAlive())
	{
		return ent;
	}
	return NULL;
}

void EntityMgr::unload()
{
	auto entities = m_entitys->getUsedEntitys();
	for (auto& ent : entities)
	{
		m_entitys->release(ent->getUID());
	}
}