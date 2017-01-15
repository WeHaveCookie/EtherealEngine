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

void FileMgr::GetFilesInDirectory(std::vector<std::wstring> &out, const std::wstring &directory, const std::wstring extention)
{
	HANDLE dir;
	WIN32_FIND_DATA file_data;

	if ((dir = FindFirstFile((directory + L"/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return; /* No files found */

	do {
		const std::wstring file_name = file_data.cFileName;
		const std::wstring full_file_name = directory + L"/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		if (extention != L"")
		{
			std::string::size_type found = full_file_name.find(extention);
			if (found != std::string::npos)
			{
				out.push_back(full_file_name);
			}
		}
		else
		{
			out.push_back(full_file_name);
		}

	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
}

void FileMgr::ReadFile(const char* path, void** buffer, int* bufferSize)
{
	FILE *f = fopen(path, "rb");
	assert(f != NULL);
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	*bufferSize = fsize;
	fseek(f, 0, SEEK_SET);

	*buffer = (void*)malloc(fsize + 1);
	int res = fread(*buffer, fsize, 1, f);
	assert(res == 1);
	fclose(f);
}

void FileMgr::CloseFile(void* buffer)
{
	free(buffer);
}