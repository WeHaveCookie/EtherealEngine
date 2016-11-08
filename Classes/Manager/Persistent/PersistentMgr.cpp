#include "stdafx.h"
#include "PersistentMgr.h"
#include "Utils/wcharUtils.h"
#include "Utils/HashUtils.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/File/FileMgr.h"
#include "Thread/SaveThread.h"
#include <ShlObj.h>

#define INVALID_SYNC_COUNTER_ID -1
#define SAVE_DATA_DEFAULT_NAME	L"default"
#define MAX_PATH 260

PersistentMgr *PersistentMgr::singleton = NULL;

struct SaveTask::Op
{
	enum class Type
	{
		Write,
		Read
	};

	Type m_type;
	std::string m_name;
	uint32_t m_openMode;
	int m_saveSize;

	const void* m_writeBuffer;
	int m_sizeWriteBuffer;

	void* m_readBuffer;
	int m_sizeReadBuffer;
	SaveError::Enum m_error;

	Op();
	void process();
};

SaveTask::SaveTask()
{
	m_submit = false;
	m_maxSaveSize = 0;
	m_saveOpenMode = 0;
	m_counter = INVALID_SYNC_COUNTER_ID;
}

SaveTask::~SaveTask()
{
	clear();
}

void SaveTask::init(const char* saveName, SaveOpenMode::Enum saveMode, int maxSaveSize)
{
	assert(m_submit == false || isFinished()); // "Cannot reinit a task before it's finished"
	clear();
	m_saveName = saveName;
	m_saveOpenMode = saveMode;
	m_maxSaveSize = maxSaveSize;
	m_submit = false;
}

void SaveTask::init(SaveOpenMode::Enum openMode, int maxSaveSize)
{
	init("", openMode, maxSaveSize);
}

void SaveTask::clear()
{
	if (!m_tasks.empty())
	{
		for (auto it = m_tasks.begin(); it != m_tasks.end(); it++)
		{
			if (it->m_type == Op::Type::Read)
			{
				PersistentMgr::getSingleton()->releaseData(it->m_readBuffer);
			}
		}
		m_tasks.clear();
	}
	m_submit = false;
	m_counter = INVALID_SYNC_COUNTER_ID;
	m_maxSaveSize = 0;
	m_saveOpenMode = 0;
}

void SaveTask::addWriteTask(const char * name, const void* data, int size)
{
	if ((m_saveOpenMode == SaveOpenMode::Write) //  "You try to add write task in wrong save mode"
		&& (m_submit == false) // "You try to add write task while this task is already submitted"
		&& (m_saveOpenMode != 0)) // "Call init first"
	{
		Op op;
		op.m_name = name;
		op.m_writeBuffer = data;
		op.m_sizeWriteBuffer = size;
		op.m_type = Op::Type::Write;
		m_tasks.push_back(op);
	}
}

void SaveTask::addReadTask(const char* name)
{
	if ((m_submit == false) //  "You try to add read task while this task is already submitted"
		&& (m_saveOpenMode != 0)) // "Call init first"
	{
		Op op;
		op.m_name = name;
		op.m_type = Op::Type::Read;
		m_tasks.push_back(op);
	}
}

SaveError::Enum SaveTask::getResult(uint32_t index, void** buffer, int* sizeRead)
{
	assert(m_submit);
	wait();
	assert(index < m_tasks.size());
	Op op = m_tasks[index];
	if (op.m_type == Op::Type::Read)
	{
		*buffer = op.m_readBuffer;
		*sizeRead = op.m_sizeReadBuffer;
		return op.m_error;
	}
	else if (op.m_type == Op::Type::Write)
	{
		return op.m_error;
	}
	else
	{
		return SaveError::NoError;
	}
}

int SaveTask::getTasksSize() const
{
	return m_tasks.size();
}

void SaveTask::submit()
{
	if ((m_submit != true)) // "You try to submit a task already submitted"
	{
		if (m_tasks.size() == 0 && m_saveOpenMode != 0)
		{
			// No task to be done
			return;
		}
		m_submit = true;

		if (SaveThread::getSingleton())
		{
			m_counter = 1;
			SaveThread::getSingleton()->pushTask(m_counter, this);
		}
		else
		{
			process();
		}
	}
}

