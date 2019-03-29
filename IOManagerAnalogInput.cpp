#include "IOManagerAnalogInput.h"

namespace Thing
{
	namespace Core
	{
		IOManagerAnalogInput::IOManagerAnalogInput()
		{
		}

		IOManagerAnalogInput::~IOManagerAnalogInput()
		{
		}

		bool IOManagerAnalogInput::Contains(std::list<IOManagerAnalogInput>& list, IAnalogInput * input)
		{
			for (auto &item : list)
			{
				if (item.AnalogInput == input)
					return true;
			}
			return false;
		}
	}
}