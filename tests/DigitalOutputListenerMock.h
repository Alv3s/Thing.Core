#pragma once

#include "gmock/gmock.h"
#include "../IDigitalIOListeners.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class DigitalOutputListenerMock : public virtual Thing::Core::IDigitalOutputListener
			{
			public:
				MOCK_METHOD2(OnActivating, void(int code, unsigned int count));
				MOCK_METHOD2(OnInactivating, void(int code, unsigned int count));
			};
		}
	}
}