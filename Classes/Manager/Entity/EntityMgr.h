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

		Entity* createEntity(const char* path) const;
		uint32_t createAsyncEntity(const char* path) const;
		void deleteEntity(uint32_t id);
		Entity* getEntity(uint32_t id) const;
		Entity* getMainCharacter() const { return getEntity(m_mainCharacterID); }
		void setMainCharacter(uint32_t id) { m_mainCharacterID = id; }
		const bool entityIsLoaded(uint32_t id) const;
		int getNumberUsedEntity();
		
		sf::Time getProcessTime() { return m_processTime; }

		void displayEntitysInfos();

	private:
		static EntityMgr*		s_singleton;
		EntityPool*				m_entitys;
		sf::Time				m_processTime;
		uint32_t				m_mainCharacterID;
		bool					m_onEdition;
};