#pragma once

#include "Manager/Manager.h"

class GameMgr : public Manager
{
    public:
        GameMgr();
        ~GameMgr();

        // Function
		void Init();
		void Process(const float dt);
		void End();
		void Paint();


    protected:
    private:
        // Function
        
};
