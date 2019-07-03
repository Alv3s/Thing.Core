#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/IHardware.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class HardwareMock : public virtual Thing::Core::IHardware
			{
			public:
				MOCK_METHOD2(ConfigurePin, void(int gpio, Thing::Core::PinMode mode));
				MOCK_METHOD2(DigitalWrite, void(int gpio, Thing::Core::DigitalValue value));
				MOCK_METHOD1(DigitalRead, Thing::Core::DigitalValue(int gpio));
				MOCK_METHOD1(AnalogRead, int(int gpio));

				MOCK_METHOD0(Millis, unsigned long());
				MOCK_METHOD0(Micros, unsigned long());
				MOCK_METHOD1(Delay, void(unsigned long millis));

				virtual MOCK_METHOD3(AttachInterrupt, void(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable* runnable));
				virtual MOCK_METHOD3(AttachInterrupt, void(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable& runnable));
				virtual MOCK_METHOD1(DetachInterrupt, void(int gpio));

				MOCK_METHOD0(Reset, void());
			};
		}
	}
}