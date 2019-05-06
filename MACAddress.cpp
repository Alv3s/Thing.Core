#include "MACAddress.h"

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
	}
}

