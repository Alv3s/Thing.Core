#pragma once

#include "IDigitalOutput.h"
#include "GenericDigitalOutputBase.h"

namespace Thing
{
	namespace Core
	{
		class GenericInverseDigitalOutput : public GenericDigitalOutputBase<false>
		{
		public: 
			GenericInverseDigitalOutput(int gpio, int code, DigitalValue defaultValue = DigitalValue::Low);
		};
	}
}