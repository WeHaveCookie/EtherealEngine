#include "stdafx.h"
#include "Camera.h"
#include "Manager/Entity/EntityMgr.h"

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::update(const float dt)
{

}

void Camera::setTarget(uint32_t entityUID)
{
	if (EntityMgr::getSingleton()->isValidEntity(entityUID))
	{
		m_target = entityUID;
	}
}