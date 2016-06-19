#pragma once

#include "Manager/Manager.h"

class RenderMgr : public Manager
{
	public:
		RenderMgr();
		~RenderMgr();

		void Init();
		void Process(const float dt);
		void End();
		void StartFrame();
		void EndFrame();
};