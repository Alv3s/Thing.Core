#include "MACAddress.h"
#include <cstdio>

namespace Thing
{
	namespace Core
	{
		MACAddress::MACAddress(uint8_t* address)
		{
			for(int i = 0; i < 6; ++i)
				this->address[i] = address[i];
		}

		MACAddress::MACAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6)
		{
			address[0] = byte1;
			address[1] = byte2;
			address[2] = byte3;
			address[3] = byte4;
			address[4] = byte5;
			address[5] = byte6;
		}

		MACAddress::~MACAddress()
		{

		}

		uint8_t MACAddress::GetOctet(int index)
		{
			return address[index];
		}

		uint8_t* MACAddress::GetAddress()
		{
			return address;
		}

		std::string MACAddress::AsString(char delimiter)
		{
			char str[18];
			sprintf(str, "%02x%c%02x%c%02x%c%02x%c%02x%c%02x\0", address[0], delimiter, address[1], delimiter, address[2], delimiter, address[3], delimiter, address[4], delimiter, address[5]);
			return std::string(str);
		}
	}
}

