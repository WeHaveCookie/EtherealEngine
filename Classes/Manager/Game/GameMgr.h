#pragma once

#include "Manager/Manager.h"

class GameMgr : public Manager
{
    public:
        GameMgr();
        ~GameMgr();

        // Function
		void init();
		void process(const float dt);
		void end();
		void paint();
		bool isRunning();
		void setFrameRate(unsigned int frameRate);
		sf::RenderWindow* getMainRenderWindow() const { return m_mainRenderWindow; }

    protected:
    private:
        // Function
		sf::RenderWindow* m_mainRenderWindow;
};
