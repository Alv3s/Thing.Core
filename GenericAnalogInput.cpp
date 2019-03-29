#include "GenericAnalogInput.h"
#include "Ioc.h"

namespace Thing
{
	namespace Core
	{
		GenericAnalogInput::GenericAnalogInput(int gpio, int code, int precision) : _gpio(gpio), _precision(precision)
		{
			Logger->Debug("Initialize GenericAnalogInput -> Code = %d", code);

			SetCode(code);

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

		int GenericAnalogInput::GetCode() const
		{
			return this->_code;
		}

		void GenericAnalogInput::SetCode(int code)
		{
			this->_code = code;
		}
	}
}