#pragma once
#include "Manager/Manager.h"

class ItemMgr : public Manager
{
    public:
        ItemMgr();
        virtual ~ItemMgr();
		
		void Init();
		void Process(const float dt);
		void End();

    protected:
    private:
};

