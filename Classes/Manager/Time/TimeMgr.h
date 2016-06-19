#pragma once

#include "Manager/Manager.h"

class TimeMgr : public Manager
{
	public:
		TimeMgr();
		~TimeMgr();

		void Init();
		void Process(const float dt);
		void End();

		const float GetDeltaTime() const;

	private:
};