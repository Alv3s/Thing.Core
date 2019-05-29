#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/WiFiBase.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class WiFiBaseMock : public Thing::Core::WiFiBase, public virtual Thing::Core::IWiFi
			{
			public:
				MOCK_METHOD2(Connect, void(std::string, std::string));
				MOCK_METHOD0(Disconnect, void());
				MOCK_METHOD0(Status, WiFiStatus());
				MOCK_METHOD0(GetIP, IPAddress());
				MOCK_METHOD0(GetMACAddress, MACAddress());

				void SignalOnConnectMock()
				{
					SignalOnConnect();
				}

				void SignalOnDisconnectMock()
				{
					SignalOnDisconnect();
				}

				void SignalOnFailedMock()
				{
					SignalOnFailed();
				}
			};
		}
	}
}