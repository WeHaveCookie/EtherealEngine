#include "stdafx.h"
#include "EntityMgr.h"

#include "../../External/rapidjson/document.h"

EntityMgr::EntityMgr()
:Manager(ManagerType::Enum::Entity)
{
	buildEntity("Data/Character/testPlayer.json");
	buildEntity("Data/Character/chicken.json");
}

EntityMgr::~EntityMgr()
{

}

void EntityMgr::init()
{
}

void EntityMgr::process(const float dt)
{
	for (auto it = m_entitys.begin(); it != m_entitys.end(); it++)
	{
		(*it)->update();
	}
}

void EntityMgr::end()
{

}

void EntityMgr::paint()
{
	for (auto it = m_entitys.begin(); it != m_entitys.end(); it++)
	{
		(*it)->paint();
	}

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
	Entity* ent = new Entity(path);
	m_entitys.push_back(ent);
}