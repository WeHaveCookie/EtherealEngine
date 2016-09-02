#pragma once
#include "Manager/Manager.h"

class ItemMgr : public Manager
{
    public:
        ItemMgr();
        virtual ~ItemMgr();
		
		void init();
		void process(const float dt);
		void end();

    protected:
    private:
};

