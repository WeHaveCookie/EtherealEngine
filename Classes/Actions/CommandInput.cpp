#include "stdafx.h"
#include "CommandInput.h"
#include "Manager/Input/InputMgr.h"

void CommandLockInput::execute()
{
	InputMgr::getSingleton()->lockInput(true);
}


void CommandUnlockInput::execute()
{
	InputMgr::getSingleton()->lockInput(false);
}