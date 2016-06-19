#pragma once

#include "Manager/Manager.h"

class InputMgr : public Manager
{

	public:
		InputMgr();
		~InputMgr();

		void Init();
		void Process(const float dt);
		void End();

	private:
};