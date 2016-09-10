#include "stdafx.h"
#include "LoadingThread.h"

#include "stdafx.h"
#include "SaveThread.h"
#include "blockingconcurrentqueue.h"
#include "Manager/Persistent/PersistentMgr.h"
#include "Entity/Entity.h"
#include "Manager/Loading/LoadingMgr.h"


struct LoadingThread::Task
{
	int m_counter;
	LoadingTask* m_loadingTask;
};

LoadingThread* LoadingThread::s_singleton = NULL;
class LoadingThread::TaskQueue : public moodycamel::BlockingConcurrentQueue<Task> {};

LoadingThread::LoadingThread()
	:m_running(false)
{
	m_taskQueue = new TaskQueue();
	s_singleton = this;

	start();
}

LoadingThread::~LoadingThread()
{
	if (m_running)
		stop();

	delete m_taskQueue;
}

void LoadingThread::start()
{
	if (m_running)
		return;

	m_running = true;

	m_thread = std::thread(threadFunc, this);
}

void LoadingThread::stop()
{
	if (!m_running)
		return;

	m_running = false;
	Task task; // Dummy task for exit thread
	task.m_loadingTask = NULL;
	m_taskQueue->enqueue(task);
	m_thread.join();
}


void LoadingThread::pushTask(int counter, LoadingTask* loadingTask)
{
	Task task;
	task.m_counter = counter;
	task.m_loadingTask = loadingTask;

	m_taskQueue->enqueue(task);
}

LoadingThread::Task LoadingThread::pop()
{
	Task task;
	m_taskQueue->wait_dequeue(task);
	return task;
}

uint32_t LoadingThread::threadFunc(void* arg)
{
	LoadingThread* loadingThread = (LoadingThread*)arg;

	while (loadingThread->m_running)
	{
		Task task = loadingThread->pop();

		// SAVE TASKS
		if (task.m_loadingTask != NULL)
		{
			task.m_loadingTask->process();
		}
	}

	return 0;
}