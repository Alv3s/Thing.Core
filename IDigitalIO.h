#pragma once

namespace Thing
{
	namespace Core
	{
		class IDigitalIO
		{
		public:
			friend bool operator== (const IDigitalIO& c1, const IDigitalIO& c2)
			{
				return &c1 == &c2;
			}
		};
	}
}