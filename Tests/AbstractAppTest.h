#pragma once

#include "gtest/gtest.h"
#include "AbstractAppMock.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AbstractAppTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				AbstractAppMock App;
			};
		}
	}
}