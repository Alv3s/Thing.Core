#include "GenericAnalogInput.h"
#include "Ioc.h"

namespace Thing
{
	namespace Core
	{
		GenericAnalogInput::GenericAnalogInput(int gpio, int precision) : _gpio(gpio), _precision(precision)
		{
			Hardware->ConfigurePin(_gpio, PinMode::Input);
		}

		GenericAnalogInput::~GenericAnalogInput()
		{
		}


		unsigned int GenericAnalogInput::GetPrecision()
		{
			return _precision;
		}

		int GenericAnalogInput::AnalogRead()
		{
			return Hardware->AnalogRead(_gpio);
		}
	}
}