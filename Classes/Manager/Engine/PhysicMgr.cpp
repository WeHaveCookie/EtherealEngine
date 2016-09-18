#include "stdafx.h"
#include "PhysicMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Utils/Random.h"
#include "blockingconcurrentqueue.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Entity/EntityMgr.h"

PhysicMgr* PhysicMgr::s_singleton = NULL;

class PhysicMgr::RegisteryQueue : public moodycamel::BlockingConcurrentQueue<Entity*> {};

PhysicMgr::PhysicMgr()
	:Manager(ManagerType::Enum::Engine),
	m_gravity(0.0f, 9.81f)
{
	s_singleton = this;
	m_enable = true;
	m_registeryQueue = new RegisteryQueue();
}

PhysicMgr::~PhysicMgr()
{
}

void PhysicMgr::init()
{
	m_processTime = sf::Time::Zero;
	registerEntity(EntityMgr::getSingleton()->createEntity("Data/Character/ground.json"));
}

void PhysicMgr::process(const float dt)
{
	sf::Clock clock;
	processRegisteryQueue();

	if (m_enable)
	{
		for (auto& entity : m_entitys)
		{
			auto mouseCurrentPosition = InputMgr::getSingleton()->getMousePosition();
			if (entity->getGlobalBounds().contains(mouseCurrentPosition) && InputMgr::getSingleton()->keyIsJustPressed(KeyType::mouseLeft))
			{
				entity->showInfo();
			}
			if (entity->getGlobalBounds().contains(mouseCurrentPosition) && InputMgr::getSingleton()->keyIsJustPressed(KeyType::mouseWheelButton))
			{
				entity->editable();
			}
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
		for (auto& entity : m_entitys)
		{
			ImGui::Text("%i : %s - %f", entity->getUID(), entity->getName(), entity->getMass());
			if (ImGui::IsItemClicked())
			{
				entity->showInfo();
			}
		}
	}
	ImGui::End();
}

void PhysicMgr::registerEntity(Entity* ent)
{
	m_registeryQueue->enqueue(ent);
}

void PhysicMgr::unregisterEntity(Entity* ent)
{
	auto pos = std::find(m_entitys.begin(), m_entitys.end(), ent);
	if (pos != m_entitys.end())
	{
		LevelMgr::getSingleton()->unregisterEntity(ent->getUID());
		m_entitys.erase(pos);
	}
}

bool PhysicMgr::CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circle)
{
   float d2 = (box1.left-circle.getPosition().x)*(box1.left-circle.getPosition().x) + (box1.top-circle.getPosition().y)*(box1.top-circle.getPosition().y);
   if (d2 > circle.getRadius() * circle.getRadius())
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

struct CollisionState
{
	enum Enum
	{
		XAxis,
		YAxis,
		None,
	};
};

CollisionState::Enum getCollisionState(sf::FloatRect box1, sf::FloatRect box2)
{
	if ((box1.left + box1.width < box2.left) || (box1.left > box2.left + box2.width))
	{
		return CollisionState::XAxis;
	}
	else if ((box1.top + box1.height < box2.top) || (box1.top > box2.top + box2.height))
	{
		return CollisionState::YAxis;
	}
	else
	{
		return CollisionState::None;
	}
}

void PhysicMgr::checkValidityOfPosition(Entity* ent)
{
	if (ent->asMoved())
	{
		auto entityCollided = LevelMgr::getSingleton()->getEntityAround(ent->getGlobalBounds());
		for (auto& collider : entityCollided)
		{
			while (ent->getUID() != collider->getUID() && CollisionAABBandAABB(ent->getGlobalBounds(), collider->getGlobalBounds()))
			{
				auto collision = getCollisionState(ent->getLastPosition(), collider->getLastPosition());
				if (collision == CollisionState::XAxis)
				{
					ent->rollbackXAxis();
				}
				else if (collision == CollisionState::YAxis)
				{
					ent->rollbackYAxis();
				}
				else
				{
					ent->rollBackAllAxis();
				}
				if (ent->getMotion().x == 0.0f && ent->getMotion().y == 0.0f)
				{
					break;
				}
				ent->retry();
			}
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
		LevelMgr::getSingleton()->registerEntity(ent);
		dequeue = m_registeryQueue->try_dequeue(ent);
	}
}