#include "stdafx.h"
#include "CommandRotate.h"
#include "Entity/Entity.h"
#include "Manager/Input/InputMgr.h"


void CommandRotate::init(Entity* ent, void* data)
{
	Command::init(ent);
	Command::initDone();
}

bool CommandRotate::execute()
{
	Entity* entity = getEntity();
	if (entity == NULL || !isInit())
	{
		return true;
	}
	auto inputMgr = InputMgr::getSingleton();
	std::vector<uint32_t> pads = inputMgr->getActivePads();
	auto padX = inputMgr->getPadKeyValue(KeyType::padX,pads[0]);
	auto padY = inputMgr->getPadKeyValue(KeyType::padY,pads[0]);
	float angle = atan2(padY, padX) + 90*DEGTORAD;//Sale
	entity->setAngle(angle);
	return true;
}

void CommandRotate::undo()
{

}

void* CommandRotate::makeCopy()
{
	return (void*)new CommandRotate();
}
