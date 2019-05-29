#include "GenericInverseDigitalOutputTest.h"
#include "IDigitalOutputTest.h"
#include "../Thing.Core/Ioc.h"
#include "../Thing.Core/GenericInverseDigitalOutput.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			void GenericInverseDigitalOutputTest::SetUp()
			{
				Hardware = &HardwareMock;
				Logger = &LoggerMock;
			}

			void GenericInverseDigitalOutputTest::TearDown()
			{
			}

			TEST_F(GenericInverseDigitalOutputTest, InitializeTest)
			{
				const int gpio[] = { 10, 12 };
				const Thing::Core::DigitalValue initValue[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };

				for (int i = 0; i < 2; ++i)
				{
					EXPECT_CALL(HardwareMock, ConfigurePin(gpio[i], Thing::Core::PinMode::Output)).Times(1);
					Thing::Core::GenericInverseDigitalOutput output(gpio[i], initValue[i]);
				}
			}

			TEST_F(GenericInverseDigitalOutputTest, WriteValueTest)
			{
				const Thing::Core::DigitalValue initValue[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const int gpio = 1;

				Thing::Core::GenericInverseDigitalOutput output(gpio);

				for (int i = 0; i < sizeof(initValue) / sizeof(Thing::Core::DigitalValue); ++i)
				{
					Thing::Core::DigitalValue inverseValued = initValue[i] == DigitalValue::High ? DigitalValue::Low : DigitalValue::High;
					EXPECT_CALL(HardwareMock, DigitalWrite(gpio, inverseValued)).Times(1);
					output.DigitalWrite(initValue[i]);
					EXPECT_EQ(initValue[i], output.GetState());
				}
			}

			TEST_F(GenericInverseDigitalOutputTest, WriteValueToggleTest)
			{
				const int totalToggles = 1;
				const int gpio = 1;

				Thing::Core::DigitalValue state = DigitalValue::High;
				Thing::Core::GenericInverseDigitalOutput output(gpio, state);

				for (int i = 0; i < totalToggles; ++i)
				{
					Thing::Core::DigitalValue nextValue = state == DigitalValue::High ? DigitalValue::Low : DigitalValue::High;
					EXPECT_CALL(HardwareMock, DigitalWrite(gpio, state)).Times(1);
					output.DigitalWrite(DigitalValue::Toggle);
					EXPECT_EQ(nextValue, output.GetState());
					state = nextValue;
				}
			}
		}
	}
}