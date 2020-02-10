#pragma once

#include "../IFileSystem.h"
#include <stdint.h>
#include <FS.h>
#include <LittleFS.h>

namespace Thing
{
	namespace Core
	{
		namespace ESP
		{
			class LittleFileSystem : public virtual Thing::Core::IFileSystem
			{
			public:
				LittleFileSystem()
				{
					LittleFS.begin();
				}

				bool Read(std::string file, std::vector<uint8_t>* bytes) override
				{
					bool ret = LittleFS.exists(file.c_str());
					if (!ret)
						return false;

					fs::File openFile = LittleFS.open(file.c_str(), "r");
					size_t size = openFile.size();
					std::vector<char> buffer(size, 0);
					openFile.readBytes(&buffer[0], size);
					openFile.close();

					*bytes = std::vector<uint8_t>(buffer.begin(), buffer.end());
					return true;
				}

				bool Read(std::string file, std::string* bytes) override
				{
					std::vector<uint8_t> result;
					if (!Read(file, &result))
						return false;

					*bytes = std::string(result.begin(), result.end());
					return true;
				}

				bool Write(std::string file, std::string& bytes) override
				{
					File openFile = LittleFS.open(file.c_str(), "w");
					if (!openFile)
						return false;

					openFile.print(bytes.c_str());
					return true;
				}

				bool Write(std::string file, std::vector<uint8_t>& bytes) override
				{
					File openFile = LittleFS.open(file.c_str(), "w");
					if (!openFile)
						return false;
						
					openFile.write(&bytes[0], bytes.size());
					return true;
				}

				void Delete(std::string file) override
				{
					LittleFS.remove(file.c_str());
				}
			};
		}
	}
}