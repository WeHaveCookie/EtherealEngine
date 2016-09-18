#pragma once
#include <iterator>

static std::wstring AsciiToWideChar(const std::string& asciiStr)
{
	std::wstring wStr;
	std::copy(asciiStr.begin(), asciiStr.end(), std::back_inserter(wStr));
	return wStr;
}


static std::string WideChartoAscii(const std::wstring& wStr)
{
	std::string asciiStr;
	std::copy(wStr.begin(), wStr.end(), std::back_inserter(asciiStr));
	return asciiStr;
}