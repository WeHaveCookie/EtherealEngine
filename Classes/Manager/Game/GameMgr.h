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
		bool isRunning();
		void setFrameRate(uint32_t frameRate);
		sf::RenderWindow* getMainRenderWindow() const { return m_mainRenderWindow; }

		const char* getGameName() { return m_gameName; }

    protected:
    private:
        // Function
		static GameMgr* s_singleton;
		sf::RenderWindow* m_mainRenderWindow;
		char m_gameName[128];
};
