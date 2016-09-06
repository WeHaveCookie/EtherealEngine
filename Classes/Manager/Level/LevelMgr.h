#pragma once
#include "Manager/Manager.h"

class Quadtree;
class Entity;

class LevelMgr : public Manager
{
    public:

		static LevelMgr* getSingleton() { return s_singleton; }

        LevelMgr();
        virtual ~LevelMgr();

		void init();
		void process(const float dt);
		void end();
		void paint();

		void registerEntity(Entity* ent);
		std::vector<Entity*> getEntityAround(sf::FloatRect bound);
		Quadtree* getQuadtree() { return m_quadtree; }

    protected:
    private:
        // Function
		static LevelMgr*	s_singleton;
		Quadtree*			m_quadtree;
};

