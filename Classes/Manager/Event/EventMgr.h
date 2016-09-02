#pragma once
#include "Manager/Manager.h"

class EventMgr : public Manager
{
    public:
		EventMgr();
		~EventMgr();

		void init();
		void process(const float dt);
		void end();

	private:
};
