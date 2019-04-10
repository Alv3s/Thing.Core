#pragma once

#include "gtest/gtest.h"
#include "TaskSchedulerMock.h"
#include "TimedHardwareMock.h"
#include "LoggerManagerMock.h"
#include "../IOManager.h"
#include "AnalogInputListenerMock.h"
#include "AnalogInputMock.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class IOManagerTest : public testing::Test
			{
			public:
				void SetUp() override;
				void TearDown() override;
				LoggerManagerMock LoggerMock;

			protected:
				void NotifyAnalogInputListeners(Thing::Core::IOManager& Manager, const int totalListeners, AnalogInputListenerMock* listeners);
				void TestListenerEventTestWithMultipleAnalogInputs(Thing::Core::IOManager& Manager, const int totalInputs, AnalogInputMock* inputs);
				void TestRemoveAnalogInputs(Thing::Core::IOManager& Manager, AnalogInputMock& input1, AnalogInputMock& input2, AnalogInputMock& input3);

			private:
				TaskSchedulerMock TaskSchedulerMock;
				TimedHardwareMock HardwareMock;
			};
		}
	}
}