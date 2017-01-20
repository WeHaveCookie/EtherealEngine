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
	:Manager(ManagerType::Enum::Physic)
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
	m_gravity = 9.81F;
}

void PhysicMgr::process(const float dt)
{
	sf::Clock clock;
	processRegisteryQueue();
	processPhysic(dt);
	if (m_enable)
	{
		for (auto& entity : m_entitys)
		{
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
			ImGui::Text("%i : %s", entity->getUID(), entity->getName());
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
			if (/*entity->isFall() && */entity->getType() != EntityType::Anchor && !entity->isSleeping())
			{
				entity->addMotion(Vector2(0, m_gravity));
			}
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

bool PhysicMgr::RayCastAABBAndAABB(sf::FloatRect box1Last, sf::FloatRect box1, sf::FloatRect box2)
{
	Vector2 aLast, bLast, cLast, dLast, centerLast, a, b, c, d, center;
	aLast.x = box1Last.left;
	aLast.y = box1Last.top;

	a.x = box1.left;
	a.y = box1.top;

	bLast.x = box1Last.left + box1Last.width;
	bLast.y = box1Last.top;

	b.x = box1.left + box1.width;
	b.y = box1.top;

	cLast.x = box1Last.left + box1Last.width;
	cLast.y = box1Last.top + box1.height;

	c.x = box1.left + box1.width;
	c.y = box1.top + box1.height;

	dLast.x = box1Last.left;
	dLast.y = box1Last.top + box1Last.height;

	d.x = box1.left;
	d.y = box1.top + box1.height;

	centerLast.x = box1Last.left + (box1Last.width / 2.0f);
	centerLast.y = box1Last.top + (box1Last.height / 2.0f);

	center.x = box1.left + (box1.width / 2.0f);
	center.y = box1.top + (box1.height / 2.0f);

	return (CollissionAABBAndSeg(box2, centerLast, center) || CollissionAABBAndSeg(box2, aLast, a) || CollissionAABBAndSeg(box2, bLast, b) || CollissionAABBAndSeg(box2, cLast, c) || CollissionAABBAndSeg(box2, dLast, d));
}

bool PhysicMgr::CollissionAABBAndSeg(sf::FloatRect box1, Vector2 sStart, Vector2 sEnd)
{
	Vector2 a, b, c, d;
	a.x = box1.left;
	a.y = box1.top;
	
	b.x = box1.left + box1.width;
	b.y = box1.top;
	
	c.x = box1.left + box1.width;
	c.y = box1.top + box1.height;

	d.x = box1.left;
	d.y = box1.top + box1.height;
	
	return CollisionSegAndSeg(a, b, sStart, sEnd) || CollisionSegAndSeg(b, c, sStart, sEnd) || CollisionSegAndSeg(c, d, sStart, sEnd) || CollisionSegAndSeg(d, a, sStart, sEnd);
}

bool PhysicMgr::CollisionLineAndSeg(Vector2 dStart, Vector2 dEnd, Vector2 sStart, Vector2 sEnd)
{
	Vector2 AO, AP, AB;
	AB.x = dEnd.x - dStart.x;
	AB.y = dEnd.y - dStart.y;
	AP.x = sEnd.x - dStart.x;
	AP.y = sEnd.y - dStart.y;
	AO.x = sStart.x - dStart.x;
	AO.y = sStart.y - dStart.y;
	if ((AB.x*AP.y - AB.y*AP.x)*(AB.x*AO.y - AB.y*AO.x) < 0)
		return true;
	else
		return false;
}

bool PhysicMgr::CollisionSegAndSeg(Vector2 s1Start, Vector2 s1End, Vector2 s2Start, Vector2 s2End)
{
	if (CollisionLineAndSeg(s1Start, s1End, s2Start, s2End) == false)
		return false;
	if (CollisionLineAndSeg(s2Start, s2End, s1Start, s1End) == false)
		return false;
	return true;
}


CollisionState::Enum getCollisionState(sf::FloatRect box1, sf::FloatRect box2)
{

	if ((box1.left > box2.left + box2.width && box1.top > box2.top + box2.height) ||
		(box1.left + box1.width < box2.left && box1.top > box1.top + box1.height) ||
		(box1.left + box1.width < box2.left && box1.top + box1.height < box2.top) ||
		(box1.left > box2.left + box2.width && box1.top + box1.height < box2.top))
	{
		return CollisionState::None;
	}

	bool left = (box1.left + box1.width <= box2.left);
	bool right = (box1.left >= box2.left + box2.width);
	bool top = (box1.top + box1.height <= box2.top);
	bool bottom = (box1.top >= box2.top + box2.height);

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
	if (!ent->collisionResolved() && ent->getType() != EntityType::Anchor)
	{
		ent->proceedCollision();
		if (!ent->isSleeping())
		{
			sf::FloatRect entLastGlobalBounds = ent->getLastGlobalBounds();
			const uint32_t entID = ent->getUID();

			auto anchorCollided = LevelMgr::getSingleton()->getEntityAround(ent, entLastGlobalBounds, EntityType::Anchor);
			auto movableCollided = LevelMgr::getSingleton()->getEntityAround(ent, entLastGlobalBounds, EntityType::Movable);

			for (auto& anchor : anchorCollided)
			{
				auto anchorID = anchor->getUID();
				sf::FloatRect anchorGlobalBounds = anchor->getGlobalBounds();
				sf::FloatRect entGlobalBounds = ent->getGlobalBounds();
				sf::FloatRect entLastGlobalBounds = ent->getLastGlobalBounds();
				
				bool collisionAABBAndAABB = CollisionAABBandAABB(entGlobalBounds, anchorGlobalBounds);
				bool rayCastAABBAndAABB = RayCastAABBAndAABB(entLastGlobalBounds, entGlobalBounds, anchorGlobalBounds);
				if (collisionAABBAndAABB || rayCastAABBAndAABB)
				{
					sf::FloatRect entGlobalBounds = ent->getGlobalBounds();
					CollisionState::Enum collision = getCollisionState(anchorGlobalBounds, entLastGlobalBounds);

					if (collision == CollisionState::None)
					{
						sf::FloatRect rollBackPos = entLastGlobalBounds;
						rollBackPos.left -= ent->getLastMotion().x;
						rollBackPos.top -= ent->getLastMotion().y;
						collision = getCollisionState(anchorGlobalBounds, rollBackPos);
					}

					auto collisionState = ent->getCollisionState();

					switch (collision)
					{
					case CollisionState::Right:
					{
						if ((collisionState & CollisionState::Right) != CollisionState::Right)
						{
							Vector2 impulse(anchorGlobalBounds.left - (entGlobalBounds.left + entGlobalBounds.width + 1.0f), 0.0f);
							ent->rollback(impulse);
							ent->setCollisionState(CollisionState::Right);
						}
					}
					break;
					case CollisionState::Left:
					{
						if ((collisionState & CollisionState::Left) != CollisionState::Left)
						{
							Vector2 impulse(anchorGlobalBounds.left + anchorGlobalBounds.width + 1.0f - entGlobalBounds.left, 0.0f);
							ent->rollback(impulse);
							ent->setCollisionState(CollisionState::Left);
						}
					}
					break;
					case CollisionState::Bottom:
					{
						if ((collisionState & CollisionState::Bottom) != CollisionState::Bottom)
						{
							Vector2 impulse(0.0f, anchorGlobalBounds.top - (entGlobalBounds.top + entGlobalBounds.height + 1.0f));
							ent->rollback(impulse);
							ent->setFall(false);
							ent->setCollisionState(CollisionState::Bottom);
						}
					}
					break;
					case CollisionState::Top:
					{
						if ((collisionState & CollisionState::Top) != CollisionState::Top)
						{
							Vector2 impulse(0.0f, anchorGlobalBounds.top + anchorGlobalBounds.height + 1.0f - entGlobalBounds.top);
							ent->rollback(impulse);
							ent->setCollisionState(CollisionState::Top);
						}
					}
					break;
					case CollisionState::None:
						// Intentional fall through
					default:
						break;
					}
				}
			}

			for (auto& collider : movableCollided)
			{
				auto colliderID = collider->getUID();
				sf::FloatRect entLastGlobalBounds = ent->getLastGlobalBounds();
				sf::FloatRect entGlobalBounds = ent->getGlobalBounds();
				sf::FloatRect colliderGlobalBounds = collider->getGlobalBounds();
				
				auto motion = ent->getMotion();
				auto colliderPosition = collider->getPosition();
				Vector2 entPosition = ent->getPosition();

				if ((motion.x < 0.0f && colliderPosition.x < entPosition.x + entGlobalBounds.width) || // Go to left and collider at left
					(motion.x > 0.0f && colliderPosition.x + colliderGlobalBounds.width > entPosition.x) || // Go to right and collider at right
					(motion.y < 0.0f && colliderPosition.y < entPosition.y + entGlobalBounds.height) || // Go to top and collider at top
					(motion.y > 0.0f && colliderPosition.y + colliderGlobalBounds.height > entPosition.y)) // Go to bottom and collider at bottom
				{
					if (ent->getDistance(collider) < entGlobalBounds.height * 1.5f + colliderGlobalBounds.height)
					{
						if (collider->isSleeping()/* && ent->getCounterSleeping() < 5*/)
						{
							collider->wakeUp();
						}
						if (!collider->collisionResolved() && !collider->collisionProcessOngoing())
						{
							checkValidityOfPosition(collider);
						}
					}
				}

				colliderGlobalBounds = collider->getGlobalBounds();
				bool collisionAABBAndAABB = CollisionAABBandAABB(entGlobalBounds, colliderGlobalBounds);
				bool rayCastAABBAndAABB = RayCastAABBAndAABB(entLastGlobalBounds, entGlobalBounds, colliderGlobalBounds);
				if (collisionAABBAndAABB || rayCastAABBAndAABB)
				{
					auto colliderPosition = collider->getPosition();
					Vector2 entPosition = ent->getPosition();
				
					auto colliderGlobalBounds = collider->getGlobalBounds();
					auto entCollisionState = ent->getCollisionState();
					auto motion = ent->getMotion();

					CollisionState::Enum collision = getCollisionState(collider->getLastGlobalBounds(), entLastGlobalBounds);

					if (collision == CollisionState::None)
					{
						sf::FloatRect rollBackPos = entLastGlobalBounds;
						rollBackPos.left -= ent->getLastMotion().x;
						rollBackPos.top -= ent->getLastMotion().y;
						collision = getCollisionState(collider->getLastGlobalBounds(), rollBackPos);
					}

					auto collisionState = ent->getCollisionState();

					switch (collision)
					{
					case CollisionState::Right:
					{
						if ((collisionState & CollisionState::Right) != CollisionState::Right)
						{
							if (motion.x > 0.0f)
							{
								Vector2 impulse(colliderGlobalBounds.left - (entGlobalBounds.left + entGlobalBounds.width + 1.0f), 0.0f);
								ent->rollback(impulse);
								ent->setCollisionState(CollisionState::Right);
							}
						}
					}
					break;
					case CollisionState::Left:
					{
						if ((collisionState & CollisionState::Left) != CollisionState::Left)
						{
							if (motion.x < 0.0f)
							{
								Vector2 impulse(colliderGlobalBounds.left + colliderGlobalBounds.width + 1.0f - entGlobalBounds.left, 0.0f);
								ent->rollback(impulse);
								ent->setCollisionState(CollisionState::Left);
							}
						}
					}
					break;
					case CollisionState::Bottom:
					{
						if ((collisionState & CollisionState::Bottom) != CollisionState::Bottom)
						{
							if (motion.y > 0.0f)
							{
								Vector2 impulse(0.0f, colliderGlobalBounds.top - (entGlobalBounds.top + entGlobalBounds.height + 1.0f));
								ent->rollback(impulse);
								ent->setFall(false);
								ent->setCollisionState(CollisionState::Bottom);
							}
						}
					}
					break;
					case CollisionState::Top:
					{
						if ((collisionState & CollisionState::Top) != CollisionState::Top)
						{
							if (motion.y < 0.0f)
							{
								Vector2 impulse(0.0f, colliderGlobalBounds.top + colliderGlobalBounds.height + 1.0f - entGlobalBounds.top);
								ent->rollback(impulse);
								ent->setCollisionState(CollisionState::Top);
							}
						}
					}
					break;
					case CollisionState::None:
						// Intentional fall through
					default:
						break;
					}
				}
			}
			ent->resetCollisionState();
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

void PhysicMgr::processPhysic(const float dt)
{
	for (auto& entity : m_entitys)
	{
		if (entity->isEdition())
		{
			entity->move(entity->getMotion());
			continue;
		}
		auto entID = entity->getUID();
		entity->setCollisionResolved(false);
		Vector2 motion = entity->getMotion()*(dt * 100);

		float v_grav = entity->getVGrav();
		float v_y = entity->getVY();
		motion.y += v_y;
		v_y += v_grav;
		entity->setVY(v_y);

		if (entity->isJump())
		{

			float v_x = entity->getVX();
			float v_saut = entity->getVJump();
			motion.x += v_x;
		}
		else
		{
			if (motion == Vector2(0.0, entity->getVMax()))
			{
				if (!entity->isFall())
				{
					if (!tryToFall(entity))
					{
						motion.y = 0.0f;
						entity->sleep();
					}
				}
			}
		}
		entity->move(motion);
	}
}

bool PhysicMgr::tryToFall(Entity* ent)
{
	auto pos = ent->getPosition();
	auto newPos = pos + Vector2(0.0f, ent->getVY());
	auto globalBounds = ent->getGlobalBounds();
	sf::FloatRect newGlobalBounds = sf::FloatRect(newPos.x, newPos.y, globalBounds.width, globalBounds.height);
	auto anchorCollided = LevelMgr::getSingleton()->getEntityAround(ent, newGlobalBounds, EntityType::Anchor);
	auto movableCollided = LevelMgr::getSingleton()->getEntityAround(ent, newGlobalBounds, EntityType::Movable);

	for (auto& anchor : anchorCollided)
	{
		auto anchorGlobalBounds = anchor->getGlobalBounds();
		bool collisionAABBAndAABB = CollisionAABBandAABB(newGlobalBounds, anchorGlobalBounds);
		bool rayCastAABBAndAABB = RayCastAABBAndAABB(globalBounds, newGlobalBounds, anchorGlobalBounds);

		if (collisionAABBAndAABB || rayCastAABBAndAABB)
		{
			sf::FloatRect entGlobalBounds = ent->getGlobalBounds();
			CollisionState::Enum collision = getCollisionState(anchorGlobalBounds, globalBounds);
			if (collision == CollisionState::Bottom)
			{
				return false;
			}
			
		}
	}
	
	for (auto& collider : movableCollided)
	{
		auto colliderGlobalBounds = collider->getGlobalBounds();
		bool collisionAABBAndAABB = CollisionAABBandAABB(newGlobalBounds, colliderGlobalBounds);
		bool rayCastAABBAndAABB = RayCastAABBAndAABB(globalBounds, newGlobalBounds, colliderGlobalBounds);
		
		if (collisionAABBAndAABB || rayCastAABBAndAABB)
		{
			CollisionState::Enum collision = getCollisionState(collider->getLastGlobalBounds(), globalBounds);
			if (collision == CollisionState::Bottom)
			{
				return false;
			}
		}
	}
	return true;
}