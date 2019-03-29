#include "GenericDigitalOutput.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		GenericDigitalOutput::GenericDigitalOutput(int gpio, int code, DigitalValue defaultValue) : GenericDigitalOutputBase(gpio, code, defaultValue)
		{

		}
	}
}