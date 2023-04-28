#pragma once

#include <string>

class FileLoader
{
public:
	static std::string GetFileContents(const char* filename);
};

