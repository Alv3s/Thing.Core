#pragma once

#pragma once

#include "gtest/gtest.h"
#include "TimedHardwareMock.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class DebouncedInputTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				TimedHardwareMock HardwareMock;
			};
		}
	}
}
