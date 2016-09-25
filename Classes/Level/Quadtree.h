#pragma once
#include "Entity/Entity.h"

class Quadtree
{
    public:

        Quadtree(float x, float y, float width, float height, Quadtree* master);
		Quadtree();
        ~Quadtree();
		
		void init(float x, float y, float width, float height);
		void registerEntity(std::vector<Entity*> entitys);
		bool registerEntity(Entity* ent);
		void unregisterEntity(std::vector<uint32_t> ids);
		void unregisterEntity(uint32_t id);
		void update();
        
		std::vector<Entity*> queryRange(Entity* ent, sf::FloatRect bound, EntityType::Enum type);
		std::vector<Entity*> getAllElements();

		sf::FloatRect getShape() {return m_shape;}

        void clear();
        void paint();

        bool isEnable() {return m_enable;}
		void setNodeCapacity(uint32_t value) { s_quadNodeCapacity = value; }

		static int getRegisterCount() { int tmp = s_registerCount; s_registerCount = 0; return tmp; }
		static int getUnregisterCount() { int tmp = s_unregisterCount; s_unregisterCount = 0; return tmp; }
		static int getQueryCount() { int tmp = s_queryCount; s_queryCount = 0; return tmp; }
		int getMasterRegisterCount() { int tmp = m_masterRegisterCount; m_masterRegisterCount = 0; return tmp; }

    protected:
      
    private:
		
        void subdivide();
        void merge();
		bool isEmpty() { return m_entitys.empty(); }
		uint32_t nbElement();
		bool children() {  return m_northWest != NULL && m_northEast != NULL && m_southWest != NULL && m_southEast != NULL; }
		void insertEntityOnMap(std::vector<Entity*> entitys);
		void addToRegistrary(Entity* ent);
		void processRegistrary();

		std::vector<Entity*> getElements(Entity* ent = NULL, EntityType::Enum type = EntityType::All);
		std::vector<uint32_t> getIds();

		Quadtree*										m_master;
        Quadtree*										m_northWest;
        Quadtree*										m_northEast;
        Quadtree*										m_southWest;
        Quadtree*										m_southEast;
        std::map<uint32_t, Entity*>						m_entitys;
        sf::RectangleShape								m_boundary;
        sf::FloatRect									m_shape;
        bool											m_enable;
		static uint32_t									s_quadNodeCapacity;
		static int										s_registerCount;
		static int										s_unregisterCount;
		static int										s_queryCount;
		int												m_masterRegisterCount;
		std::map<uint32_t, Entity*>						m_registrary;
};
