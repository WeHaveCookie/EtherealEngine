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
		if (ImGui::Button("Close All"))
		{
			for (auto& task : m_loadingTasks->getUsedTask())
			{
				task->closeInfo();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete All"))
		{
			for (auto& task : m_loadingTasks->getUsedTask())
			{
				m_loadingTasks->release(task->getUID());
			}
		}
		ImGui::SameLine();
		bool pers = m_loadingTasks->isTaskPersistent();
		ImGui::Checkbox("Persistent Task", &pers);
		m_loadingTasks->setTaskPersistent(pers);
		for (auto& task : m_loadingTasks->getUsedTask())
		{
			ImGui::Text("%i : %s", task->getUID(), task->getName());
			if (ImGui::IsItemClicked())
			{
				task->showInfo();
			}
			ImGui::SameLine();
			std::string label = "Delete###D" + std::to_string(task->getUID());
			if (ImGui::Button(label.c_str()))
			{
				m_loadingTasks->release(task->getUID());
			}
		}
	}
	ImGui::End();
}


void LoadingMgr::load(Entity* ent, const char* path)
{
	if (ent != NULL)
	{
		m_loadingTasks->load(ent, path);
	}
}

uint32_t LoadingMgr::loadAsync(Entity* ent, const char* path)
{
	if (ent != NULL)
	{
		m_loadingTasks->loadAsync(ent, path);
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
	m_persistent = false;
	m_displayInfo = false;
}

LoadingTask::~LoadingTask()
{

}

void LoadingTask::init(Entity* ent, const char* path, bool persistent)
{
	m_state.m_live.m_ent = ent;
	m_state.m_live.m_path = path;
	m_state.m_live.m_submit = false;
	m_state.m_live.m_processClock.restart();
	m_persistent = persistent;
	m_used = true;
}

void LoadingTask::process()
{
	m_state.m_live.m_ent->build(m_state.m_live.m_path.c_str());
	m_state.m_live.m_counter--;
	m_state.m_live.m_processTime = m_state.m_live.m_processClock.getElapsedTime();
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
		volatile int i = 0; // Use to prevent the compiler optimization
	}
	m_state.m_live.m_counter = INVALID_SYNC_COUNTER_ID;
}

bool LoadingTask::isFinished() const
{
	return (m_state.m_live.m_counter == 0 && m_state.m_live.m_submit == true || m_state.m_next != NULL );
}

void LoadingTask::displayInfo()
{
	if (m_used && m_displayInfo)
	{
		std::string label = std::to_string(m_uid) + " : " + m_state.m_live.m_path;
		if (ImGui::Begin(label.c_str(), &m_displayInfo, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Entity : %i", m_state.m_live.m_ent->getUID());
			ImGui::Text("Loaded in : %f ms", m_state.m_live.m_processTime.asMicroseconds() / 1000.0f);
		}
		ImGui::End();
	}
}

LoadingTaskPool::LoadingTaskPool(int size)
	:m_poolSize(size)
{
	m_taskPersistent = false;
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
	for (auto& task : m_tasks)
	{
		delete task;
	}
	m_tasks.clear();
}

void LoadingTaskPool::load(Entity* ent, const char* path)
{
	if (m_firstAvailable == NULL)
	{
		return;
	}
	LoadingTask* newTask = m_firstAvailable;
	m_firstAvailable = newTask->getNext();

	newTask->init(ent, path, m_taskPersistent);
	newTask->submit();
	newTask->wait();
}

void LoadingTaskPool::loadAsync(Entity* ent, const char* path)
{
	if (m_firstAvailable == NULL)
	{
		return;
	}
	LoadingTask* newTask = m_firstAvailable;
	m_firstAvailable = newTask->getNext();

	newTask->init(ent, path, m_taskPersistent);
	newTask->submit();

}

void LoadingTaskPool::process()
{
	for (auto& task : m_tasks)
	{
		if (task->isFinished() && !task->isPersistent())
		{
			release(task);
		}
		if (task->isUsed())
		{
			task->displayInfo();
		}
	}
}

const bool LoadingTaskPool::taskIsFinished(uint32_t id) const
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

std::vector<LoadingTask*> LoadingTaskPool::getUsedTask()
{
	std::vector<LoadingTask*> res;
	for (auto& task : m_tasks)
	{
		if (task->isUsed())
		{
			res.push_back(task);
		}
	}
	return res;
}

void LoadingTaskPool::release(uint32_t id)
{
	for (auto& task : m_tasks)
	{
		if (task->getUID() == id)
		{
			release(task);
		}
	}
}

void LoadingTaskPool::release(LoadingTask* task)
{
	task->setNext(m_firstAvailable);
	m_firstAvailable = task;
}