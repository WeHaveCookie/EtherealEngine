#include "stdafx.h"
#include "LevelMgr.h"
#include "Level/Quadtree.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Render/RenderMgr.h"

LevelMgr* LevelMgr::s_singleton = NULL;

LevelMgr::LevelMgr()
:Manager(ManagerType::Enum::Level)
{
	s_singleton = this;
	m_queryCount = 0.0f;
}

LevelMgr::~LevelMgr()
{
	delete m_quadtree;
}

void LevelMgr::init()
{
	m_processTime = sf::Time::Zero;
	m_quadtree = new Quadtree();
	m_quadtree->init(0.0f, 0.0f, 1920.0f, 1080.0f);
	m_quadtree->setNodeCapacity(50);
	
	sf::Texture* txt = new sf::Texture();
	sf::Sprite* spr = new sf::Sprite();
	txt->loadFromFile("Data/Building/arcane.png");
	spr->setTexture(*txt);
	spr->setPosition(sf::Vector2f(1045.0f, 750.0f));
	m_buildingTexture.push_back(txt);
	m_building[location_type::arcane] = spr;

	txt = new sf::Texture();
	spr = new sf::Sprite();
	txt->loadFromFile("Data/Building/village.png");
	spr->setTexture(*txt);
	spr->setPosition(sf::Vector2f(603.0f, 250.0f));
	m_buildingTexture.push_back(txt);
	m_building[location_type::village] = spr;

	txt = new sf::Texture();
	spr = new sf::Sprite();
	txt->loadFromFile("Data/Building/forest.png");
	spr->setTexture(*txt);
	spr->setPosition(sf::Vector2f(292.0f, 765.0f));
	m_buildingTexture.push_back(txt);
	m_building[location_type::forest] = spr;

	txt = new sf::Texture();
	spr = new sf::Sprite();
	txt->loadFromFile("Data/Building/shack.png");
	spr->setTexture(*txt);
	spr->setPosition(sf::Vector2f(1477.0f, 516.0f));
	m_buildingTexture.push_back(txt);
	m_building[location_type::shack] = spr;

	txt = new sf::Texture();
	spr = new sf::Sprite();
	txt->loadFromFile("Data/Building/goldmine.png");
	spr->setTexture(*txt);
	spr->setPosition(sf::Vector2f(67.0f, 87.0f));
	m_buildingTexture.push_back(txt);
	m_building[location_type::goldmine] = spr;

	txt = new sf::Texture();
	spr = new sf::Sprite();
	txt->loadFromFile("Data/Building/saloon.png");
	spr->setTexture(*txt);
	spr->setPosition(sf::Vector2f(1182.0f, 143.0f));
	m_buildingTexture.push_back(txt);
	m_building[location_type::saloon] = spr;

	txt = new sf::Texture();
	spr = new sf::Sprite();
	txt->loadFromFile("Data/Building/bank.png");
	spr->setTexture(*txt);
	spr->setPosition(sf::Vector2f(18.0f, 432.0f));
	m_buildingTexture.push_back(txt);
	m_building[location_type::bank] = spr;

}

void LevelMgr::process(const float dt)
{
	sf::Clock clock;
	m_quadtree->update();
	m_processTime = clock.getElapsedTime();
}

void LevelMgr::end()
{
}

void LevelMgr::paint()
{
	for (auto& building : m_building)
	{
		RenderMgr::getSingleton()->getMainRenderWindow()->draw(*building.second);
	}
	m_quadtree->paint();
}

void LevelMgr::showImGuiWindow(bool* window)
{
	if (ImGui::Begin("LevelMgr", window))
	{
		if(ImGui::CollapsingHeader("Quadtree"))
		{
			auto elements = m_quadtree->getAllElements();
			for (auto& element : elements)
			{
				ImGui::Text("%i - %s", element->getUID(), element->getName());
				if (ImGui::IsItemClicked())
				{
					element->showInfo();
				}
			}
		}
	}
	ImGui::End();
}

void LevelMgr::registerEntity(Entity* ent)
{
	m_quadtree->registerEntity(ent);
}

void LevelMgr::unregisterEntity(uint32_t id)
{
	m_quadtree->unregisterEntity(id);
}

std::vector<Entity*> LevelMgr::getEntityAround(Entity* ent, sf::FloatRect bound, EntityType::Enum type)
{
	m_queryCount++;
	return m_quadtree->queryRange(ent, bound, type);
}

int LevelMgr::getRegisterCount() 
{ 
	return m_quadtree->getRegisterCount(); 
}

int LevelMgr::getUnregisterCount() 
{ 
	return m_quadtree->getUnregisterCount(); 
}

int LevelMgr::getMasterRegisterCount()
{
	return m_quadtree->getMasterRegisterCount();
}

int LevelMgr::getQueryCount()
{
	return m_quadtree->getQueryCount();
}

const Vector2 LevelMgr::getPosLocation(location_type loc)
{
	auto bound = m_building[loc]->getGlobalBounds();
	return Vector2(bound.left + bound.width, bound.top + (bound.height / 2.0f));
}