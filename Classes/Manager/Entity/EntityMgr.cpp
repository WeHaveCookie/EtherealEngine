#include "stdafx.h"
#include "EntityMgr.h"

#include "../../External/rapidjson/document.h"
#include "Entity/EntityPool.h"

EntityMgr::EntityMgr()
:Manager(ManagerType::Enum::Entity)
{
	m_pool = new EntityPool(100);
}

EntityMgr::~EntityMgr()
{

}

void EntityMgr::init()
{
	buildEntity("Data/Character/player.json");
	buildEntity("Data/Character/chicken.json");
	buildEntity("Data/Character/cow.json");
	buildEntity("Data/Character/pig.json");
	buildEntity("Data/Character/client.json");
	buildEntity("Data/Character/client.json");
	buildEntity("Data/Character/client.json");
	buildEntity("Data/Character/client.json");
	buildEntity("Data/Character/entrepreneur.json");
	buildEntity("Data/Character/entrepreneur.json");
	buildEntity("Data/Character/entrepreneur.json");
	buildEntity("Data/Character/entrepreneur.json");
}

void EntityMgr::process(const float dt)
{
	m_pool->process(dt);
}

void EntityMgr::end()
{

}

void EntityMgr::paint()
{
	m_pool->paint();

// 	if(ImGui::Begin("EntityMgr"))
// 	{
// 		for (auto it = m_entitys.begin(); it != m_entitys.end(); it++)
// 		{
// 
// 			if (ImGui::TreeNode(std::to_string((*it)->getId()).c_str()))
// 			{
// 				for (auto &state : EnumToString)
// 				{
// 					if (ImGui::TreeNode(state))
// 					{
// 						auto entAnim = (*it)->getAnimation(StateToEnum[state]);
// 						auto anim = entAnim->m_animation;
// 						int count = 0;
// 						for (auto it = anim.begin(); it < anim.end(); it++)
// 						{
// 							ImGui::Text(std::to_string(count).c_str());
// 							if (ImGui::IsItemHovered())
// 							{
// 								sf::Sprite mumu = *it;
// 								const sf::Texture* bulbe = mumu.getTexture();
// 								ImGui::Image((void*)bulbe, ImVec2(bulbe->getSize().x, bulbe->getSize().y));
// 							}
// 							count++;
// 						}
// 					}
// 					ImGui::TreePop();
// 				}
// 			}
// 			ImGui::TreePop();
// 		}
// 	}

}

void EntityMgr::buildEntity(const char* path)
{
	m_pool->create(path);
}

Entity* EntityMgr::getEntity(unsigned int id)
{
	return NULL;
}