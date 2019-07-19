#pragma once

#include "../IHardware.h"
#include <map>
#include <list>
#include <chrono>
#include "../IoC.h"
#include "Utils.h"


#ifdef linux
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

class GPIO;

namespace Thing
{
	namespace Core
	{
		namespace OS
		{
			class IGPIOListener {
			public:
				virtual void OnActivating(int gpio) = 0;
				virtual void OnInactivating(int gpio) = 0;
			};

			class FakeHardware : public virtual Thing::Core::IHardware
			{
			public:
				FakeHardware()
				{
					auto now = std::chrono::system_clock::now();
					auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
					startedMillis = now_ms.time_since_epoch().count();

					auto now_us = std::chrono::time_point_cast<std::chrono::microseconds>(now);
					startedMicros = now_us.time_since_epoch().count();
				}

				void ConfigurePin(int gpio, Thing::Core::PinMode mode) override
				{
					asureGpio(gpio);
					gpios[gpio]->SetMode(mode);
				}

				void MockInputState(int gpio, Thing::Core::DigitalValue value)
				{
					if (gpios.count(gpio) == 0)
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

				void DigitalWrite(int gpio, Thing::Core::DigitalValue value) override
				{
					asureGpio(gpio);
					if (gpios[gpio]->GetMode() == Thing::Core::PinMode::Input)
					{
						Logger->Error("Attempting to Write to Input Pin %d to level %s.", gpio, convertToString(value).c_str());
						return;
					}
					gpios[gpio]->SetValue(value);
				}

				Thing::Core::DigitalValue DigitalRead(int gpio) override
				{
					asureGpio(gpio);
					return gpios[gpio]->GetValue();
				}

				int AnalogRead(int gpio) override
				{
					asureGpio(gpio);
					return gpios[gpio]->GetAnalogValue();
				}

				unsigned long Millis() override
				{
					auto now = std::chrono::system_clock::now();
					auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
					return now_ms.time_since_epoch().count() - startedMillis;
				}

				unsigned long Micros() override
				{
					auto now = std::chrono::system_clock::now();
					auto now_us = std::chrono::time_point_cast<std::chrono::microseconds>(now);
					return now_us.time_since_epoch().count() - startedMicros;
				}

				void Delay(unsigned long millis) override
				{
#ifdef linux
					usleep(sleepMs * 1000);
#endif
#ifdef _WIN32
					Sleep(millis);
#endif
				}

				void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable* runnable) override
				{
					asureGpio(gpio);
					gpios[gpio]->SetInterrupt(runnable, mode);
				}

				void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable& runnable) override
				{
					AttachInterrupt(gpio, mode, &runnable);
				}

				void DetachInterrupt(int gpio) override
				{
					asureGpio(gpio);
					gpios[gpio]->ClearInterrupt();
				}

				void Reset() override
				{
					//TODO: Implement Reset. This should restart program
				}

				void SetAnalogValue(int gpio, int value)
				{
					if (gpios.count(gpio) == 0)
						return;

					if (gpios[gpio]->GetMode() != Thing::Core::PinMode::Input)
						return;

					gpios[gpio]->SetAnalogValue(value);
				}

				void SetInputState(int gpio, Thing::Core::DigitalValue value)
				{
					if (gpios.count(gpio) == 0)
						return;

					if (gpios[gpio]->GetMode() != Thing::Core::PinMode::Input)
						return;

					gpios[gpio]->SetValue(value);
				}

				void AddListener(IGPIOListener* listener, int gpio)
				{
					asureGpio(gpio);
					gpios[gpio]->AddListener(listener);
				}
			private:
				class GPIO
				{
				public:
					GPIO(int pin) : pin(pin), value(Thing::Core::DigitalValue::Low), interrupt(NULL)
					{
					}


					void SetMode(Thing::Core::PinMode mode)
					{
						this->mode = mode;
					}
					
					Thing::Core::PinMode GetMode()
					{
						return mode;
					}

					void SetValue(Thing::Core::DigitalValue value)
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

					Thing::Core::DigitalValue GetValue()
					{
						return this->value;
					}

					void SetAnalogValue(int value)
					{
						this->analogValue = value;
					}

					int GetAnalogValue()
					{
						return this->analogValue;
					}

					void SetInterrupt(Thing::Core::IRunnable* runnable, Thing::Core::InterruptMode mode)
					{
						interrupt = runnable;
						interruptMode = mode;
					}

					void ClearInterrupt()
					{
						interrupt = NULL;
					}

					void AddListener(IGPIOListener* listener)
					{
						listeners.push_back(listener);
					}
				private:
					Thing::Core::IRunnable* interrupt;
					Thing::Core::InterruptMode interruptMode;
					std::list<IGPIOListener*> listeners;

					const int pin;
					Thing::Core::PinMode mode;
					Thing::Core::DigitalValue value;
					int analogValue;
				};

				unsigned long startedMillis;
				unsigned long startedMicros;
				static std::map<int, GPIO*> gpios;

				void asureGpio(int gpio)
				{
					if (gpios.count(gpio) == 0)
					{
						GPIO* obj = new GPIO(gpio);
						gpios.insert(std::pair<int, GPIO*>(gpio, obj));
					}
				}
			};

			std::map<int, FakeHardware::GPIO*> FakeHardware::gpios;
		}
	}
}
