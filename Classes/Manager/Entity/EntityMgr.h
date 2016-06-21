#pragma once

#include "Manager/Manager.h"

class Entity;

class EntityMgr : public Manager
{
	struct EntityBuilder {
		const char*						m_id;
		int								m_width;
		int								m_height;
		float							m_speed;
		std::vector<EntityAnimation>	m_entityAnimations;
	};

	public:
		EntityMgr();
		~EntityMgr();

		void Init();
		void Process(const float dt);
		void End();
		void Paint();

		void BuildEntity(const char* path);

	private:
		std::vector<Entity*> m_entitys;

};