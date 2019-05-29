#pragma once

#include "IHardware.h"

namespace Thing
{
	namespace Core
	{
		DigitalInputState DigitalReadToInputState(bool previousValue, bool currentValue);
		DigitalInputState DigitalReadToInputState(DigitalValue previousValue, DigitalValue currentValue);
	}
}