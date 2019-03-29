#pragma once

namespace Thing
{
	namespace Core
	{
		class IDigitalIOListener
		{
		public:
			virtual void OnActivating(int code) = 0;
			virtual void OnActivating(int code, unsigned int count) = 0;
			virtual void OnInactivating(int code) = 0;
			virtual void OnInactivating(int code, unsigned int count) = 0;
		};

		class IDigitalInputListener : public virtual IDigitalIOListener
		{
		};

		class IDigitalOutputListener : public virtual IDigitalIOListener
		{
		};
	}
}