#include "GenericInverseDigitalInputTest.h"
#include "../Ioc.h"
#include "IDigitalInputTest.h"
#include "../GenericInverseDigitalInput.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			void GenericInverseDigitalInputTest::SetUp()
			{
				Hardware = &HardwareMock;
				Logger = &LoggerMock;
				ON_CALL(HardwareMock, DigitalRead(_)).WillByDefault(Return(Thing::Core::DigitalValue::Low));
			}

			void GenericInverseDigitalInputTest::TearDown()
			{
			}

			TEST_F(GenericInverseDigitalInputTest, InitializeTest)
			{
				const int gpio = 10;

				EXPECT_CALL(HardwareMock, ConfigurePin(gpio, Thing::Core::PinMode::Input)).Times(1);
				Thing::Core::GenericInverseDigitalInput input(gpio);
			}

			TEST_F(GenericInverseDigitalInputTest, ReadValueTest)
			{
				const int gpio = 10;

				EXPECT_CALL(HardwareMock, DigitalRead(gpio))
					.Times(3)
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));

				Thing::Core::GenericInverseDigitalInput input(gpio);
				IDigitalInputTest::TestDigitalRead(input);
			}
		}
	}
}