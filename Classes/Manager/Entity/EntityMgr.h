#pragma once

#include "Manager/Manager.h"
#include "Entity/Entity.h"

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

		void init();
		void process(const float dt);
		void end();
		void paint();

		void buildEntity(const char* path);

	private:
		std::vector<Entity*> m_entitys;

};