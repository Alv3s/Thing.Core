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
			GenericInverseDigitalOutput(int gpio, DigitalValue defaultValue = DigitalValue::Low);
		};
	}
}