#ifdef ESP8266
#include <FS.h>
#include "ESPFileSystem.h"

bool ESPFileSystem::Read(std::string file, std::vector<uint8_t>* bytes)
{
	bool ret = SPIFFS.begin();
	if (!ret)
		return false;
	ret = SPIFFS.exists(file.c_str());
	if (!ret)
		return false;

	File openFile = SPIFFS.open(file.c_str(), "r");
	size_t size = openFile.size();
	std::vector<char> buffer(size, 0);
	openFile.readBytes(&buffer[0], size);
	openFile.close();

	*bytes = std::vector<uint8_t>(buffer.begin(), buffer.end());
	return true;
}

bool ESPFileSystem::Read(std::string file, std::string* bytes)
{
	std::vector<uint8_t> result;
	if (!Read(file, &result))
		return false;

	*bytes = std::string(result.begin(), result.end());
	return true;
}

bool ESPFileSystem::Write(std::string file, std::string& bytes)
{
	File openFile = SPIFFS.open(file.c_str(), "w");
	if (!openFile)
		return false;

	openFile.print(bytes.c_str());
	return true;
}

bool ESPFileSystem::Write(std::string file, std::vector<uint8_t>& bytes)
{
	std::string string(bytes.begin(), bytes.end());
	return Write(file, string);
}

void ESPFileSystem::Delete(std::string file)
{
	SPIFFS.remove("/config.json");
}
#endif
