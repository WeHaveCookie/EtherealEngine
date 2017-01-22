#include "stdafx.h"
#include "CameraMgr.h"
#include "Camera/Camera.h"

CameraMgr* CameraMgr::s_singleton = NULL;

CameraMgr::CameraMgr()
	:Manager(ManagerType::Enum::Camera)
{
	s_singleton = this;
}


CameraMgr::~CameraMgr()
{
}

void CameraMgr::init()
{
	m_camera = new Camera();
}

void CameraMgr::process()
{

}

void CameraMgr::end()
{

}