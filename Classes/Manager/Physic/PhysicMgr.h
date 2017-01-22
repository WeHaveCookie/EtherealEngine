#pragma once

#include "Manager/Manager.h"
#include "Entity/Entity.h"

class PhysicMgr : public Manager
{
    public:

		static PhysicMgr* getSingleton() { return s_singleton; }

        PhysicMgr();
        virtual ~PhysicMgr();

		void init();
		void process(const float dt);
		void end();
		void showImGuiWindow(bool* window);

		void registerEntity(Entity* ent);
		void unregisterEntity(Entity* ent);

		void unregisterProjectile(Entity* ent);
		void applyGravity();

        // Function
        static bool CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circle);
        static bool CollisionAABBandAABB(sf::FloatRect box1, sf::FloatRect box2);
		static bool RayCastAABBAndAABB(sf::FloatRect box1Last, sf::FloatRect box1, sf::FloatRect box2);
		static bool CollissionAABBAndSeg(sf::FloatRect box1, Vector2 sStart, Vector2 sEnd);
		static bool CollisionLineAndSeg(Vector2	dStart, Vector2 dEnd, Vector2 sStart, Vector2 sEnd);
		static bool CollisionSegAndSeg(Vector2 s1Start, Vector2 s1End, Vector2 s2Start, Vector2 s2End);
		static Vector2 GetCollisionPointSegAndScreenBorder(Vector2 s1Start, Vector2 s1End);
		static Vector2 GetCollisionPointSegAndSeg(Vector2 s1Start, Vector2 s1End, Vector2 s2Start, Vector2 s2End);
		bool CollisionEntToOthers(Entity* ent);
		std::vector<Entity*> GetEntityInArea(Entity* ent, ShootType::Enum shootType);
		static bool CollisionKDOPAndPoint(sf::ConvexShape* kdop, Vector2 pointPos, int count = 0);

		sf::Time getProcessTime() { return m_processTime; }
		
		const double getGravity() const { return m_gravity; }
		void enable(bool b) { m_enable = b; }

		void processCollisionCore();
        // Inline
    protected:
	private:
		void processPhysic(const float dt);
		bool tryToFall(Entity* ent);
		static PhysicMgr* s_singleton;
		void checkValidityOfPosition(Entity* ent);
		void processRegisteryQueue();
		void processProjectile();

		std::vector<Entity*>							m_entitys;
		std::vector<Entity*>							m_projectiles;
		class RegisteryQueue;
		RegisteryQueue*									m_registeryQueue;
		sf::Time										m_processTime;
		bool											m_enable;
		double											m_gravity;
};
