#pragma once

#include <cstdint> 

namespace Thing
{
	namespace Core
	{
		class IPAddress
		{
		public:
			IPAddress(uint32_t ip);
			IPAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);
			~IPAddress();

			uint8_t GetOctet(int index);
			uint32_t GetIP();
		private:
			uint32_t ip;
		};
	}
}
