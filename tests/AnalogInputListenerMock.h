#pragma once

#include "gmock/gmock.h"
#include "../IAnalogIOListener.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AnalogInputListenerMock : public virtual Thing::Core::IAnalogInputListener
			{
			public:
				MOCK_METHOD2(OnIncreasingValue, void(IAnalogIO* io, int value));
				MOCK_METHOD2(OnDecreasingValue, void(IAnalogIO* io, int value));
			};
		}
	}
}
