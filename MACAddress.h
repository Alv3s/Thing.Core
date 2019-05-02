#pragma once

#include <cstdint> 

namespace Thing
{
	namespace Core
	{
		class MACAddress
		{
		public:
			MACAddress(uint8_t* address);
			MACAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4, uint8_t byte5, uint8_t byte6);
			~MACAddress();

			uint8_t GetOctet(int index);
			uint8_t* GetAddress();
		private:
			uint8_t address[6];
		};

	}
}

