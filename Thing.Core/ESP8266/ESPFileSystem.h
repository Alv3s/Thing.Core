#pragma once

#include "../IFileSystem.h"
#include <stdint.h>

class ESPFileSystem : public virtual Thing::Core::IFileSystem
{
public:
	bool Read(std::string file, std::vector<uint8_t>* bytes) override;
	bool Read(std::string file, std::string* bytes) override;

	bool Write(std::string file, std::string& bytes) override;
	bool Write(std::string file, std::vector<uint8_t>& bytes) override;

	void Delete(std::string file) override;
};