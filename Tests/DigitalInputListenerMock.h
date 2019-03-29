#pragma once

#include "gmock/gmock.h"
#include "../IDigitalIOListeners.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class DigitalInputListenerMock : public virtual Thing::Core::IDigitalInputListener
			{
			public:
				MOCK_METHOD1(OnActivating, void(int code));
				MOCK_METHOD2(OnActivating, void(int code, unsigned int count));
				MOCK_METHOD1(OnInactivating, void(int code));
				MOCK_METHOD2(OnInactivating, void(int code, unsigned int count));
			};
		}
	}
}