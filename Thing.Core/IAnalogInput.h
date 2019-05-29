#pragma once

#include "IAnalogIO.h"

namespace Thing
{
	namespace Core
	{
		class IAnalogInput : public virtual IAnalogIO
		{
		public:
			virtual unsigned int GetPrecision() = 0;
			virtual int AnalogRead() = 0;
		};
	}
}