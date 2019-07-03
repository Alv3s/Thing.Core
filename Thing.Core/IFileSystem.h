#pragma once

#include <string>
#include <vector>

namespace Thing 
{
	namespace Core
	{
		class IFileSystem
		{
		public:
			virtual bool Read(std::string file, std::vector<uint8_t>* bytes) = 0;
			virtual bool Read(std::string file, std::string* bytes) = 0;

			virtual bool Write(std::string file, std::string& bytes) = 0;
			virtual bool Write(std::string file, std::vector<uint8_t>& bytes) = 0;

			virtual void Delete(std::string file) = 0;
		};
	}
}

