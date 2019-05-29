#pragma once

#include "../Thing.Core/IDigitalInput.h"
#include "gtest/gtest.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class IDigitalInputTest
			{
			public:
				static void TestDigitalRead(Thing::Core::IDigitalInput& input)
				{
					EXPECT_EQ(Thing::Core::DigitalValue::Low, input.DigitalRead());
					EXPECT_EQ(Thing::Core::DigitalValue::High, input.DigitalRead());
					EXPECT_EQ(Thing::Core::DigitalValue::Low, input.DigitalRead());
				};
			};
		}
	}
}