#include "GenericDigitalInputTest.h"
#include "../Ioc.h"
#include "IDigitalInputTest.h"
#include "../GenericDigitalInput.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			void GenericDigitalInputTest::SetUp()
			{
				Hardware = &HardwareMock;
				Logger = &LoggerMock;
				ON_CALL(HardwareMock, DigitalRead(_)).WillByDefault(Return(Thing::Core::DigitalValue::Low));
			}

			void GenericDigitalInputTest::TearDown()
			{
			}

			TEST_F(GenericDigitalInputTest, InitializeTest)
			{
				const int gpio = 10;
				const int code = 5;

				EXPECT_CALL(HardwareMock, ConfigurePin(gpio, Thing::Core::PinMode::Input)).Times(1);
				Thing::Core::GenericDigitalInput input(gpio, code);
				EXPECT_EQ(code, input.GetCode());
			}

			TEST_F(GenericDigitalInputTest, ReadValueTest)
			{
				const int gpio = 10;
				const int code = 5;

				EXPECT_CALL(HardwareMock, DigitalRead(gpio))
					.Times(3)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));

				Thing::Core::GenericDigitalInput input(gpio, code);
				IDigitalInputTest::TestDigitalRead(input);
			}

			TEST_F(GenericDigitalInputTest, SetCodeTest)
			{
				const int gpio = 1;
				Thing::Core::GenericDigitalInput input(gpio, gpio);
				IDigitalInputTest::TestCode(input);
			}
		}
	}
}