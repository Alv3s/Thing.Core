#pragma once

#include "../IHardware.h"
#include <Arduino.h>
#ifdef ESP8266
#include <Esp.h>
#endif
#define INTERRUPT_METHOD(n) static void interrupt ## n() { interrupts[n]->Run();}

namespace Thing
{
	namespace Core
	{
		namespace Arduino
		{
			class Hardware : public virtual Thing::Core::IHardware
			{
			public:
				void ConfigurePin(int gpio, Thing::Core::PinMode mode) override
				{
					switch (mode)
					{
					case Thing::Core::PinMode::Input:
						pinMode(gpio, INPUT);
						break;
					case Thing::Core::PinMode::Output:
						pinMode(gpio, OUTPUT);
					}
				}

				void DigitalWrite(int gpio, Thing::Core::DigitalValue value) override
				{
					switch (value)
					{
					case Thing::Core::DigitalValue::Low:
						digitalWrite(gpio, LOW);
						break;
					case Thing::Core::DigitalValue::High:
						digitalWrite(gpio, HIGH);
					}
				}

				Thing::Core::DigitalValue DigitalRead(int gpio) override
				{
					bool value = digitalRead(gpio);
					return Thing::Core::DigitalValue(value);
				}

				int AnalogRead(int gpio) override
				{
					return analogRead(gpio);
				}

				unsigned long Millis() override
				{
					return millis();
				}

				unsigned long Micros() override
				{
					return micros();
				}

				void Delay(unsigned long millis) override
				{
					delay(millis);
				}

				void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable* runnable) override
				{
					int aMode = RISING;
					switch (mode)
					{
					case Thing::Core::InterruptMode::WasActivated: aMode = RISING;  break;
					case Thing::Core::InterruptMode::WasInactivated: aMode = FALLING;  break;
					case Thing::Core::InterruptMode::OnChange: aMode = CHANGE;  break;
					}

					int interrupt = digitalPinToInterrupt(gpio);
					interrupts[interrupt] = runnable;

					switch (interrupt)
					{
					case 0: attachInterrupt(interrupt, interrupt0, aMode); break;
					case 1: attachInterrupt(interrupt, interrupt1, aMode); break;
					case 2: attachInterrupt(interrupt, interrupt2, aMode); break;
					case 3: attachInterrupt(interrupt, interrupt3, aMode); break;
					case 4: attachInterrupt(interrupt, interrupt4, aMode); break;
					case 5: attachInterrupt(interrupt, interrupt5, aMode); break;
					case 6: attachInterrupt(interrupt, interrupt6, aMode); break;
					case 7: attachInterrupt(interrupt, interrupt7, aMode); break;
					case 8: attachInterrupt(interrupt, interrupt8, aMode); break;
					case 9: attachInterrupt(interrupt, interrupt9, aMode); break;
					case 10: attachInterrupt(interrupt, interrupt10, aMode); break;
					case 11: attachInterrupt(interrupt, interrupt11, aMode); break;
					case 12: attachInterrupt(interrupt, interrupt12, aMode); break;
					case 13: attachInterrupt(interrupt, interrupt13, aMode); break;
					case 14: attachInterrupt(interrupt, interrupt14, aMode); break;
					case 15: attachInterrupt(interrupt, interrupt15, aMode); break;
					}
				}

				void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable& runnable) override
				{
					AttachInterrupt(gpio, mode, &runnable);
				}

				void DetachInterrupt(int gpio) override
				{
					int interrupt = digitalPinToInterrupt(gpio);
					detachInterrupt(interrupt);
				}

				void Reset() override
				{
#ifdef ESP8266
					ESP.reset();
#endif
				}
			private:
				static Thing::Core::IRunnable* interrupts[16];

				INTERRUPT_METHOD(0);
				INTERRUPT_METHOD(1);
				INTERRUPT_METHOD(2);
				INTERRUPT_METHOD(3);
				INTERRUPT_METHOD(4);
				INTERRUPT_METHOD(5);
				INTERRUPT_METHOD(6);
				INTERRUPT_METHOD(7);
				INTERRUPT_METHOD(8);
				INTERRUPT_METHOD(9);
				INTERRUPT_METHOD(10);
				INTERRUPT_METHOD(11);
				INTERRUPT_METHOD(12);
				INTERRUPT_METHOD(13);
				INTERRUPT_METHOD(14);
				INTERRUPT_METHOD(15);
			};

			Thing::Core::IRunnable* Hardware::interrupts[16];
		}
	}
}
#undef INTERRUPT_METHOD(n)