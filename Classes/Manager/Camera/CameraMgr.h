#pragma once
#include "Manager/Manager.h"

class Camera;

class CameraMgr : public Manager
{
	public:

		static CameraMgr* getSingleton() { return s_singleton; }

		CameraMgr();
		~CameraMgr();

		void init();
		void process();
		void end();

	private:

		static CameraMgr*	s_singleton;
		Camera*				m_camera;


};

