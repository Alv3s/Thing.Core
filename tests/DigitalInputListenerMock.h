#pragma once

#include "gmock/gmock.h"
#include "../IDigitalIOListeners.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class DigitalInputListenerMock : public virtual Thing::Core::IDigitalInputListener
			{
			public:
				MOCK_METHOD2(OnActivating, void(IDigitalIO* io, unsigned int count));
				MOCK_METHOD2(OnInactivating, void(IDigitalIO* io, unsigned int count));
			};
		}
	}
}