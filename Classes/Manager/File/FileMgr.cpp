#include "stdafx.h"
#include "FileMgr.h"
#include <Windows.h>

FileMgr *FileMgr::s_singleton = NULL;

FileMgr::FileMgr()
	:Manager(ManagerType::File)
{
	s_singleton = this;
}

FileMgr::~FileMgr()
{

}

void FileMgr::CreateDirectories(const char* path)
{
	char dir[MAX_PATH];
	for (int n = 0; path[n]; n++)
	{
		const char c = path[n];
		if (c == '/')
		{
			dir[n] = 0;
			CreateDirectoryA(dir, NULL);
		}
		dir[n] = c;
	}
}

void FileMgr::CreateDirectories(const wchar_t* path)
{
	wchar_t dir[MAX_PATH];
	for (int n = 0; path[n]; n++)
	{
		const wchar_t c = path[n];
		if (c == L'/')
		{
			dir[n] = 0;
			CreateDirectoryW(dir, NULL);
		}
		dir[n] = c;
	}
}


/*
File* FileMgr::createFile(const char* fileName)
{
	return new WinFile(fileName);
}

File* FileMgr::createFile(const char* filePath, const char* resourceRootPath)
{
	std::string finalPath = convertResourcePath(resourceRootPath, filePath);
	return new WinFile(finalPath.c_str(), false);
}*/