#pragma once

#include "gmock/gmock.h"
#include "../IAnalogInput.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AnalogInputMock : public virtual Thing::Core::IAnalogInput
			{
			public:
				MOCK_METHOD0(GetPrecision, unsigned int());
				MOCK_METHOD0(AnalogRead, int());
				MOCK_CONST_METHOD0(GetCode, int());
				MOCK_METHOD1(SetCode, void(int code));
			};
		}
	}
}