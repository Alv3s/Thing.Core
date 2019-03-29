#pragma once

#include <list>
#include <map>
#include <algorithm>

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

			template <typename T1, typename T2>
			inline bool Contains(std::map<T1, T2>& map, T1& key)
			{
				return map.find(key) != map.end();
			}

			std::string IntToString(int value);
			std::string FormatString(char* buffer, std::string format, ...);
		}
	}
}

