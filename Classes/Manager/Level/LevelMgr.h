#pragma once
#include "Manager/Manager.h"
#include "Entity/Entity.h"

class Quadtree;
class Entity;
class Level;

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
		void showImGuiWindow(bool* window);

		void registerEntity(Entity* ent);
		void unregisterEntity(uint32_t id);
		std::vector<Entity*> getEntityAround(Entity* ent, sf::FloatRect bound, EntityType::Enum type = EntityType::All);
		Quadtree* getQuadtree() { return m_quadtree; }

		sf::Time getProcessTime() { return m_processTime; }
		int getRegisterCount();
		int getUnregisterCount();
		int getMasterRegisterCount();
		int getMasterQueryCount() { int tmp = m_queryCount; m_queryCount = 0; return tmp; }
		int getQueryCount();

		bool loadLevel(char* path);
		void unloadLevel();
		uint32_t loadLevelAsync();
		void registerEntityIntoLevel(Entity* ent);
		const char* getCharacterPath();
		const sf::View getCamera() const;
		void shake(bool b);

		const bool isPlayableLevel() const;
		void killEnemyType(ShootType::Enum type);
		void startExtermination();
		void endExtermination();

    protected:
    private:
        // Function
		static LevelMgr*	s_singleton;
		Quadtree*			m_quadtree;
		sf::Time			m_processTime;
		int					m_queryCount;
		Level*				m_level;
};

