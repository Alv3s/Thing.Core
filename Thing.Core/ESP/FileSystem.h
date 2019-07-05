#pragma once

#include "../IFileSystem.h"
#include <stdint.h>
#include <FS.h>

namespace Thing
{
	namespace Core
	{
		namespace ESP
		{
			class FileSystem : public virtual Thing::Core::IFileSystem
			{
			public:
				FileSystem()
				{
					opened = SPIFFS.begin();
				}

				bool Read(std::string file, std::vector<uint8_t>* bytes) override
				{
					if (!opened)
						return false;
					bool ret = SPIFFS.exists(file.c_str());
					if (!ret)
						return false;

					fs::File openFile = SPIFFS.open(file.c_str(), "r");
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
					fs::File openFile = SPIFFS.open(file.c_str(), "w");
					if (!openFile)
						return false;

					openFile.print(bytes.c_str());
					return true;
				}

				bool Write(std::string file, std::vector<uint8_t>& bytes) override
				{
					std::string string(bytes.begin(), bytes.end());
					return Write(file, string);
				}

				void Delete(std::string file) override
				{
					SPIFFS.remove(file.c_str());
				}
			private:
				bool opened;
			};
		}
	}
}