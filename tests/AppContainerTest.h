#pragma once

#include "gtest/gtest.h"
#include "../AppContainer.h"
#include "AppMock.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AppContainerTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				AppMock App;
				AppContainer AppContainer;
			};
		}
	}
}