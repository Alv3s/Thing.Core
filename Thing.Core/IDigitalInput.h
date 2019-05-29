#pragma once

#include "IDigitalIO.h"
#include "HardwareEnumerations.h"

namespace Thing
{
	namespace Core
	{
		class IDigitalInput : public virtual IDigitalIO
		{
		public:
			virtual DigitalValue DigitalRead() = 0;
		};
	}
}