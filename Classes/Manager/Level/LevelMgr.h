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
		void unregisterEntity(uint32_t id);
		std::vector<Entity*> getEntityAround(sf::FloatRect bound);
		Quadtree* getQuadtree() { return m_quadtree; }

		sf::Time getProcessTime() { return m_processTime; }
		int getRegisterCount();
		int getUnregisterCount();
		int getMasterRegisterCount();
		int getMasterQueryCount() { int tmp = m_queryCount; m_queryCount = 0; return tmp; }
		int getQueryCount();

    protected:
    private:
        // Function
		static LevelMgr*	s_singleton;
		Quadtree*			m_quadtree;
		sf::Time			m_processTime;
		int					m_queryCount;
};

