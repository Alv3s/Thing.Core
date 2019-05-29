#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/IDigitalIOListeners.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class DigitalOutputListenerMock : public virtual Thing::Core::IDigitalOutputListener
			{
			public:
				MOCK_METHOD2(OnActivating, void(IDigitalIO* io, unsigned int count));
				MOCK_METHOD2(OnInactivating, void(IDigitalIO* io, unsigned int count));
			};
		}
	}
}