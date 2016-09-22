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
		sf::Time getProcessTime() { return m_processTime; }
		


        // Inline
    protected:
	private:
		static PhysicMgr* s_singleton;
		void checkValidityOfPosition(Entity* ent);
		void processRegisteryQueue();

		std::vector<Entity*>							m_entitys;
		class RegisteryQueue;
		RegisteryQueue*									m_registeryQueue;
		sf::Time										m_processTime;
		bool											m_enable;
		sf::Vector2f									m_gravity;
};
