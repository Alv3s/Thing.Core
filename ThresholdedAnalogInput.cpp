#include "ThresholdedAnalogInput.h"
#include "InputHelpers.h"

#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		ThresholdedAnalogInput::ThresholdedAnalogInput(int gpio, int code, int precision) : _gpio(gpio), _threshold(0), _precision(precision)
		{
			Logger->Debug("Initialize ThresholdedAnalogInput -> Code = %d", code);
			SetCode(code);
		}

		ThresholdedAnalogInput::~ThresholdedAnalogInput()
		{

		}



		void ThresholdedAnalogInput::SetThresholdHigherThan(int value)
		{
			_threshold = value;
			_higherThreshold = true;
		}

		void ThresholdedAnalogInput::SetThresholdLesserThan(int value)
		{
			_threshold = value;
			_higherThreshold = false;
		}



		DigitalValue ThresholdedAnalogInput::DigitalRead()
		{
			return privateDigitalRead();
		}

		unsigned int ThresholdedAnalogInput::GetPrecision()
		{
			return _precision;
		}

		int ThresholdedAnalogInput::AnalogRead()
		{
			return Hardware->AnalogRead(_gpio);
		}

		int ThresholdedAnalogInput::GetCode() const
		{
			return this->_code;
		}

		void ThresholdedAnalogInput::SetCode(int code)
		{
			this->_code = code;
		}


		DigitalValue ThresholdedAnalogInput::privateDigitalRead()
		{
			int readValue = AnalogRead();
			return (_higherThreshold && readValue > _threshold) || (!_higherThreshold && readValue < _threshold) ? DigitalValue::High : DigitalValue::Low;
		}
	}
}