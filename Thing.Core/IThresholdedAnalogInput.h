#pragma once

#include "IDigitalInput.h"
#include "IAnalogInput.h"

namespace Thing
{
	namespace Core
	{
		class IThresholdedAnalogInput : public virtual IDigitalInput, public virtual IAnalogInput
		{

		};
	}
}