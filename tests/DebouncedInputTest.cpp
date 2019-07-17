#include "DebouncedInputTest.h"
#include "../Thing.Core/IoC.h"
#include "../Thing.Core/DebouncedInput.h"
#include "DigitalInputMock.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			class DebouncedMockInput : public DigitalInputMock
			{
			public:
				DebouncedMockInput(int a)
				{
					EXPECT_CALL(*this, DigitalRead()).WillOnce(Return(DigitalValue::Low));
				}
			};

			void DebouncedInputTest::SetUp()
			{
				Hardware = &HardwareMock;
			}

			void DebouncedInputTest::TearDown()
			{
			}

			TEST_F(DebouncedInputTest, SimpleDebounceStart)
			{
				const int debounceTimer = 500;
				DebouncedInput<DebouncedMockInput> input(debounceTimer, 1);

				DebouncedMockInput& inputMock = (DebouncedMockInput&)input;

				//First call. No debouncing
				EXPECT_CALL(inputMock, DigitalRead()).WillOnce(Return(DigitalValue::Low));
				EXPECT_EQ(DigitalValue::Low, input.DigitalRead());

				//Should always return High, since it's debouncing
				EXPECT_CALL(inputMock, DigitalRead())
					.WillOnce(Return(DigitalValue::High))
					.WillRepeatedly(Return(DigitalValue::Low));
				for (int increment = 100, i = increment; i < debounceTimer; i += increment)
				{
					HardwareMock.Delay(increment);
					EXPECT_EQ(DigitalValue::High, input.DigitalRead());
				}

				HardwareMock.Delay(debounceTimer);
				EXPECT_EQ(DigitalValue::Low, input.DigitalRead());
			}

			TEST_F(DebouncedInputTest, SimpleDebounceEnd)
			{
				const int debounceTimer = 500;
				DebouncedInput<DebouncedMockInput> input(debounceTimer, 1);

				DebouncedMockInput& inputMock = (DebouncedMockInput&)input;

				//First call. No debouncing
				EXPECT_CALL(inputMock, DigitalRead()).WillOnce(Return(DigitalValue::Low));
				ASSERT_EQ(DigitalValue::Low, input.DigitalRead());

				//Force value change.
				EXPECT_CALL(inputMock, DigitalRead()).WillOnce(Return(DigitalValue::High));
				ASSERT_EQ(DigitalValue::High, input.DigitalRead());

				//Make enough time passed for the debouncer
				HardwareMock.Delay(debounceTimer);
				EXPECT_CALL(inputMock, DigitalRead()).WillOnce(Return(DigitalValue::Low));
				ASSERT_EQ(DigitalValue::Low, input.DigitalRead());

				//Should always return Low, since it's debouncing
				EXPECT_CALL(inputMock, DigitalRead())
					.WillRepeatedly(Return(DigitalValue::High));
				for (int increment = 100, i = increment; i < debounceTimer; i += increment)
				{
					HardwareMock.Delay(increment);
					ASSERT_EQ(DigitalValue::Low, input.DigitalRead());
				}

				//After enough debouncing time has passed, the new value is assumed
				HardwareMock.Delay(debounceTimer);
				ASSERT_EQ(DigitalValue::High, input.DigitalRead());
			}
		}
	}
}