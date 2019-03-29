#pragma once

#include "GenericDigitalOutputBase.h"

namespace Thing
{
	namespace Core
	{
		class GenericDigitalOutput : public GenericDigitalOutputBase<true>
		{
		public:
			GenericDigitalOutput(int gpio, int code, DigitalValue defaultValue = DigitalValue::Low);
		};
	}
}