#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/IDigitalInput.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class DigitalInputMock : public virtual Thing::Core::IDigitalInput
			{
			public:
				MOCK_METHOD0(DigitalRead, Thing::Core::DigitalValue());
			};
		}
	}
}