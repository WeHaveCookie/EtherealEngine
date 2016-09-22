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
	:Manager(ManagerType::Enum::Physic),
	m_gravity(0.0f, 9.81f)
{
	s_singleton = this;
	m_enable = true;
	m_registeryQueue = new RegisteryQueue();
}

PhysicMgr::~PhysicMgr()
{
	delete m_registeryQueue;
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
		bool editableDone = false;
		for (auto& entity : m_entitys)
		{
			auto mouseCurrentPosition = InputMgr::getSingleton()->getMousePosition();
			if (entity->getGlobalBounds().contains(mouseCurrentPosition) && InputMgr::getSingleton()->keyIsJustPressed(KeyType::mouseLeft))
			{
				entity->showInfo();
			}
			if (entity->getGlobalBounds().contains(mouseCurrentPosition) && InputMgr::getSingleton()->keyIsJustPressed(KeyType::mouseWheelButton) && !editableDone)
			{
				editableDone = true;
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

void PhysicMgr::applyGravity()
{
	if (m_enable)
	{
		for (auto& entity : m_entitys)
		{
			entity->addMotion(m_gravity);
		}
	}
}

bool PhysicMgr::CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circle)
{
	float d2 = (box1.left - circle.getPosition().x)*(box1.left - circle.getPosition().x) + (box1.top - circle.getPosition().y)*(box1.top - circle.getPosition().y);
	if (d2 > circle.getRadius() * circle.getRadius())
		return false;
	else
		return true;
}

bool PhysicMgr::CollisionAABBandAABB(sf::FloatRect box1, sf::FloatRect box2)
{
	return !((box2.left >= box1.left + box1.width) ||
		(box2.left + box2.width <= box1.left) ||
		(box2.top >= box1.top + box1.height) ||
		(box2.top + box2.height <= box1.top));
}

CollisionState::Enum getCollisionState(sf::FloatRect box1, sf::FloatRect box2)
{
	if (box1.left + box1.width <= box2.left)
	{
		return CollisionState::Left;
	}
	else if (box1.left >= box2.left + box2.width)
	{
		return CollisionState::Right;
	}
	else if (box1.top + box1.height <= box2.top)
	{
		return CollisionState::Top;
	}
	else if (box1.top >= box2.top + box2.height)
	{
		return CollisionState::Bottom;
	}
	else
	{
		return CollisionState::None;
	}
}

void PhysicMgr::checkValidityOfPosition(Entity* ent)
{
	if (!ent->collisionResolved() && !ent->isAnchor())
	{
		auto entityCollided = LevelMgr::getSingleton()->getEntityAround(ent->getGlobalBounds());
		bool collided = false;
		ent->proceedCollision();
		for (auto& collider : entityCollided)
		{
			auto colliderID = collider->getUID();
			auto entID = ent->getUID();
			auto motion = ent->getLastMotion();
			auto entPosition = ent->getPosition();
			auto colliderPosition = collider->getPosition();
			if ((motion.x < 0.0f && colliderPosition.x < entPosition.x) || // Go to left and collider at left
				(motion.x > 0.0f && colliderPosition.x > entPosition.x) || // Go to right and collider at right
				(motion.y < 0.0f && colliderPosition.y < entPosition.y) || // Go to Top and collider at Top
				(motion.y > 0.0f && colliderPosition.y > entPosition.y)) // Go to left
			{
				if (!collider->collisionResolved() && !collider->collisionProcessOngoing() && !collider->isAnchor())
				{
					checkValidityOfPosition(collider);
				}
			}
			if (ent->getUID() != collider->getUID() && CollisionAABBandAABB(ent->getGlobalBounds(), collider->getGlobalBounds()))
			{
				if (collider->isMovable() && !collider->collisionProcessOngoing())
				{
					//collider->addMotion(m_gravity); // HACKY HACK ! Use for Unresolve collision.... I know it's just a bullshit !
					checkValidityOfPosition(collider);
				}
				auto lastPosition = ent->getLastPosition();
				CollisionState::Enum collision;
				if (collider->isAnchor())
				{
					collision = getCollisionState(lastPosition, collider->getGlobalBounds());
				}
				else
				{
					collision = getCollisionState(lastPosition, collider->getLastPosition());
				}
				auto entCollisionState = ent->getCollisionState();
				auto lastMotion = ent->getLastMotion();
				switch (collision)
				{
				case CollisionState::Right:
					if (((entCollisionState & CollisionState::Right) != CollisionState::Right || collider->isAnchor() || (!ent->isMovable() && motion.x < 0.0f) || collider->collisionResolved()) && (!collider->isMovable() || ent->isMovable()) || collider->collisionResolved())
					{
							ent->setPosition(sf::Vector2f(collider->getPosition().x + collider->getGlobalBounds().width + 1.0f, ent->getPosition().y));
							auto test = ent->getPosition();
							ent->setCollisionState(CollisionState::Left);
					}
					break;
				case CollisionState::Left:
					if (((entCollisionState & CollisionState::Left) != CollisionState::Left || collider->isAnchor() || (!ent->isMovable() && motion.x > 0.0f) || collider->collisionResolved()) && (!collider->isMovable() || ent->isMovable()) || collider->collisionResolved())
					{
							ent->setPosition(sf::Vector2f(collider->getPosition().x - ent->getGlobalBounds().width - 1.0f, ent->getPosition().y));
							auto test = ent->getPosition();
							ent->setCollisionState(CollisionState::Right);
					}
					break;
				case CollisionState::Bottom:
					if (((entCollisionState & CollisionState::Bottom) != CollisionState::Bottom || collider->isAnchor() || (!ent->isMovable() && motion.y < 0.0f) || collider->collisionResolved()) && (!collider->isMovable() || ent->isMovable()) || collider->collisionResolved())
					{
						ent->setPosition(sf::Vector2f(ent->getPosition().x, collider->getPosition().y + collider->getGlobalBounds().height + 1.0f));
						auto test = ent->getPosition();
						ent->setCollisionState(CollisionState::Top);
					}
					break;
				case CollisionState::Top:
					if ((((entCollisionState & CollisionState::Top) != CollisionState::Top) || collider->isAnchor() || (!ent->isMovable() && motion.x > 0.0f)) && (!collider->isMovable() || ent->isMovable()) || collider->collisionResolved())
					{
							ent->setPosition(sf::Vector2f(ent->getPosition().x, collider->getPosition().y - ent->getGlobalBounds().height - 1.0f));
							auto test = ent->getPosition();
							ent->setCollisionState(CollisionState::Bottom);
					}
					break;
				case CollisionState::None:
					// Intentional fall through
				default:
					ent->setPosition(sf::Vector2f(lastPosition.left, lastPosition.top));
					auto test = ent->getPosition();
					ent->setCollisionState(CollisionState::None);
					break;
				}


				ent->retry();
			}
		}
		if (!ent->isMovable())
		{
			ent->resetCollisionState();
		}
		else
		{
			ent->setCollisionProcess(false);
			ent->setCollisionResolved(true);
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