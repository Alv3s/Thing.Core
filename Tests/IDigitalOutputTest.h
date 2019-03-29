#pragma once

#include "../IDigitalOutput.h"
#include "gtest/gtest.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class IDigitalOutputTest
			{
			public:
				static void TestCode(Thing::Core::IDigitalOutput& output, const int code1 = 1, const int code2 = 2)
				{
					output.SetCode(code1);
					EXPECT_EQ(code1, output.GetCode());
					output.SetCode(code2);
					EXPECT_EQ(code2, output.GetCode());
				};
			};
		}
	}
}