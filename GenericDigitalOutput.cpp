#include "GenericDigitalOutput.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		GenericDigitalOutput::GenericDigitalOutput(int gpio, DigitalValue defaultValue) : GenericDigitalOutputBase(gpio, defaultValue)
		{

		}
	}
}