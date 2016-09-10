#include "stdafx.h"
#include "LoadingMgr.h"
#include "Entity/Entity.h"
#include "Manager/Entity/EntityMgr.h"
#include "Thread/LoadingThread.h"

#define INVALID_SYNC_COUNTER_ID -1

LoadingMgr* LoadingMgr::s_singleton = NULL;
uint32_t	LoadingTask::newUID = 0;

LoadingMgr::LoadingMgr()
	:Manager(ManagerType::Loading)
{
	s_singleton = this;
}

LoadingMgr::~LoadingMgr()
{
	delete m_loadingTasks;
}

void LoadingMgr::init()
{
	m_processTime = sf::Time::Zero;
	m_loadingTasks = new LoadingTaskPool(100);
}

void LoadingMgr::process(const float dt)
{
	sf::Clock clock;
	m_loadingTasks->process();
	m_processTime = clock.getElapsedTime();
}

void LoadingMgr::end()
{
}

void LoadingMgr::showImGuiWindow(bool* window)
{
	if (ImGui::Begin("LoadingMgr", window))
	{

		ImGui::End();
	}
}


uint32_t LoadingMgr::loadAsync(Entity* ent, const char* path)
{
	if (ent != NULL)
	{
		m_loadingTasks->load(ent, path);
		return ent->getUID();
	}
	return -1;
}

bool LoadingMgr::isLoaded(uint32_t id)
{
	return m_loadingTasks->taskIsFinished(id);
}

bool LoadingMgr::getResult(uint32_t id, void* result)
{
	if (isLoaded(id))
	{
		Entity* ent = EntityMgr::getSingleton()->getEntity(id);
		if (ent != NULL)
		{
			result = ent;
			return true;
		}
	}
	return false;
}

LoadingTask::LoadingTask()
:m_uid(newUID++)
{
}

LoadingTask::~LoadingTask()
{

}

void LoadingTask::init(Entity* ent, const char* path)
{
	m_state.m_live.m_ent = ent;
	m_state.m_live.m_path = path;
	m_state.m_live.m_submit = false;
}

void LoadingTask::process()
{
	m_state.m_live.m_ent->build(m_state.m_live.m_path.c_str());
	m_state.m_live.m_counter--;
}

void LoadingTask::submit()
{
	m_state.m_live.m_submit = true;
	m_state.m_live.m_counter = 1;
	LoadingThread::getSingleton()->pushTask(m_state.m_live.m_counter, this);
}

void LoadingTask::wait()
{
	while (m_state.m_live.m_counter != 0)
	{
	}
	m_state.m_live.m_counter = INVALID_SYNC_COUNTER_ID;
}

bool LoadingTask::isFinished() const
{
	return (m_state.m_live.m_counter == 0 && m_state.m_live.m_submit == true || m_state.m_next != NULL);
}

LoadingTaskPool::LoadingTaskPool(int size)
	:m_poolSize(size)
{
	m_tasks.reserve(size);
	for (int i = 0; i < size; i++)
	{
		m_tasks.push_back(new LoadingTask());
	}

	m_firstAvailable = m_tasks[0];

	for (int i = 0; i < m_poolSize - 1; i++)
	{
		m_tasks[i]->setNext(m_tasks[i + 1]);
	}

	m_tasks[m_poolSize - 1]->setNext(NULL);
	m_usedEntity = 0;
}

LoadingTaskPool::~LoadingTaskPool()
{
	m_tasks.clear();
}

void LoadingTaskPool::load(Entity* ent, const char* path)
{
	if (m_firstAvailable == NULL)
	{
		std::cout << "Cannot create new loading task" << std::endl;
	}
	LoadingTask* newTask = m_firstAvailable;
	m_firstAvailable = newTask->getNext();

	newTask->init(ent, path);
	newTask->submit();

}

void LoadingTaskPool::process()
{
	for (auto& task : m_tasks)
	{
		if (task->isFinished())
		{
			task->setNext(m_firstAvailable);
			m_firstAvailable = task;
		}
	}
}

bool LoadingTaskPool::taskIsFinished(uint32_t id)
{
	for (auto& task : m_tasks)
	{
		if (task->getUID() == id && task->getNext() != NULL)
		{
			return true;
		}
	}
	return false;
}