#pragma once
#include "Manager/Manager.h"

class Entity;

class LoadingTask
{
public:
	LoadingTask();
	~LoadingTask();
	void init(Entity* ent, const char* path);
	void process();
	void submit();
	void wait();
	bool isFinished() const;
	const uint32_t getUID() const { return m_uid; }

	void setNext(LoadingTask* task) { m_state.m_next = task; }
	LoadingTask* getNext() { return m_state.m_next; }
protected:
	static uint32_t		newUID;
	const uint32_t		m_uid;
private:
	union State{
		struct
		{
			Entity* m_ent;
			std::string m_path;
			int m_counter;
			bool m_submit;
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
	void process();
	bool taskIsFinished(uint32_t id);

private:
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
	uint32_t loadAsync(Entity* ent, const char* path);
	bool getResult(uint32_t id, void* result);

	sf::Time getProcessTime() { return m_processTime; }

protected:

private:
	static LoadingMgr*	s_singleton;
	LoadingTaskPool*	m_loadingTasks;
	sf::Time			m_processTime;
};
