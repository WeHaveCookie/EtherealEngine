#pragma once

#include "Manager/Manager.h"

class GameMgr : public Manager
{
    public:
		static GameMgr* getSingleton() { return s_singleton; }
        GameMgr();
        ~GameMgr();

        // Function
		void init();
		void process(const float dt);
		void end();
		void paint();
		void showImGuiWindow(bool* window);

		bool isRunning();
		void setFrameRate(uint32_t frameRate);
		sf::RenderWindow* getMainRenderWindow() const { return m_mainRenderWindow; }
		sf::Time getProcessTime() { return m_processTime; }
		const char* getGameName() { return m_gameName; }

    protected:
    private:
        // Function
		static GameMgr*		s_singleton;
		sf::RenderWindow*	m_mainRenderWindow;
		sf::Time			m_processTime;
		int					m_framerate;
		char				m_gameName[128];
};
