#pragma once

#include "Manager/Manager.h"

class SoundMgr : public Manager
{

	public:
		SoundMgr();
		~SoundMgr();

		void init();
		void process(const float dt);
		void end();

	private:
};