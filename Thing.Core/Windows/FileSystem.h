#pragma once

#include "../IFileSystem.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <Windows.h>

static std::string ExePath()
{
	using namespace std;

	char buffer[MAX_PATH];

	GetModuleFileName(NULL, buffer, MAX_PATH);

	string::size_type pos = string(buffer).find_last_of("\\/");

	if (pos == string::npos)
		return "";
	else
		return string(buffer).substr(0, pos);
}

namespace Thing
{
	namespace Core
	{
		namespace Windows
		{
			class FileSystem : public virtual Thing::Core::IFileSystem
			{
			public:
				bool Read(std::string file, std::vector<uint8_t>* bytes) override
				{
					file = ExePath() + file;
					std::ifstream openfile(file, std::fstream::in | std::fstream::out);
					if (!openfile.is_open())
						return false;

					openfile.seekg(0, std::ios::end);
					bytes->reserve(openfile.tellg());
					openfile.seekg(0, std::ios::beg);

					bytes->assign((std::istreambuf_iterator<char>(openfile)), std::istreambuf_iterator<char>());
					return true;
				}

				bool Read(std::string file, std::string* bytes) override
				{
					file = ExePath() + file;
					std::ifstream openfile(file, std::fstream::in | std::fstream::out);
					if (!openfile.is_open())
						return false;

					openfile.seekg(0, std::ios::end);
					bytes->reserve(openfile.tellg());
					openfile.seekg(0, std::ios::beg);

					bytes->assign((std::istreambuf_iterator<char>(openfile)), std::istreambuf_iterator<char>());
					return true;
				}

				bool Write(std::string file, std::string& bytes) override
				{
					file = ExePath() + file;

					std::ofstream openFile;
					openFile.open(file, std::fstream::in | std::fstream::out);
					if (!openFile.is_open())
						return false;

					openFile.write(bytes.c_str(), bytes.size());
					openFile.close();
					return true;
				}

				bool Write(std::string file, std::vector<uint8_t>& bytes) override
				{
					file = ExePath() + file;

					std::ofstream openFile;
					openFile.open(file, std::fstream::in | std::fstream::out);
					if (!openFile.is_open())
						return false;

					openFile.write((char*)& bytes[0], bytes.size());
					openFile.close();
					return true;
				}

				void Delete(std::string file) override
				{
					remove(file.c_str());
				}
			};
		}
	}
}