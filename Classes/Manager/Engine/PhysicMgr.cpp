#include "stdafx.h"
#include "PhysicMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Utils/Random.h"
#include "blockingconcurrentqueue.h"

PhysicMgr* PhysicMgr::s_singleton = NULL;

class PhysicMgr::RegisteryQueue : public moodycamel::BlockingConcurrentQueue<Entity*> {};

PhysicMgr::PhysicMgr()
:Manager(ManagerType::Enum::Engine)
{
	s_singleton = this;
	m_enable = true;
	m_gravity = 9.81f; // m/sec
	m_registeryQueue = new RegisteryQueue();
}

PhysicMgr::~PhysicMgr()
{
}

void PhysicMgr::init()
{
	m_processTime = sf::Time::Zero;
}

void PhysicMgr::process(const float dt)
{
	sf::Clock clock;
	processRegisteryQueue();
	if (m_enable)
	{
		for (auto& entity : m_entitys)
		{
			entity->addMotion(sf::Vector2f(0.0f, m_gravity * dt));
			checkValidityOfPosition(entity);
		}
	}
	m_processTime = clock.getElapsedTime();
}

void PhysicMgr::end()
{

}

void PhysicMgr::showImGuiWindow(bool* window)
{
	if (ImGui::Begin("PhysicMgr", window))
	{
		ImGui::Checkbox("Enable Phys", &m_enable);
		ImGui::Text("Registery entity : %i", m_entitys.size());
		for (auto& entity : m_entitys)
		{
			ImGui::Text("%i : %s", entity->getUID(), entity->getName());
			if (ImGui::IsItemClicked())
			{
				entity->showInfo();
			}
		}
		ImGui::End();
	}
}

void PhysicMgr::registerEntity(Entity* ent)
{
	m_registeryQueue->enqueue(ent);
}

void PhysicMgr::unregisterEntity(Entity* ent)
{
	auto it = std::find(m_entitys.begin(), m_entitys.end(), ent);
	if (it != m_entitys.end())
	{
		LevelMgr::getSingleton()->unregisterEntity(ent->getUID());
		m_entitys.erase(it);

	}
}

bool PhysicMgr::CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circle)
{
   float d2 = (box1.left-circle.getPosition().x)*(box1.left-circle.getPosition().x) + (box1.top-circle.getPosition().y)*(box1.top-circle.getPosition().y);
   if (d2>pow(circle.getRadius(),2))
      return false;
   else
      return true;
}

bool PhysicMgr::CollisionAABBandAABB(sf::FloatRect box1, sf::FloatRect box2)
{
    return !((box2.left >= box1.left + box1.width) ||
        (box2.left  + box2.width <= box1.left) ||
        (box2.top >= box1.top + box1.height) ||
        (box2.top + box2.height <= box1.top));
}

void PhysicMgr::checkValidityOfPosition(Entity* ent)
{
	auto entityCollided = LevelMgr::getSingleton()->getEntityAround(ent->getGlobalBounds());
	for (auto& entity : entityCollided)
	{
		if(ent->getUID() != entity->getUID() && CollisionAABBandAABB(ent->getGlobalBounds(), entity->getGlobalBounds()))
		{
			ent->roolback();
			entity->roolback();
		}
	}
}

void PhysicMgr::processRegisteryQueue()
{
	Entity* ent;
	bool dequeue = m_registeryQueue->try_dequeue(ent);
	while (dequeue)
	{
		m_entitys.push_back(ent);
		if (ent->isCollidable())
		{
			LevelMgr::getSingleton()->registerEntity(ent);
		}
		dequeue = m_registeryQueue->try_dequeue(ent);
	}
}