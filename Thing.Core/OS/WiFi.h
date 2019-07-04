#pragma once

#include "../IWiFi.h"
#include "../WiFiBase.h"

namespace Thing
{
	namespace Core
	{
		namespace OS
		{
			class WiFi : public WiFiBase, public virtual IWiFi
			{
			public:
				WiFi() : status(WiFiStatus::Disconnected)
				{

				}

				~WiFi()
				{

				}

				void Connect(std::string SSID, std::string password) override
				{
					WiFiStatus previousStatus = status;
					status = WiFiStatus::Connected;
					if (previousStatus != status)
						SignalOnConnect();
				}

				void Disconnect() override
				{
					WiFiStatus previousStatus = status;
					status = WiFiStatus::Disconnected;
					if (previousStatus != status)
						SignalOnDisconnect();
				}

				WiFiStatus Status() override
				{
					return status;
				}

				IPAddress GetIP() override
				{
					return IPAddress(127, 0, 0, 1);
				}

				MACAddress GetMACAddress() override
				{
					return MACAddress(0, 0, 0, 0, 0, 0);
				}
			private:
				WiFiStatus status;
			};
		}
	}
}

