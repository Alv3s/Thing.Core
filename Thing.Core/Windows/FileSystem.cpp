#ifdef _WIN32
#include "FileSystem.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <Windows.h>

std::string ExePath()
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
			bool FileSystem::Read(std::string file, std::vector<uint8_t>* bytes)
			{
				file = ExePath() + file;
				std::ifstream openfile(file, std::fstream::in | std::fstream::out | std::fstream::app);
				if (!openfile.is_open())
					return false;

				openfile.seekg(0, std::ios::end);
				bytes->reserve(openfile.tellg());
				openfile.seekg(0, std::ios::beg);

				bytes->assign((std::istreambuf_iterator<char>(openfile)), std::istreambuf_iterator<char>());
				return true;
			}

			bool FileSystem::Read(std::string file, std::string* bytes)
			{
				file = ExePath() + file;
				std::ifstream openfile(file, std::fstream::in | std::fstream::out | std::fstream::app);
				if (!openfile.is_open())
					return false;

				openfile.seekg(0, std::ios::end);
				bytes->reserve(openfile.tellg());
				openfile.seekg(0, std::ios::beg);

				bytes->assign((std::istreambuf_iterator<char>(openfile)), std::istreambuf_iterator<char>());
				return true;
			}

			bool FileSystem::Write(std::string file, std::string& bytes)
			{
				std::ofstream openFile;
				openFile.open(file, std::fstream::in | std::fstream::out | std::fstream::app);
				if (!openFile.is_open())
					return false;

				openFile << bytes;
				openFile.close();
				return true;
			}

			bool FileSystem::Write(std::string file, std::vector<uint8_t>& bytes)
			{
				std::ofstream openFile;
				openFile.open(file, std::fstream::in | std::fstream::out | std::fstream::app);
				if (!openFile.is_open())
					return false;

				openFile.write((char*)&bytes[0], bytes.size());
				openFile.close();
				return true;
			}

			void FileSystem::Delete(std::string file)
			{
				remove(file.c_str());
			}
		}
	}
}
#endif