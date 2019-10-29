#pragma once

#include <list>
#include <algorithm>
#include <string>

namespace Thing
{
	namespace Core
	{
		namespace Utils
		{
			template <typename T>
			inline bool Contains(std::list<T>& list, T& element)
			{
				return (std::find(list.begin(), list.end(), element) != list.end());
			}

			std::string IntToString(int value);
			std::string FormatString(char* buffer, std::string format, ...);
		}
	}
}

