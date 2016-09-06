#pragma once
#include "Manager/Manager.h"

namespace SaveError
{
	enum Enum
	{
		NoError,
		NotFound,
		CorruptedData,
		BadUser,
		NotEnoughSpace,
		Other
	};
}

namespace SaveOpenMode
{
	enum Enum
	{
		Read = 1,
		Write = 2,
		ReadWrite = Write
	};
}

class SaveTask
{
public:
	SaveTask();
	~SaveTask();
	void init(const char* saveName, SaveOpenMode::Enum openMode, int maxSaveSize = 32 * 1024);
	// Open the default save
	void init(SaveOpenMode::Enum openMode, int maxSaveSize = 32 * 1024);

	void addWriteTask(const char* name, const void* data, int size);
	void addReadTask(const char* name);

	void submit();
	void wait();
	bool isFinished() const;
	SaveError::Enum getResult(uint32_t index, void** data = NULL, int* sizeRead = NULL);
	int getTasksSize() const;

	SaveTask(const SaveTask&) = delete;
	SaveTask& operator=(const SaveTask&) = delete;
private:
	friend class SaveThread;
	void clear();
	void process();
	struct Op;
	std::vector<Op> m_tasks;
	std::string m_saveName;
	uint32_t m_saveOpenMode;
	int m_counter;
	bool m_submit;
	int m_maxSaveSize;
};

class DeleteSaveTask
{
public:
	DeleteSaveTask();
	void init(const char* saveName = "");

	void submit();

	void wait();
	bool isFinished() const;

	DeleteSaveTask(const DeleteSaveTask&) = delete;
	DeleteSaveTask& operator=(const DeleteSaveTask&) = delete;
private:
	friend class SaveThread;
	void process();
	std::string m_saveName;
	int m_counter;
	bool m_submit;
};

class PersistentMgr : public Manager
{
public:

	PersistentMgr();
	virtual ~PersistentMgr();
	static PersistentMgr *getSingleton() { return singleton; }

	void init();

	void deleteSave(const char* saveName);
	void openSave(const char* saveName, SaveOpenMode::Enum saveMode, int maxSaveSize = 32 * 1024);
	void closeSave();

	SaveError::Enum readData(const char* name, void** data, int* sizeRead);
	SaveError::Enum writeData(const char* name, const void* data, int size);

	void releaseData(void* data);

	void setSaveDirName(const wchar_t* dirName);
	void setSaveFullPath(const wchar_t* fullPath);

private:
	static PersistentMgr *singleton;

	std::wstring m_saveDataRootDir;
	std::wstring m_saveName;
	bool m_saveOpened;
};
