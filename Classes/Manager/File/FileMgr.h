#pragma once
#include "Manager/Manager.h"

class FileMgr : public Manager
{
public:
	static FileMgr* getSingleton() { return s_singleton; }

	FileMgr();
	virtual ~FileMgr();
	void init() {}

	static void CreateDirectories(const char* path);
	static void CreateDirectories(const wchar_t* path);
	static void GetFilesInDirectory(std::vector<std::wstring> &out, const std::wstring &directory, const std::wstring extention = L"");
	static void ReadFile(const char* path, void** buffer, int* bufferSize);
	static void CloseFile(void* buffer);

private:
	static FileMgr* s_singleton;
};


