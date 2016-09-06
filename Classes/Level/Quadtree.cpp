#include "stdafx.h"
#include "Quadtree.h"
#include "Entity/Entity.h"
#include "Manager/Render/RenderMgr.h"

uint32_t Quadtree::m_quadNodeCapacity = 10;

Quadtree::Quadtree(float x, float y, float width, float height, Quadtree* master)
	:Quadtree()
{
	init(x, y, width, height);
	m_master = master;
}

Quadtree::Quadtree()
{
	m_boundary.setFillColor(sf::Color(0, 0, 0, 0));
	m_boundary.setOutlineThickness(1.0f);
	m_boundary.setOutlineColor(sf::Color::Green);
	m_northWest = NULL;
	m_northEast = NULL;
	m_southWest = NULL;
	m_southEast = NULL;
	m_enable = true;
	m_master = this;
}

Quadtree::~Quadtree()
{
	if (!m_enable) {
		delete m_northWest;
		delete m_northEast;
		delete m_southWest;
		delete m_southEast;
	}
	m_entitys.clear();
}

void Quadtree::init(float x, float y, float width, float height)
{
	m_boundary.setPosition(x, y);
	m_boundary.setSize(sf::Vector2f(width, height));
	m_shape = sf::FloatRect(x, y, width, height);
}

void Quadtree::registerEntity(std::vector<Entity*> entitys)
{
	for (auto& entity : entitys)
	{
		registerEntity(entity);
	}
}

bool Quadtree::registerEntity(Entity* ent)
{
    sf::FloatRect bound = ent->getGlobalBounds();
    if (!m_shape.intersects(bound))
    {
        return false;
    } else
    {
		if (m_entitys.find(ent->getUID()) != m_entitys.end() && m_enable)
		{ // Already exist
			m_entitys[ent->getUID()] = ent;
			return true;
		}

        if (m_entitys.size() < m_quadNodeCapacity && m_enable)
        {
			m_entitys[ent->getUID()] = ent;
            return true;
        } else
        {
            if(m_entitys.size() >= m_quadNodeCapacity && m_enable)
            {
                subdivide();
            }

            sf::FloatRect shapeNW = m_northWest->getShape();
            sf::FloatRect shapeNE = m_northEast->getShape();
            sf::FloatRect shapeSW = m_southWest->getShape();
            sf::FloatRect shapeSE = m_southEast->getShape();

            if(shapeNW.intersects(bound))
            {
                if(shapeSW.intersects(bound))
                {
                    if(shapeSE.intersects(bound) && shapeNE.intersects(bound))
                    {
                        m_northWest->registerEntity(ent);
                        m_northEast->registerEntity(ent);
                        m_southWest->registerEntity(ent);
                        m_southEast->registerEntity(ent);
					} else
					{
						m_northWest->registerEntity(ent);
						m_southWest->registerEntity(ent);
					}
                } else if (shapeNE.intersects(bound))
                {
                    m_northWest->registerEntity(ent);
                    m_northEast->registerEntity(ent);
                } else
                {
                    m_northWest->registerEntity(ent);
                }
            } else if (shapeNE.intersects(bound))
            {
                if(shapeSE.intersects(bound))
                {
                    m_northEast->registerEntity(ent);
                    m_southEast->registerEntity(ent);

                } else
                {
                    m_northEast->registerEntity(ent);
                }
            } else if (shapeSW.intersects(bound))
            {
                if(shapeSE.intersects(bound))
                {
                    m_southWest->registerEntity(ent);
                    m_southEast->registerEntity(ent);
                } else
                {
                    m_southWest->registerEntity(ent);
                }
            } else if (shapeSE.intersects(bound))
            {
                m_southEast->registerEntity(ent);
            }
            return true;
        }
        return false;
    }
}

void Quadtree::unregisterEntity(std::vector<uint32_t> ids)
{
	for (auto& id : ids)
	{
		unregisterEntity(id);
	}
}

void Quadtree::unregisterEntity(uint32_t id)
{
	if (!m_enable)
	{
		m_northWest->unregisterEntity(id);
		m_northEast->unregisterEntity(id);
		m_southWest->unregisterEntity(id);
		m_southEast->unregisterEntity(id);
	} else
	{
		m_entitys.erase(id);
	}
}

