#pragma once

#include "../IHardware.h"
#include <map>
#include <list>

class GPIO;

namespace Thing
{
	namespace Core
	{
		namespace Windows
		{
			class IGPIOListener {
			public:
				virtual void OnActivating(int gpio) = 0;
				virtual void OnInactivating(int gpio) = 0;
			};

			class FakeHardware : public virtual Thing::Core::IHardware
			{
			public:
				virtual void ConfigurePin(int gpio, Thing::Core::PinMode mode) override;

				void MockInputState(int gpio, Thing::Core::DigitalValue value);
				virtual void DigitalWrite(int gpio, Thing::Core::DigitalValue value) override;
				virtual Thing::Core::DigitalValue DigitalRead(int gpio) override;

				virtual int AnalogRead(int gpio) override;

				virtual unsigned long Millis() override;
				virtual unsigned long Micros() override;
				virtual void Delay(unsigned long millis) override;

				virtual void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable* runnable) override;
				virtual void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable& runnable) override;
				virtual void DetachInterrupt(int gpio) override;

				virtual void Reset() override;

				void SetAnalogValue(int gpio, int value);
				void SetInputState(int gpio, Thing::Core::DigitalValue value);

				void AddListener(IGPIOListener* listener, int gpio);
			private:
				class GPIO
				{
				public:
					GPIO(int pin);

					void SetMode(Thing::Core::PinMode mode);
					Thing::Core::PinMode GetMode();

					void SetValue(Thing::Core::DigitalValue value);
					Thing::Core::DigitalValue GetValue();

					void SetAnalogValue(int value);
					int GetAnalogValue();

					void SetInterrupt(Thing::Core::IRunnable* runnable, Thing::Core::InterruptMode mode);
					void ClearInterrupt();

					void AddListener(IGPIOListener* listener);
				private:
					Thing::Core::IRunnable* interrupt;
					Thing::Core::InterruptMode interruptMode;
					std::list<IGPIOListener*> listeners;

					const int pin;
					Thing::Core::PinMode mode;
					Thing::Core::DigitalValue value;
					int analogValue;
				};

				static std::map<int, GPIO*> gpios;

				void asureGpio(int gpio);
			};
		}
	}
}
