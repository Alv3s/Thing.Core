#pragma once
#include "IDigitalOutput.h"
#include "HardwareEnumerations.h"
#include "IOManagerDigitalIO.h"

namespace Thing
{
	namespace Core
	{
		class IOManagerDigitalOutput : public IOManagerDigitalIO<IDigitalOutput>
		{
			friend class IOManager;
		};
	}
}