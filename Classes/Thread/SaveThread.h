#pragma once
#include <thread>

class SaveTask;
class DeleteSaveTask;

class SaveThread
{
public:

	SaveThread();
	~SaveThread();

	static SaveThread* getSingleton() { return s_singleton; }

	/// Starts the loading thread (automatically called in constructor).
	void start();
	/// Stops the loading thread (automatically called in destructor).
	void stop();

	void pushTask(
		int counter,			/// The counter to decrement when the task is finished
		SaveTask* saveTask				/// SaveTask for read / write operation
	);

	void pushTask(
		int counter,			/// The counter to decrement when the task is finished
		DeleteSaveTask* deleteSaveTask  /// DeleteTask for delete operation
	);

private:

	struct Task;

	Task pop();

	static uint32_t threadFunc(void* arg);

	class TaskQueue;
	TaskQueue*		 m_taskQueue;
	std::thread		 m_thread;
	
	bool			 m_running;

	static SaveThread* s_singleton;
};