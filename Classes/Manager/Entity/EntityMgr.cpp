#include "stdafx.h"
#include "EntityMgr.h"

#include "../../External/rapidjson/document.h"
#include "Entity/EntityPool.h"
#include "Manager/Loading/LoadingMgr.h"

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

}

void EntityMgr::init()
{
	m_processTime = sf::Time::Zero;
}

void EntityMgr::process(const float dt)
{
	sf::Clock clock;
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
		if(ImGui::Button("Close All"))
		{
			for (auto& entity : m_entitys->getEntitys())
			{
				entity->closeInfo();
			}
		}
		ImGui::Text("Used entitys : %i", getNumberUsedEntity());
		for (auto& entity : m_entitys->getEntitys())
		{
			if (!entity->isAlive() && showAll)
			{
				ImGui::Text("%i : unused", entity->getUID());
			}
			else if (entity->isAlive())
			{
				ImGui::Text("%i : %s", entity->getUID(), entity->getName());
				if(ImGui::IsItemClicked())
				{
					entity->showInfo();
				}
			}
		}
		ImGui::End();
	}
}


uint32_t EntityMgr::buildEntity(const char* path)
{
	return LoadingMgr::getSingleton()->loadAsync(m_entitys->getNextEntity(), path);
}

void EntityMgr::removeEntity(uint32_t id)
{
	m_entitys->release(id);
}

Entity* EntityMgr::getEntity(uint32_t id)
{
	return m_entitys->getEntity(id);
}

int EntityMgr::getNumberUsedEntity()
{
	return m_entitys->getNumberUsedEntity();
}

bool EntityMgr::entityIsLoaded(uint32_t id)
{
	return LoadingMgr::getSingleton()->isLoaded(id);
}