#ifdef _WIN32

#include "pch.h"
#include "WindowsWiFi.h"

namespace Thing
{
	namespace Core
	{
		namespace Windows
		{
			WiFi::WiFi()
			{
			}


			WiFi::~WiFi()
			{
			}

			void WiFi::Connect(std::string SSID, std::string password)
			{
			}

			void WiFi::Disconnect()
			{
			}

			WiFiStatus WiFi::Status()
			{
				return WiFiStatus();
			}

			IPAddress WiFi::GetIP()
			{
				return IPAddress(127, 0, 0, 1);
			}

			MACAddress WiFi::GetMACAddress()
			{
				return MACAddress(0, 0, 0, 0, 0, 0);
			}
		}
	}
}
#endif