void Quadtree::update()
{
	if (!m_enable && children())
	{
		m_northWest->update();
		m_northEast->update();
		m_southWest->update();
		m_southEast->update();
		if (m_entitys.empty())
		{
			if ((m_northWest->nbElement() + m_northEast->nbElement() + m_southWest->nbElement() + m_southEast->nbElement()) <= m_quadNodeCapacity)
			{
				merge();
			}
		}
	} else
	{
		std::vector<uint32_t> removedEntity;
		for (auto& entity : m_entitys)
		{
			auto entityBound = entity.second->getGlobalBounds();
			// Not in shape. We need to erase this entity and register on the master
			if (!m_shape.intersects(entityBound))
			{
				m_master->registerEntity(entity.second);
				removedEntity.push_back(entity.first);
			}
			else if ((entityBound.left + entityBound.width > m_shape.left + m_shape.width) ||
				(entityBound.left < m_shape.left) ||
				(entityBound.top + entityBound.height > m_shape.top + m_shape.height) ||
				(entityBound.top < m_shape.top))
			{
				m_master->registerEntity(entity.second);
			}
		}

		for (auto& id : removedEntity)
		{
			m_entitys.erase(id);
		}
	}
}

 std::vector<Entity*> Quadtree::queryRange(sf::FloatRect bound)
{
     std::vector<Entity*> answer =  std::vector<Entity*>();
    if(!m_shape.intersects(bound))
    {
        return answer;
    }
    if(m_enable)
    {
        answer = getElements();
        return answer;
    } else
    {
        std::vector<Entity*> tmp = std::vector<Entity*>();
        if(m_northWest->getShape().intersects(bound))
        {
            tmp = m_northWest->queryRange(bound);
            answer.insert(answer.end(),tmp.begin(),tmp.end());
        }
        if (m_northEast->getShape().intersects(bound))
        {
            tmp = m_northEast->queryRange(sf::FloatRect(bound));
            answer.insert(answer.end(),tmp.begin(),tmp.end());
        }
        if (m_southWest->getShape().intersects(bound))
        {
            tmp = m_southWest->queryRange(sf::FloatRect(bound));
            answer.insert(answer.end(),tmp.begin(),tmp.end());
        }
        if (m_southEast->getShape().intersects(bound))
        {
            tmp = m_southEast->queryRange(sf::FloatRect(bound));
            answer.insert(answer.end(),tmp.begin(),tmp.end());
        }
        tmp.clear();
        return answer;
    }
}

void Quadtree::subdivide()
{
    m_northWest = new Quadtree(m_shape.left, m_shape.top, m_shape.width/2.0f, m_shape.height/2.0f, m_master);
    m_northEast = new Quadtree(m_shape.left+(m_shape.width/2.0f), m_shape.top, m_shape.width/2.0f, m_shape.height/2.0f, m_master);
    m_southWest = new Quadtree(m_shape.left, m_shape.top+(m_shape.height/2.0f), m_shape.width/2.0f, m_shape.height/2.0f, m_master);
    m_southEast = new Quadtree(m_shape.left+(m_shape.width/2.0f), m_shape.top+(m_shape.height/2.0f), m_shape.width/2.0f, m_shape.height/2.0f, m_master);
    m_enable = false;
    if(!m_entitys.empty())
    {
		for (auto& entity : m_entitys)
		{
			registerEntity(entity.second);
		}
        m_entitys.clear();
    }

}

void Quadtree::merge()
{
	insertEntityOnMap(m_northWest->getElements());
	insertEntityOnMap(m_northEast->getElements());
	insertEntityOnMap(m_southWest->getElements());
	insertEntityOnMap(m_southEast->getElements());
    clear();
}

void Quadtree::clear()
{
    if(!m_enable) 
	{
        m_northWest->clear();
        m_northEast->clear();
        m_southWest->clear();
        m_southEast->clear();
        delete m_northWest;
        delete m_northEast;
        delete m_southWest;
        delete m_southEast;
        m_northWest = NULL;
        m_northEast = NULL;
        m_southWest = NULL;
        m_southEast = NULL;
        m_enable = true;
    } else 
	{
        m_entitys.clear();
    }
}

void Quadtree::paint()
{
	auto rdrWin = RenderMgr::getSingleton()->getMainRenderWindow();
	rdrWin->draw(m_boundary);
    if(m_entitys.empty() && children())
    {
        m_northWest->paint();
        m_northEast->paint();
        m_southWest->paint();
        m_southEast->paint();
    }
}

uint32_t Quadtree::nbElement()
{
    if(m_enable)
    {
        return m_entitys.size();
    } else
    {
        return m_northWest->nbElement() + m_northEast->nbElement() + m_southWest->nbElement() + m_southEast->nbElement();
    }
}

std::vector<Entity*> Quadtree::getElements()
{
	std::vector<Entity*> elements;
	for (auto& entity : m_entitys)
	{
		elements.push_back(entity.second);
	}
	return elements;
}

std::vector<uint32_t> Quadtree::getIds()
{
	std::vector<uint32_t> ids;
	for (auto& entity : m_entitys)
	{
		ids.push_back(entity.first);
	}
	return ids;
}

void Quadtree::insertEntityOnMap(std::vector<Entity*> entitys)
{
	for (auto& entity : entitys)
	{
		m_entitys[entity->getUID()] = entity;
	}
}
