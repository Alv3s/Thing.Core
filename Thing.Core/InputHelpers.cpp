#include "InputHelpers.h"

namespace Thing
{
	namespace Core
	{
		DigitalInputState DigitalReadToInputState(bool previousValue, bool currentValue)
		{
			DigitalInputState result = DigitalInputState::Inactive;
			if (previousValue)
				result = (currentValue ? DigitalInputState::Active : DigitalInputState::WasInactivated);
			else
				result = (currentValue ? DigitalInputState::WasActivated : DigitalInputState::Inactive);
			return result;
		}

		DigitalInputState DigitalReadToInputState(DigitalValue previousValue, DigitalValue currentValue)
		{
			DigitalInputState result = DigitalInputState::Inactive;
			if (previousValue == DigitalValue::High)
				result = (currentValue == DigitalValue::High ? DigitalInputState::Active : DigitalInputState::WasInactivated);
			else
				result = (currentValue == DigitalValue::High ? DigitalInputState::WasActivated : DigitalInputState::Inactive);
			return result;
		}
	}
}