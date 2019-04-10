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
				const int code = 5;
				const unsigned int precision = 1;

				EXPECT_CALL(HardwareMock, ConfigurePin(gpio, Thing::Core::PinMode::Input)).Times(1);
				Thing::Core::GenericAnalogInput input(gpio, code, precision);
				EXPECT_EQ(code, input.GetCode());
			}

			TEST_F(GenericAnalogInputTest, ReadValueTest)
			{
				const int gpio = 10;
				const int code = 5;
				const unsigned int precision = 1;

				EXPECT_CALL(HardwareMock, AnalogRead(gpio))
					.Times(3)
					.WillOnce(Return(10))
					.WillOnce(Return(20))
					.WillOnce(Return(15));

				Thing::Core::GenericAnalogInput input(gpio, code, precision);

				EXPECT_EQ(10, input.AnalogRead());
				EXPECT_EQ(20, input.AnalogRead());
				EXPECT_EQ(15, input.AnalogRead());
			}

			TEST_F(GenericAnalogInputTest, SetCodeTest)
			{
				const int gpio = 1;
				int code1 = 1;
				int code2 = 2;

				Thing::Core::GenericAnalogInput input(gpio, gpio,1);
				input.SetCode(code1);
				EXPECT_EQ(code1, input.GetCode());
				input.SetCode(code2);
				EXPECT_EQ(code2, input.GetCode());
			}

			TEST_F(GenericAnalogInputTest, GetPrecisionTest)
			{
				const int gpio = 10;
				const int code = 5;
				const unsigned int precision = 2U;

				EXPECT_CALL(HardwareMock, ConfigurePin(gpio, Thing::Core::PinMode::Input)).Times(1);
				Thing::Core::GenericAnalogInput input(gpio, code, precision);
				EXPECT_EQ(precision, input.GetPrecision());
			}
		}
	}
}