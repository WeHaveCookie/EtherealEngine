#pragma once

#include "Manager/Manager.h"
#include "Entity/Entity.h"

class EntityPool;

class EntityMgr : public Manager
{

	public:

		static EntityMgr* getSingleton() { return s_singleton; }

		EntityMgr();
		~EntityMgr();

		void init();
		void process(const float dt);
		void end();
		void paint();
		void showImGuiWindow(bool* window);

		uint32_t buildEntity(const char* path);
		void removeEntity(uint32_t id);
		Entity* getEntity(uint32_t id);
		bool entityIsLoaded(uint32_t id);
		int getNumberUsedEntity();

		sf::Time getProcessTime() { return m_processTime; }

	private:
		static EntityMgr*		s_singleton;
		EntityPool*				m_entitys;
		sf::Time				m_processTime;
};