#pragma once

#include "IDigitalInput.h"
#include "GenericDigitalInputBase.h"

namespace Thing
{
	namespace Core
	{
		class GenericInverseDigitalInput : public GenericDigitalInputBase<true>, public virtual IDigitalInput
		{
		public:
			GenericInverseDigitalInput(int gpio, int code);
		};
	}
}