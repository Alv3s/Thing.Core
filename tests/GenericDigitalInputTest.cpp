#include "GenericDigitalInputTest.h"
#include "../Thing.Core/Ioc.h"
#include "IDigitalInputTest.h"
#include "../Thing.Core/GenericDigitalInput.h"

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

				EXPECT_CALL(HardwareMock, ConfigurePin(gpio, Thing::Core::PinMode::Input)).Times(1);
				Thing::Core::GenericDigitalInput input(gpio);
			}

			TEST_F(GenericDigitalInputTest, ReadValueTest)
			{
				const int gpio = 10;

				EXPECT_CALL(HardwareMock, DigitalRead(gpio))
					.Times(3)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));

				Thing::Core::GenericDigitalInput input(gpio);
				IDigitalInputTest::TestDigitalRead(input);
			}
		}
	}
}