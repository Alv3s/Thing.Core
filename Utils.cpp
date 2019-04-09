#include "Utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <string>

namespace Thing
{
	namespace Core
	{
		namespace Utils
		{
			std::string IntToString(int value)
			{
				char str[12];
				sprintf(str, "%d", value);
				return std::string(str);
			}

			std::string FormatString(char* buffer, std::string format, ...)
			{
				va_list values;
				va_start(values, format);
				vsprintf(buffer, format.c_str(), values);
				va_end(values);
				return std::string(buffer);
			}
		}
	}
}