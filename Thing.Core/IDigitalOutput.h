#pragma once

#include "IDigitalIO.h"
#include "HardwareEnumerations.h"

namespace Thing
{
	namespace Core
	{
		class IDigitalOutput : public virtual IDigitalIO
		{
		public:
			virtual void DigitalWrite(DigitalValue value) = 0;
			virtual DigitalValue GetState() = 0;
		};
	}
}