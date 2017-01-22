#pragma once

#include "Manager/Manager.h"

class EntityPool;
class Entity;

namespace ShootType
{
	enum Enum
	{
		None,
		Sinus,
		Spike,
		Triangle
	};

	static ShootType::Enum GetType(const char* name)
	{
		if (strcmp(name, "Sinus"))
		{
			return ShootType::Sinus;
		}
		else if (strcmp(name, "Spike"))
		{
			return ShootType::Spike;
		}
		else if (strcmp(name, "Triangle"))
		{
			return ShootType::Triangle;
		}
		else
		{
			return ShootType::None;
		}
	}
}

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
		Entity* getEntity(std::string name) const;
		Entity* getMainCharacter() const { return getEntity(m_mainCharacterID); }
		void setMainCharacter(uint32_t id) { m_mainCharacterID = id; }
		const bool entityIsLoaded(uint32_t id) const;
		int getNumberUsedEntity();
		void spawnIntoRegion(uint32_t id, sf::FloatRect region);
		
		const bool isValidEntity(uint32_t UID) const;
		sf::Time getProcessTime() { return m_processTime; }

		void displayEntitysInfos();

		void createShoot(ShootType::Enum shootType);

	private:
		static EntityMgr*		s_singleton;
		EntityPool*				m_entitys;
		sf::Time				m_processTime;
		uint32_t				m_mainCharacterID;
		bool					m_onEdition;
};