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
			GenericDigitalOutputBase(int gpio, int code, DigitalValue defaultValue = DigitalValue::Low) : _gpio(gpio)
			{
				Logger->Debug("Initialize GenericDigitalOutput -> Code = %d", code);

				Hardware->ConfigurePin(_gpio, PinMode::Output);
				DigitalWrite(defaultValue);
				SetCode(code);
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

			virtual int GetCode() const override
			{
				return this->_code;
			}

			virtual void SetCode(int code) override
			{
				this->_code = code;
			}

		private:
			const int _gpio;
			int _code;

			bool _currentValue;
		};
	}
}
