#pragma once
#include "IDigitalInput.h"
#include "HardwareEnumerations.h"
#include "IOManagerDigitalIO.h"

namespace Thing
{
	namespace Core
	{
		class IOManagerDigitalInput : public IOManagerDigitalIO<IDigitalInput>
		{
			friend class IOManager;
		private:
			DigitalValue LastReadValue;
		};
	}
}