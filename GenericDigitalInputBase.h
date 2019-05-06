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
			GenericDigitalInputBase(int gpio) : _gpio(gpio)
			{
				Hardware->ConfigurePin(_gpio, PinMode::Input);
			}

			~GenericDigitalInputBase()
			{

			}

			virtual DigitalValue DigitalRead() override
			{
				return (DigitalValue)((uint8_t)Hardware->DigitalRead(_gpio) ^ LOGIC);
			}

		private:
			const int _gpio;
		};
	}
}