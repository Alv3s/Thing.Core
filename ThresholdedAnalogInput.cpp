#include "ThresholdedAnalogInput.h"
#include "InputHelpers.h"

#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		ThresholdedAnalogInput::ThresholdedAnalogInput(int gpio, int precision) : _gpio(gpio), _threshold(0), _precision(precision)
		{
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

		DigitalValue ThresholdedAnalogInput::privateDigitalRead()
		{
			int readValue = AnalogRead();
			return (_higherThreshold && readValue > _threshold) || (!_higherThreshold && readValue < _threshold) ? DigitalValue::High : DigitalValue::Low;
		}
	}
}