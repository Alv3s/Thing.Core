#pragma once

#include "../IWiFi.h"
#include "../WiFiBase.h"

namespace Thing
{
	namespace Core
	{
		namespace Windows
		{
			class WiFi : public WiFiBase, public virtual IWiFi
			{
			public:
				WiFi();
				~WiFi();

				void Connect(std::string SSID, std::string password) override;
				void Disconnect() override;
				WiFiStatus Status() override;
				IPAddress GetIP() override;
				MACAddress GetMACAddress() override;
			};
		}
	}
}

