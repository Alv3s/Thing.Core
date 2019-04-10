#pragma once

#include "gmock/gmock.h"
#include "../IWiFi.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class IWiFiListenerMock : public virtual Thing::Core::IWiFiListener
			{
			public:
				MOCK_METHOD0(OnConnect, void());
				MOCK_METHOD0(OnDisconnect, void());
			};
		}
	}
}