#include "stdafx.h"
#include "CommandInput.h"
#include "Manager/Input/InputMgr.h"

bool CommandLockInput::execute()
{
	InputMgr::getSingleton()->lockInput(true);
	return true;
}


bool CommandUnlockInput::execute()
{
	InputMgr::getSingleton()->lockInput(false);
	return true;
}