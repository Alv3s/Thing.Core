#pragma once

namespace Thing
{
	namespace Core
	{
		class IDigitalOutputMonitor
		{
		public:
			virtual void For(int millis) = 0;
		};
	}
}