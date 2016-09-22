#include "stdafx.h"
#include "Thread.h"


Thread::Thread()
{
	m_alive = false;
}

Thread::~Thread()
{
	delete m_thread;
}

void Thread::process()
{

}

void Thread::start()
{
	m_alive = true;
	m_thread = new std::thread(&Thread::process, this);
}

void Thread::stop()
{
	m_alive = false;
	free(m_thread);
}