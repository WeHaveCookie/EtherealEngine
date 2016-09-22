#pragma once

#include "Manager/Manager.h"

class RenderMgr : public Manager
{
	public:

		static RenderMgr* getSingleton() { return s_singleton; }

		RenderMgr();
		~RenderMgr();

		void init();
		void process(const float dt);
		void end();
		void startFrame();
		void endFrame();

		sf::RenderWindow* getMainRenderWindow() const { return m_mainRenderWindow; }

	private:
		static RenderMgr*	s_singleton;
		sf::Clock			m_timeFrame;
		sf::RenderWindow*	m_mainRenderWindow;
};