#include "stdafx.h"
#include "SaveThread.h"
#include "blockingconcurrentqueue.h"
#include "Manager/Persistent/PersistentMgr.h"

struct SaveThread::Task
{
	int m_counter;
	SaveTask* m_saveTask;
	DeleteSaveTask* m_deleteSaveTask;
};

SaveThread* SaveThread::s_singleton = NULL;
class SaveThread::TaskQueue : public moodycamel::BlockingConcurrentQueue<Task> {};

SaveThread::SaveThread()
	:m_running(false)
{
	m_taskQueue = new TaskQueue();
	s_singleton = this;

	start();
}

SaveThread::~SaveThread()
{
	if (m_running)
		stop();

	delete m_taskQueue;
}

void SaveThread::start()
{
	if (m_running)
		return;

	m_running = true;

	m_thread = std::thread(threadFunc, this);
}

void SaveThread::stop()
{
	if (!m_running)
		return;

	m_running = false;
	Task task; // Dummy task for exit thread
	task.m_saveTask = NULL;
	task.m_deleteSaveTask = NULL;
	m_taskQueue->enqueue(task);
	m_thread.join();
}


void SaveThread::pushTask(int counter, SaveTask* saveTask)
{
	Task task;
	task.m_counter = counter;
	task.m_saveTask = saveTask;
	task.m_deleteSaveTask = NULL;

	m_taskQueue->enqueue(task);
}

void SaveThread::pushTask(int counter, DeleteSaveTask* deleteSaveTask)
{
	Task task;
	task.m_counter = counter;
	task.m_deleteSaveTask = deleteSaveTask;
	task.m_saveTask = NULL;

	m_taskQueue->enqueue(task);
}

SaveThread::Task SaveThread::pop()
{
	Task task;
	m_taskQueue->wait_dequeue(task);
	return task;
}

uint32_t SaveThread::threadFunc(void* arg)
{
	SaveThread* loadingThread = (SaveThread*)arg;

	while (loadingThread->m_running)
	{
		Task task = loadingThread->pop();

		// SAVE TASKS
		if (task.m_deleteSaveTask == NULL && task.m_saveTask == NULL)
		{
			continue;
		}
		if (task.m_deleteSaveTask == NULL)
		{
			task.m_saveTask->process();
		}
		else
		{
			task.m_deleteSaveTask->process();
		}
	}

	return 0;
}

