#pragma once

#include "IDigitalInput.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		template <bool LOGIC>
		class GenericDigitalInputBase : public virtual IDigitalInput
		{
		public:
			GenericDigitalInputBase(int gpio, int code) : _gpio(gpio)
			{
				Logger->Debug("Initialize GenericDigitalInput -> Code = %d", code);

				SetCode(code);

				Hardware->ConfigurePin(_gpio, PinMode::Input);
			}

			~GenericDigitalInputBase()
			{

			}

			virtual DigitalValue DigitalRead() override
			{
				return (DigitalValue)((uint8_t)Hardware->DigitalRead(_gpio) ^ LOGIC);
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
		};
	}
}