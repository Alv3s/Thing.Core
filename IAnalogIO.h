#pragma once

namespace Thing
{
	namespace Core
	{
		class IAnalogIO
		{
		public:
			virtual int GetCode() const = 0;
			virtual void SetCode(int code) = 0;
		};
	}
}