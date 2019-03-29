#pragma once

namespace Thing
{
	namespace Core
	{
		class IDigitalIO
		{
		public:
			virtual int GetCode() const = 0;
			virtual void SetCode(int code) = 0;
		};
	}
}