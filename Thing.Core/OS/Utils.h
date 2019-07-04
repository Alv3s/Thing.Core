#pragma once

#include <string>
#include "../HardwareEnumerations.h"

inline std::string convertToString(Thing::Core::DigitalValue value)
{
	return value == Thing::Core::DigitalValue::Low ? "Low" : (value == Thing::Core::DigitalValue::High ? "High" : "Toggle");
}