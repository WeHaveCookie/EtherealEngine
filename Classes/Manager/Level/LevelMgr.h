#pragma once
#include "Manager/Manager.h"

class LevelMgr : public Manager
{
    public:
        LevelMgr();
        virtual ~LevelMgr();

		void init();
		void process(const float dt);
		void end();
		void paint();


    protected:
    private:
        // Function

};

