#pragma once

#include "IDigitalIO.h"

namespace Thing
{
	namespace Core
	{
		class IDigitalIOListener
		{
		public:
			virtual void OnActivating(IDigitalIO* io, unsigned int count) = 0;
			virtual void OnInactivating(IDigitalIO* io, unsigned int count) = 0;
		};

		class IDigitalInputListener : public virtual IDigitalIOListener
		{
		};

		class IDigitalOutputListener : public virtual IDigitalIOListener
		{
		};
	}
}