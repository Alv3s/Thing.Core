#include "GenericInverseDigitalOutput.h"

namespace Thing
{
	namespace Core
	{
		GenericInverseDigitalOutput::GenericInverseDigitalOutput(int gpio, DigitalValue defaultValue) : GenericDigitalOutputBase(gpio, defaultValue)
		{
		}
	}
}