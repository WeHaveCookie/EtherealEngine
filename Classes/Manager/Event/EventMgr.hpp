#pragma once
#include "Manager/Manager.h"

class EventMgr : public Manager
{
    public:
		EventMgr();
		~EventMgr();

		void Init();
		void Process(const float dt);
		void End();

	private:
};
