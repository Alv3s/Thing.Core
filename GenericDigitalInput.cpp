#include "GenericDigitalInput.h"
#include "InputHelpers.h"

#include "Ioc.h"

namespace Thing
{
	namespace Core
	{
		GenericDigitalInput::GenericDigitalInput(int gpio, int code) : _gpio(gpio)
		{
			Logger->Debug("Initialize GenericDigitalInput -> Code = %d", code);

			SetCode(code);

			Hardware->ConfigurePin(_gpio, PinMode::Input);
		}

		GenericDigitalInput::~GenericDigitalInput()
		{

		}

		DigitalValue GenericDigitalInput::DigitalRead()
		{
			return Hardware->DigitalRead(_gpio);
		}

		int GenericDigitalInput::GetCode() const
		{
			return this->_code;
		}

		void GenericDigitalInput::SetCode(int code)
		{
			this->_code = code;
		}
	}
}