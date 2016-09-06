#pragma once
#include <thread>

class Thread
{
public:

	Thread();
	~Thread();
	virtual void init() = 0;
	virtual void process();
	void start();
	void stop();
	bool isAlive() { return m_alive; }

private:
	bool			m_alive;
	std::thread*	m_thread;
};