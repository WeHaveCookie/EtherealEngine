#pragma once
#include "Manager/Manager.h"

class Entity;

class LoadingTask
{
public:
	LoadingTask();
	~LoadingTask();
	void init(Entity* ent, const char* path, bool persistent = false);
	void process();
	void submit();
	void wait();
	bool isFinished() const;
	const uint32_t getUID() const { return m_uid; }

	void setNext(LoadingTask* task) { m_used = false; m_state.m_next = task; }
	LoadingTask* getNext() { return m_state.m_next; }
	const char* getName() const { return m_state.m_live.m_path.c_str(); }
	const bool isUsed() const { return m_used; }
	void showInfo() { m_displayInfo = !m_displayInfo; }
	void closeInfo() { m_displayInfo = false; }
	void displayInfo();
	const bool isPersistent() const { return m_persistent; }

protected:
	static uint32_t		newUID;
	const uint32_t		m_uid;
private:
	bool m_persistent;
	bool m_used;
	bool m_displayInfo;
	union State{
		struct
		{
			Entity* m_ent;
			std::string m_path;
			int m_counter;
			bool m_submit;
			sf::Clock m_processClock;
			sf::Time m_processTime;
		} m_live;
		
		LoadingTask* m_next;

		State()
		:m_live()
		{
		}

		~State()
		{
		}

	} m_state;
	
};

class LoadingTaskPool
{
public:
	LoadingTaskPool(int size);
	~LoadingTaskPool();

	void load(Entity* ent, const char* path);
	void loadAsync(Entity* ent, const char* path);
	void process();
	const bool taskIsFinished(uint32_t id) const;
	std::vector<LoadingTask*> getUsedTask();
	void release(uint32_t id);
	const bool isTaskPersistent() const { return m_taskPersistent; }
	void setTaskPersistent(bool b) { m_taskPersistent = b; }

private:
	void release(LoadingTask* task);
	bool						m_taskPersistent;
	const int					m_poolSize;
	int							m_usedEntity;
	std::vector<LoadingTask*>	m_tasks;
	LoadingTask*				m_firstAvailable;
};


class LoadingMgr : public Manager
{
public:

	static LoadingMgr* getSingleton() { return s_singleton; }

	LoadingMgr();
	~LoadingMgr();
	void init();
	void process(const float dt);
	void end();
	void showImGuiWindow(bool* window);
	bool isLoaded(uint32_t id);

	void load(Entity* ent, const char* path);
	uint32_t loadAsync(Entity* ent, const char* path);
	bool getResult(uint32_t id, void* result);

	sf::Time getProcessTime() { return m_processTime; }

protected:

private:
	static LoadingMgr*	s_singleton;
	LoadingTaskPool*	m_loadingTasks;
	sf::Time			m_processTime;
};
