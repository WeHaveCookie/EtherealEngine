#pragma once
#include <thread>

class LoadingTask;

class LoadingThread
{
public:

	static LoadingThread* getSingleton() { return s_singleton; }

	LoadingThread();
	~LoadingThread();

	void start();
	void stop();

	void pushTask(int counter, LoadingTask* loadingTask);

private:

	struct Task;

	Task pop();
	static uint32_t threadFunc(void* arg);

	class TaskQueue;
	TaskQueue*		 m_taskQueue;
	std::thread		 m_thread;

	bool			 m_running;

	static LoadingThread* s_singleton;
};
