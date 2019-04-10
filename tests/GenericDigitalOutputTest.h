#pragma once

#include "gtest/gtest.h"
#include "HardwareMock.h"
#include "LoggerManagerMock.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class GenericDigitalOutputTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;

				HardwareMock HardwareMock;
				LoggerManagerMock LoggerMock;
			};
		}
	}
}