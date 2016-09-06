#pragma once

class Entity;

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
        
		std::vector<Entity*> queryRange(sf::FloatRect bound);
        
		sf::FloatRect getShape() {return m_shape;}

        void clear();
        void paint();

        bool isEnable() {return m_enable;}
		void setNodeCapacity(uint32_t value) { m_quadNodeCapacity = value; }

    protected:
      
    private:
		
        void subdivide();
        void merge();
		bool isEmpty() { return m_entitys.empty(); }
		uint32_t nbElement();
		bool children() {  return m_northWest != NULL && m_northEast != NULL && m_southWest != NULL && m_southEast != NULL; }
		void insertEntityOnMap(std::vector<Entity*> entitys);

		std::vector<Entity*> getElements();
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
		static uint32_t									m_quadNodeCapacity;

};
