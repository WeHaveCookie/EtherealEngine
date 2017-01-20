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

		void applyGravity();

        // Function
        static bool CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circle);
        static bool CollisionAABBandAABB(sf::FloatRect box1, sf::FloatRect box2);
		static bool RayCastAABBAndAABB(sf::FloatRect box1Last, sf::FloatRect box1, sf::FloatRect box2);
		static bool CollissionAABBAndSeg(sf::FloatRect box1, Vector2 sStart, Vector2 sEnd);
		static bool CollisionLineAndSeg(Vector2	dStart, Vector2 dEnd, Vector2 sStart, Vector2 sEnd);
		static bool CollisionSegAndSeg(Vector2 s1Start, Vector2 s1End, Vector2 s2Start, Vector2 s2End);

		sf::Time getProcessTime() { return m_processTime; }
		
		const double getGravity() const { return m_gravity; }

        // Inline
    protected:
	private:
		void processPhysic(const float dt);
		bool tryToFall(Entity* ent);
		static PhysicMgr* s_singleton;
		void checkValidityOfPosition(Entity* ent);
		void processRegisteryQueue();

		std::vector<Entity*>							m_entitys;
		class RegisteryQueue;
		RegisteryQueue*									m_registeryQueue;
		sf::Time										m_processTime;
		bool											m_enable;
		double											m_gravity;
};
