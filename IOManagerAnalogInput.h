#pragma once

#include "IAnalogInput.h"
#include <list>

namespace Thing
{
	namespace Core
	{
		class IOManagerAnalogInput
		{
			friend class IOManager;
		public:
			~IOManagerAnalogInput();
		private:
			IOManagerAnalogInput();

			static bool Contains(std::list<IOManagerAnalogInput>& list, IAnalogInput * input);

			IAnalogInput* AnalogInput;
			int LastReadValue;
		};
	}
}
