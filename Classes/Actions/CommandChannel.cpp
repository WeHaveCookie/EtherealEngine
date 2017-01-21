#include "stdafx.h"
#include "CommandChannel.h"
#include "Manager/Sound/SoundMgr.h"

bool CommandChannelUpgrade::execute()
{
	SoundMgr::getSingleton()->addLayer();
	return true;
}

void* CommandChannelUpgrade::makeCopy()
{
	return (void*)new CommandChannelUpgrade();
}

bool CommandChannelDowngrade::execute()
{
	SoundMgr::getSingleton()->removeLayer();
	return true;
}

void* CommandChannelDowngrade::makeCopy()
{
	return (void*)new CommandChannelDowngrade();
}
