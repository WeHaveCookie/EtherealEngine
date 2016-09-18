#pragma once
#include "Manager/Manager.h"

class FileMgr : public Manager
{
public:
	static FileMgr* getSingleton() { return s_singleton; }

	FileMgr();
	virtual ~FileMgr();
	void init() {}

// 	File* createFile(const char* fileName);
// 	File* createFile(const char* filePath, const char* resourceRootPath);

	static void CreateDirectories(const char* path);
	static void CreateDirectories(const wchar_t* path);
	static void GetFilesInDirectory(std::vector<std::wstring> &out, const std::wstring &directory, const std::wstring extention = L"");


private:
	static FileMgr* s_singleton;
};


