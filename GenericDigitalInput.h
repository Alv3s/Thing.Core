#pragma once

#include "IDigitalInput.h"
#include "GenericDigitalInputBase.h"

namespace Thing
{
	namespace Core
	{
		class GenericDigitalInput : public GenericDigitalInputBase<false>, public virtual IDigitalInput
		{
		public:
			GenericDigitalInput(int gpio, int code);
		};
	}
}