#pragma once
#include "Manager/Manager.h"

class LevelMgr : public Manager
{
    public:
        LevelMgr();
        virtual ~LevelMgr();

		void Init();
		void Process(const float dt);
		void End();
		void Paint();


    protected:
    private:
        // Function

};

