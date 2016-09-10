#pragma once

#include "Manager/Manager.h"

class GuiMgr : public Manager
{
public:

	static GuiMgr* getSingleton() { return s_singleton; }

	GuiMgr();
	~GuiMgr();

	void init();
	void process(const float dt);
	void paint();
	void end();

private:
	struct Flags;
	
	static GuiMgr*	s_singleton;

	bool			m_displayGui;
	Flags*			m_flags;
};