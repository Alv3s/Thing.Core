#include "GenericAnalogInputTest.h"
#include "../IoC.h"
#include "../GenericAnalogInput.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			void GenericAnalogInputTest::SetUp()
			{
				Hardware = &HardwareMock;
				Logger = &LoggerMock;
				ON_CALL(HardwareMock, AnalogRead(_)).WillByDefault(Return(0));
			}

			void GenericAnalogInputTest::TearDown()
			{
			}

			TEST_F(GenericAnalogInputTest, InitializeTest)
			{
				const int gpio = 10;
				const unsigned int precision = 1;

				EXPECT_CALL(HardwareMock, ConfigurePin(gpio, Thing::Core::PinMode::Input)).Times(1);
				Thing::Core::GenericAnalogInput input(gpio, precision);
			}

			TEST_F(GenericAnalogInputTest, ReadValueTest)
			{
				const int gpio = 10;
				const unsigned int precision = 1;

				EXPECT_CALL(HardwareMock, AnalogRead(gpio))
					.Times(3)
					.WillOnce(Return(10))
					.WillOnce(Return(20))
					.WillOnce(Return(15));

				Thing::Core::GenericAnalogInput input(gpio, precision);

				EXPECT_EQ(10, input.AnalogRead());
				EXPECT_EQ(20, input.AnalogRead());
				EXPECT_EQ(15, input.AnalogRead());
			}

			TEST_F(GenericAnalogInputTest, GetPrecisionTest)
			{
				const int gpio = 10;
				const unsigned int precision = 2U;

				EXPECT_CALL(HardwareMock, ConfigurePin(gpio, Thing::Core::PinMode::Input)).Times(1);
				Thing::Core::GenericAnalogInput input(gpio, precision);
				EXPECT_EQ(precision, input.GetPrecision());
			}
		}
	}
}