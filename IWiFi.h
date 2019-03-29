#pragma once

#include <string>
#include "HardwareEnumerations.h"
#include "IP.h"

namespace Thing
{
	namespace Core
	{
		class IWiFi
		{
		public:
			virtual void Connect(std::string SSID, std::string password) = 0;
			virtual void Disconnect() = 0;
			virtual WiFiStatus Status() = 0;
			virtual IPAddress GetIP() = 0;
		};
	}
}