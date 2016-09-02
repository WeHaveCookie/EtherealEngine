#pragma once

#include "Manager/Manager.h"

class TimeMgr : public Manager
{
	public:
		TimeMgr();
		~TimeMgr();

		void init();
		void process(const float dt);
		void end();

		const float GetDeltaTime() const;

	private:
};