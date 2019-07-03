#ifdef _WIN32
#include "pch.h"
#include "FakeHardware.h"
#include <chrono>
#include "../IoC.h"

namespace Thing
{
	namespace Core
	{
		namespace Windows
		{
			std::map<int, FakeHardware::GPIO*> FakeHardware::gpios;

			void FakeHardware::asureGpio(int gpio)
			{
				if (gpios.count(gpio) == 0)
				{
					GPIO* obj = new GPIO(gpio);
					gpios.insert(std::pair<int, GPIO*>(gpio, obj));
				}
			}

			void FakeHardware::ConfigurePin(int gpio, Thing::Core::PinMode mode)
			{
				asureGpio(gpio);
				gpios[gpio]->SetMode(mode);
			}

			void FakeHardware::MockInputState(int gpio, Thing::Core::DigitalValue value)
			{
				if(gpios.count(gpio) == 0)
				{
					Logger->Error("Attempting to Write to Uninitialized Pin %d to level %s.", gpio, convertToString(value).c_str());
					return;
				}
				if (gpios[gpio]->GetMode() == Thing::Core::PinMode::Output)
				{
					Logger->Error("Attempting to Write to Output Pin %d to level %s.", gpio, convertToString(value).c_str());
					return;
				}
				gpios[gpio]->SetValue(value);
			}

			void FakeHardware::DigitalWrite(int gpio, Thing::Core::DigitalValue value)
			{
				asureGpio(gpio);
				if (gpios[gpio]->GetMode() == Thing::Core::PinMode::Input)
				{
					Logger->Error("Attempting to Write to Input Pin %d to level %s.", gpio, convertToString(value).c_str());
					return;
				}
				gpios[gpio]->SetValue(value);
			}

			Thing::Core::DigitalValue FakeHardware::DigitalRead(int gpio)
			{
				asureGpio(gpio);
				return gpios[gpio]->GetValue();
			}

			int FakeHardware::AnalogRead(int gpio)
			{
				asureGpio(gpio);
				return gpios[gpio]->GetAnalogValue();
			}

			unsigned long FakeHardware::Millis()
			{
				auto now = std::chrono::system_clock::now();
				auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
				auto epoch = now_ms.time_since_epoch();
				return epoch.count();
			}

			unsigned long FakeHardware::Micros()
			{
				auto now = std::chrono::system_clock::now();
				auto now_us = std::chrono::time_point_cast<std::chrono::microseconds>(now);
				auto epoch = now_us.time_since_epoch();
				return epoch.count();
			}

			void FakeHardware::Delay(unsigned long millis)
			{
				Sleep(millis);
			}

			void FakeHardware::AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable * runnable)
			{
				asureGpio(gpio);
				gpios[gpio]->SetInterrupt(runnable, mode);
			}

			void FakeHardware::AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable & runnable)
			{
				AttachInterrupt(gpio, mode, &runnable);
			}

			void FakeHardware::DetachInterrupt(int gpio)
			{
				asureGpio(gpio);
				gpios[gpio]->ClearInterrupt();
			}

			void FakeHardware::Reset()
			{
				//TODO: Implement Reset. This should restart program
			}

			void FakeHardware::SetAnalogValue(int gpio, int value)
			{
				if (gpios.count(gpio) == 0)
					return;

				if (gpios[gpio]->GetMode() != Thing::Core::PinMode::Input)
					return;

				gpios[gpio]->SetAnalogValue(value);
			}

			void FakeHardware::SetInputState(int gpio, Thing::Core::DigitalValue value)
			{
				if (gpios.count(gpio) == 0)
					return;

				if (gpios[gpio]->GetMode() != Thing::Core::PinMode::Input)
					return;

				gpios[gpio]->SetValue(value);
			}

			void FakeHardware::AddListener(IGPIOListener* listener, int gpio)
			{
				asureGpio(gpio);
				gpios[gpio]->AddListener(listener);
			}




			FakeHardware::GPIO::GPIO(int pin) : pin(pin), value(Thing::Core::DigitalValue::Low), interrupt(NULL)
			{
			}

			void FakeHardware::GPIO::SetMode(Thing::Core::PinMode mode)
			{
				this->mode = mode;
			}

			Thing::Core::PinMode FakeHardware::GPIO::GetMode()
			{
				return mode;
			}

			void FakeHardware::GPIO::SetValue(Thing::Core::DigitalValue value)
			{
				Thing::Core::DigitalValue previousValue = this->value;
				if (value == Thing::Core::DigitalValue::Toggle)
					this->value = this->value == Thing::Core::DigitalValue::High ? Thing::Core::DigitalValue::Low : Thing::Core::DigitalValue::High;
				else
					this->value = value;

				if (previousValue != this->value)
				{
					if (value == DigitalValue::High)
						for (auto l : listeners)
							l->OnActivating(this->pin);
					else
						for (auto l : listeners)
							l->OnInactivating(this->pin);
				}

				if (interrupt && previousValue != this->value)
				{
					switch (interruptMode)
					{
					case Thing::Core::InterruptMode::OnChange: interrupt->Run(); break;
					case Thing::Core::InterruptMode::WasActivated:
						if (previousValue == Thing::Core::DigitalValue::Low)
							interrupt->Run();
						break;
					case Thing::Core::InterruptMode::WasInactivated:
						if (previousValue == Thing::Core::DigitalValue::High)
							interrupt->Run();
						break;
					}
				}
			}

			Thing::Core::DigitalValue FakeHardware::GPIO::GetValue()
			{
				return this->value;
			}

			void FakeHardware::GPIO::SetAnalogValue(int value)
			{
				this->analogValue = value;
			}

			int FakeHardware::GPIO::GetAnalogValue()
			{
				return this->analogValue;
			}

			void FakeHardware::GPIO::SetInterrupt(Thing::Core::IRunnable * runnable, Thing::Core::InterruptMode mode)
			{
				interrupt = runnable;
				interruptMode = mode;
			}

			void FakeHardware::GPIO::ClearInterrupt()
			{
				interrupt = NULL;
			}
			
			void FakeHardware::GPIO::AddListener(IGPIOListener* listener)
			{
				listeners.push_back(listener);
			}
		}
	}
}
#endif