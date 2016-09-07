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

		void registerEntity(Entity* ent);
		void unregisterEntity(Entity* ent);

        // Function
        static bool CollisionAABBAndCircle(sf::FloatRect box1, sf::CircleShape circle);
        static bool CollisionAABBandAABB(sf::FloatRect box1, sf::FloatRect box2);

		sf::Time getProcessTime() { return m_processTime; }

        // Inline
    protected:
	private:
		static PhysicMgr* s_singleton;
		void checkValidityOfPosition(Entity* ent);

		std::vector<Entity*>	m_entitys;
		sf::Time				m_processTime;
		float					m_gravity;
};
