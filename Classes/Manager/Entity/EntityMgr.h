#pragma once

#include "Manager/Manager.h"
#include "Entity/Entity.h"

class EntityPool;

class EntityMgr : public Manager
{

	public:
		EntityMgr();
		~EntityMgr();

		void init();
		void process(const float dt);
		void end();
		void paint();

		void buildEntity(const char* path);
		Entity* getEntity(unsigned int id);

	private:
		EntityPool*				m_pool;

};