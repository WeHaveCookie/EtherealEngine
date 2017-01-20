#include "stdafx.h"
#include "GuiMgr.h"

#include "Manager/Time/TimeMgr.h"
#include "Manager/Sound/SoundMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Item/ItemMgr.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Event/EventMgr.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Render/RenderMgr.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/File/FileMgr.h"
#include "Manager/Persistent/PersistentMgr.h"
#include "Manager/Loading/LoadingMgr.h"

GuiMgr* GuiMgr::s_singleton = NULL;

struct GuiMgr::Flags
{
	bool show_TimeMgr;
	bool show_SoundMgr;
	bool show_LevelMgr;
	bool show_ItemMgr;
	bool show_InputMgr;
	bool show_GameMgr;
	bool show_EventMgr;
	bool show_PhysicMgr;
	bool show_RenderMgr;
	bool show_EntityMgr;
	bool show_FileMgr;
	bool show_PersistentMgr;
	bool show_LoadingMgr;
	bool show_Demo;

	Flags()
		:show_TimeMgr(false),
		show_SoundMgr(false),
		show_LevelMgr(false),
		show_ItemMgr(false),
		show_InputMgr(false),
		show_GameMgr(false),
		show_EventMgr(false),
		show_PhysicMgr(false),
		show_RenderMgr(false),
		show_EntityMgr(false),
		show_FileMgr(false),
		show_PersistentMgr(false),
		show_LoadingMgr(false),
		show_Demo(false)
	{

	}
};

GuiMgr::GuiMgr()
	:Manager(ManagerType::Gui)
{
	s_singleton = this;
}

GuiMgr::~GuiMgr()
{
	delete m_flags;
}

void GuiMgr::init()
{
	m_flags = new Flags();
	m_displayGui = true;
}

void GuiMgr::process(const float dt)
{
	if (InputMgr::getSingleton()->keyIsJustPressed(KeyType::mouseRight))
	{
		m_displayGui = !m_displayGui;
	}
}

void GuiMgr::paint()
{
	if (m_displayGui)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Manager"))
			{
				ImGui::MenuItem("EntityMgr", NULL, &m_flags->show_EntityMgr);
				ImGui::MenuItem("PhysicMgr", NULL, &m_flags->show_PhysicMgr);
				ImGui::MenuItem("LevelMgr", NULL, &m_flags->show_LevelMgr);
				ImGui::MenuItem("LoadingMgr", NULL, &m_flags->show_LoadingMgr);
				ImGui::MenuItem("GameMgr", NULL, &m_flags->show_GameMgr);
				ImGui::MenuItem("SoundMgr", NULL, &m_flags->show_SoundMgr);
				ImGui::MenuItem("InputMgr", NULL, &m_flags->show_InputMgr);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("ImGuiDemo", NULL, &m_flags->show_Demo);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (m_flags->show_EntityMgr)
		{
			EntityMgr::getSingleton()->showImGuiWindow(&m_flags->show_EntityMgr);
		}

		if (m_flags->show_PhysicMgr)
		{
			PhysicMgr::getSingleton()->showImGuiWindow(&m_flags->show_PhysicMgr);
		}

		if (m_flags->show_LevelMgr)
		{
			LevelMgr::getSingleton()->showImGuiWindow(&m_flags->show_LevelMgr);
		}

		if (m_flags->show_LoadingMgr)
		{
			LoadingMgr::getSingleton()->showImGuiWindow(&m_flags->show_LoadingMgr);
		}

		if (m_flags->show_GameMgr)
		{
			GameMgr::getSingleton()->showImGuiWindow(&m_flags->show_GameMgr);
		}

		if (m_flags->show_SoundMgr)
		{
			SoundMgr::getSingleton()->showImGuiWindow(&m_flags->show_SoundMgr);
		}

		if (m_flags->show_InputMgr)
		{
			InputMgr::getSingleton()->showImGuiWindow(&m_flags->show_InputMgr);
		}

		if (m_flags->show_Demo)
		{
			ImGui::ShowTestWindow(&m_flags->show_Demo);
		}

		EntityMgr::getSingleton()->displayEntitysInfos();
	}
}

void GuiMgr::end()
{

}
