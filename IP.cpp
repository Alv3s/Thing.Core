#include "IP.h"

namespace Thing
{
	namespace Core
	{
		IPAddress::IPAddress(uint32_t ip) : ip(ip)
		{
		}

		IPAddress::IPAddress(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) : 
			ip(byte1 | (int)(byte2 << 8) | (int)(byte3 << 16) | (int)(byte4 << 24))
		{
		}

		IPAddress::~IPAddress()
		{
		}

		uint8_t IPAddress::GetOctet(int index)
		{
			return (uint8_t)(ip >> (8 * index));
		}

		uint32_t IPAddress::GetIP()
		{
			return ip;
		}
	}
}