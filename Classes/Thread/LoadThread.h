#pragma once
#include "Thread.h"

class LoadThread : public Thread
{
public:
	LoadThread();
	~LoadThread();

	void init();
	void process();

private:
};