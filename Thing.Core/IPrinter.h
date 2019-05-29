#pragma once

#include <string>

namespace Thing
{
	namespace Core
	{
		class IPrinter
		{
		public:
			virtual void Print(std::string) = 0;
			virtual void Println(std::string) = 0;

			virtual void Print(int) = 0;
			virtual void Println(int) = 0;

			virtual void Print(char) = 0;
			virtual void Println(char) = 0;

			virtual void Println() = 0;
		};
	}
}