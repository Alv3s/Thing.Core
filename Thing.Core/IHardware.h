#pragma once

#include "HardwareEnumerations.h"
#include "IRunnable.h"

namespace Thing
{
	namespace Core
	{
		class IHardware
		{
		public:
			virtual void ConfigurePin(int gpio, PinMode mode) = 0;

			virtual void DigitalWrite(int gpio, DigitalValue value) = 0;
			virtual DigitalValue DigitalRead(int gpio) = 0;

			virtual int AnalogRead(int gpio) = 0;

			virtual unsigned long Millis() = 0;
			virtual unsigned long Micros() = 0;
			virtual void Delay(unsigned long millis) = 0;

			virtual void AttachInterrupt(int gpio, InterruptMode mode, IRunnable* runnable) = 0;
			virtual void AttachInterrupt(int gpio, InterruptMode mode, IRunnable& runnable) = 0;
			virtual void DetachInterrupt(int gpio) = 0;

			virtual void Reset() = 0;
		};
	}
}