void SaveTask::process()
{
	PersistentMgr::getSingleton()->openSave(m_saveName.c_str(), (SaveOpenMode::Enum)m_saveOpenMode, m_maxSaveSize);

	for (auto it = m_tasks.begin(); it != m_tasks.end(); it++)
	{
		it->process();
	}

	PersistentMgr::getSingleton()->closeSave();

	m_counter--;
}

void SaveTask::wait()
{
	while (m_counter != 0)
	{
	}
	m_counter = INVALID_SYNC_COUNTER_ID;
}

bool SaveTask::isFinished() const
{
	return (m_counter == 0 && m_submit == true);
}

SaveTask::Op::Op()
{
	m_writeBuffer = NULL;
	m_readBuffer = NULL;
	m_sizeReadBuffer = 0;
}

void SaveTask::Op::process()
{
	switch (m_type)
	{
	case Type::Write:
		m_error = PersistentMgr::getSingleton()->writeData(m_name.c_str(), m_writeBuffer, m_sizeWriteBuffer);
		break;
	case Type::Read:
		m_error = PersistentMgr::getSingleton()->readData(m_name.c_str(), (void**)&m_readBuffer, &m_sizeReadBuffer);
		break;
	default:
		break;
	}
}

DeleteSaveTask::DeleteSaveTask()
{
	m_submit = false;
	m_counter = INVALID_SYNC_COUNTER_ID;
}

void DeleteSaveTask::init(const char* saveName)
{
	assert(m_submit == false || isFinished()); //  "Cannot reinit a task before it's finished"
	m_submit = false;
	m_counter = INVALID_SYNC_COUNTER_ID;
	m_saveName = saveName;
}

void DeleteSaveTask::submit()
{
	m_submit = true;

	if (SaveThread::getSingleton())
	{
		m_counter = 1;
		SaveThread::getSingleton()->pushTask(m_counter, this);
	}
	else
	{
		process();
	}
}

void DeleteSaveTask::wait()
{
	while(m_counter != 0){}
	m_counter = INVALID_SYNC_COUNTER_ID;
}

bool DeleteSaveTask::isFinished() const
{
	return (m_counter == 0 && m_submit == true);
}

void DeleteSaveTask::process()
{
	PersistentMgr::getSingleton()->deleteSave(m_saveName.c_str());
}


PersistentMgr::PersistentMgr()
	:Manager(ManagerType::Persistent)
{
	singleton = this;
	setSaveDirName(L"/EtherealEngine/");
	m_saveName = SAVE_DATA_DEFAULT_NAME;
	m_saveOpened = false;
}

PersistentMgr::~PersistentMgr()
{
}


static void AddTrailingSlash(std::wstring& str)
{
	str += L'/';
}

void PersistentMgr::init()
{

	if (GameMgr::getSingleton() != NULL)
		m_saveDataRootDir += AsciiToWideChar(std::string(GameMgr::getSingleton()->getGameName()));
	else
	{
		assert(false);
		m_saveDataRootDir += L"NoApplicationName";
	}
	AddTrailingSlash(m_saveDataRootDir);
}

void PersistentMgr::openSave(const char* saveName, SaveOpenMode::Enum saveMode, int maxSaveSize)
{
	if (saveName == NULL || saveName[0] == 0)
	{
		m_saveName = SAVE_DATA_DEFAULT_NAME;
	}
	else
	{
		m_saveName = AsciiToWideChar(std::string(saveName));
	}
	AddTrailingSlash(m_saveName);
	m_saveOpened = true;
}

void PersistentMgr::closeSave()
{
	m_saveOpened = false;
}

void PersistentMgr::setSaveDirName(const wchar_t* dirName)
{
	wchar_t userDocumentsPath[MAX_PATH];

	int ret = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, userDocumentsPath);
	if (ret != S_OK)
	{
		assert(false); // "SHGetSpecialFolderPathW failed : %i"
		m_saveDataRootDir = L"";
		return;
	}

	m_saveDataRootDir = userDocumentsPath;
	m_saveDataRootDir += dirName;
	AddTrailingSlash(m_saveDataRootDir);
}

