#pragma once

#include <string>
#include "HardwareEnumerations.h"
#include "IP.h"

namespace Thing
{
	namespace Core
	{
		class IWiFiListener
		{
		public:
			virtual void OnConnect() = 0;
			virtual void OnDisconnect() = 0;
		};

		class IWiFi
		{
		public:
			virtual void Connect(std::string SSID, std::string password) = 0;
			virtual void Disconnect() = 0;
			virtual WiFiStatus Status() = 0;
			virtual IPAddress GetIP() = 0;

			virtual void AddListener(IWiFiListener* listener) = 0;
			virtual void AddListener(IWiFiListener& listener) = 0;

			virtual void RemoveListener(IWiFiListener* listener) = 0;
			virtual void RemoveListener(IWiFiListener& listener) = 0;
		};
	}
}