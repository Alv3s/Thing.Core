#pragma once

#include "gtest/gtest.h"
#include "TimedHardwareMock.h"
#include "AppContainerMock.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AppTaskSchedulerTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				AppContainerMock appContainerMock;
				TimedHardwareMock hardwareMock;
			};
		}
	}
}

