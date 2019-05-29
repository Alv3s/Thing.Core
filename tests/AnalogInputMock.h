#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/IAnalogInput.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AnalogInputMock : public virtual Thing::Core::IAnalogInput
			{
			public:
				MOCK_METHOD0(GetPrecision, unsigned int());
				MOCK_METHOD0(AnalogRead, int());
			};
		}
	}
}