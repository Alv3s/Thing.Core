#include "GenericInverseDigitalOutput.h"

namespace Thing
{
	namespace Core
	{
		GenericInverseDigitalOutput::GenericInverseDigitalOutput(int gpio, int code, DigitalValue defaultValue) : GenericDigitalOutputBase(gpio, code, defaultValue)
		{
		}
	}
}