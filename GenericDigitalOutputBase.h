#pragma once

#include "IDigitalOutput.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		template <bool LOGIC>
		class GenericDigitalOutputBase : public virtual IDigitalOutput
		{
		public:
			GenericDigitalOutputBase(int gpio, DigitalValue defaultValue = DigitalValue::Low) : _gpio(gpio)
			{
				Hardware->ConfigurePin(_gpio, PinMode::Output);
				DigitalWrite(defaultValue);
			}

			virtual ~GenericDigitalOutputBase()
			{

			}

			virtual void DigitalWrite(DigitalValue value) override
			{
				_currentValue = value == DigitalValue::Toggle ? !_currentValue : (bool)value;
				Hardware->DigitalWrite(_gpio, _currentValue == LOGIC ? DigitalValue::High : DigitalValue::Low);
			}

			virtual DigitalValue GetState() override
			{
				return (DigitalValue)_currentValue;
			}

		private:
			const int _gpio;

			bool _currentValue;
		};
	}
}
