#pragma once

#include "Manager/Manager.h"

class InputMgr : public Manager
{

	public:
		InputMgr();
		~InputMgr();

		void init();
		void process(const float dt);
		void end();

	private:
};