void PersistentMgr::setSaveFullPath(const wchar_t* fullPath)
{
	m_saveDataRootDir = fullPath;
	AddTrailingSlash(m_saveDataRootDir);
}

SaveError::Enum PersistentMgr::readData(const char *name, void** data, int* sizeRead)
{
	if (!m_saveOpened)
	{
		assert(false); // "You try to call readData() whitout call openSave() before."
		return SaveError::Other;
	}

	std::wstring finalFullPath = m_saveDataRootDir + m_saveName + AsciiToWideChar(std::string(name));

	FILE* fp;
	if (_wfopen_s(&fp, finalFullPath.c_str(), L"rb") == 0)
	{
		uint32_t fileSize;

		// Get file size
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// Get the global hash
		uint64_t g_hash = 0;
		fileSize -= sizeof(g_hash);
		fread(&g_hash, 1, sizeof(g_hash), fp);


		*data = (void*) malloc((fileSize));

		fread(*data, 1, fileSize, fp);

		*sizeRead = fileSize;

		uint64_t hash = HashU64((uint8_t*)*data, fileSize);
		if (g_hash != hash)
		{
			return SaveError::CorruptedData;
		}

		fclose(fp);
	}
	else
	{
		return SaveError::NotFound;
	}
	return SaveError::NoError;
}

SaveError::Enum PersistentMgr::writeData(const char * name, const void* data, int size)
{

	if (!m_saveOpened)
	{
		assert(false); // "You try to call writeData() whitout call openSave(). You should not write ! \"Gandalf\""
		return SaveError::Other;
	}

	std::wstring finalPath = m_saveDataRootDir + m_saveName + AsciiToWideChar(std::string(name));

	// Create every folder if neeeded
	FileMgr::CreateDirectories(finalPath.c_str());

	FILE* fp;
	if (_wfopen_s(&fp, finalPath.c_str(), L"wb") == 0)
	{
		uint64_t hash = HashU64(static_cast<const uint8_t*>(data), size);	// Coherency hash
		fwrite(&hash, sizeof(hash), 1, fp);
		fwrite(data, size, 1, fp);
		int ret = ferror(fp);
		fclose(fp);
		if (ret == ENOSPC)
		{
			return SaveError::NotEnoughSpace;
		}
		else if (ret != 0)
		{
			return SaveError::Other;
		}
	}
	else
	{
		return SaveError::Other;
	}
	return SaveError::NoError;
}

void PersistentMgr::deleteSave(const char* saveName)
{
	//wchar_t saveDataPath[MAX_PATH + 1]; // +1 is for the double null terminate
	SHFILEOPSTRUCTW fos = { 0 };

	std::wstring finalPath = m_saveDataRootDir;

	if (saveName == NULL || saveName[0] == 0)
	{
		finalPath += SAVE_DATA_DEFAULT_NAME;
	}
	else
	{
		finalPath += AsciiToWideChar(std::string(saveName));
	}

	AddTrailingSlash(finalPath);

	wchar_t* SHFilePath = (wchar_t*)_alloca(sizeof(wchar_t) * (finalPath.size() + 2)); // +1 is for the double null terminate

	wcscpy(SHFilePath, finalPath.c_str());

	SHFilePath[finalPath.size() + 1] = 0; // Double terminate for SHFileOperation

	fos.wFunc = FO_DELETE; // Delete the files specified in pFrom
	fos.pFrom = SHFilePath;
	fos.fFlags = FOF_NO_UI; // Used for Vista - Perform the operation silently, presenting no UI to the user.
	int ret = SHFileOperation(&fos);
	if (ret == 0x7C)
	{
		// The path in pFrom was invalid. So you don't need to assert or do anything because folder doesn't exist.
	}
	else
	{
		assert(ret == 0); // "SHFileOperation failed : %i", ret);
	}
}

void PersistentMgr::releaseData(void* data)
{
	free(data);
}