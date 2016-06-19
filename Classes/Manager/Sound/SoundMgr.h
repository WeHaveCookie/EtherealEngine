#pragma once

#include "Manager/Manager.h"

class SoundMgr : public Manager
{

	public:
		SoundMgr();
		~SoundMgr();

		void Init();
		void Process(const float dt);
		void End();

	private:
};