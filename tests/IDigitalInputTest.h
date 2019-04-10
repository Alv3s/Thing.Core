#pragma once

#include "../IDigitalInput.h"
#include "gtest/gtest.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class IDigitalInputTest
			{
			public:
				static void TestCode(Thing::Core::IDigitalInput& input, const int code1 = 1, const int code2 = 2)
				{
					input.SetCode(code1);
					EXPECT_EQ(code1, input.GetCode());
					input.SetCode(code2);
					EXPECT_EQ(code2, input.GetCode());
				};

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