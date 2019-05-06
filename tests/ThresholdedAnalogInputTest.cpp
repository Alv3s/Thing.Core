#include "ThresholdedAnalogInputTest.h"
#include "../Ioc.h"
#include "IDigitalInputTest.h"
#include "../ThresholdedAnalogInput.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			void ThresholdedAnalogInputTest::SetUp()
			{
				Hardware = &HardwareMock;
				Logger = &LoggerMock;
			}

			void ThresholdedAnalogInputTest::TearDown()
			{

			}

			TEST_F(ThresholdedAnalogInputTest, InitializeTest)
			{
				const int gpio = 20;
				const unsigned int precision = 1U;

				Thing::Core::ThresholdedAnalogInput input(gpio, precision);
			}

			TEST_F(ThresholdedAnalogInputTest, DigitalReadWithHigherThreshold)
			{
				const int threshold = 500;
				const int gpio = 20;
				const unsigned int precision = 1U;

				EXPECT_CALL(HardwareMock, AnalogRead(gpio))
					.Times(3)
					.WillOnce(Return(threshold - 1))
					.WillOnce(Return(threshold + 1))
					.WillOnce(Return(threshold - 1));

				Thing::Core::ThresholdedAnalogInput input(gpio, precision);
				input.SetThresholdHigherThan(threshold);
				IDigitalInputTest::TestDigitalRead(input);
			}

			TEST_F(ThresholdedAnalogInputTest, DigitalReadWithLowerThreshold)
			{
				const int threshold = 500;
				const int gpio = 20;
				const unsigned int precision = 1U;

				EXPECT_CALL(HardwareMock, AnalogRead(gpio))
					.Times(3)
					.WillOnce(Return(threshold + 1))
					.WillOnce(Return(threshold - 1))
					.WillOnce(Return(threshold + 1));

				Thing::Core::ThresholdedAnalogInput input(gpio, precision);
				input.SetThresholdLesserThan(threshold);
				IDigitalInputTest::TestDigitalRead(input);
			}

			TEST_F(ThresholdedAnalogInputTest, GetPrecisionTest)
			{
				const unsigned int precision = 15U;
				Thing::Core::ThresholdedAnalogInput input(1, precision);
				unsigned int actualPrecision = input.GetPrecision();
				
				ASSERT_EQ(precision, actualPrecision);
			}
		}
	}
}