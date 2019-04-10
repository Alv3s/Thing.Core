#pragma once

#include "gtest/gtest.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class WiFiBaseTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;
			};
		}
	}
}
