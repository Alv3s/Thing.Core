#pragma once

#include "gtest/gtest.h"
#include "../LoggerManager.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class LoggerManagerTest : public testing::Test
			{
			public:
				Thing::Core::LoggerManager<200> loggerManager;
				const std::string logMsgStr = "Test";

				virtual void SetUp() override;
				virtual void TearDown() override;
			};
		}
	}
}