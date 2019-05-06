#pragma once

#include "gmock/gmock.h"
#include "../IDigitalOutput.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class DigitalOutputMock : public virtual Thing::Core::IDigitalOutput
			{
			public:
				MOCK_METHOD1(DigitalWrite, void(Thing::Core::DigitalValue value));
				MOCK_METHOD0(GetState, Thing::Core::DigitalValue());
			};
		}
	}
}