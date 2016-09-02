#pragma once

#include "Manager/Manager.h"

class RenderMgr : public Manager
{
	public:
		RenderMgr();
		~RenderMgr();

		void init();
		void process(const float dt);
		void end();
		void startFrame();
		void endFrame();

		sf::RenderWindow* getMainRenderWindow() const { return m_mainRenderWindow; }

	private:
		sf::RenderWindow* m_mainRenderWindow;
};