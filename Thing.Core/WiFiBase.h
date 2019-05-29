#pragma once

#include "IWiFi.h"
#include <list>

namespace Thing
{
	namespace Core
	{
		class WiFiBase : public virtual IWiFi
		{
		public:
			virtual void AddListener(IWiFiListener* listener) override;
			virtual void AddListener(IWiFiListener& listener) override;

			virtual void RemoveListener(IWiFiListener* listener) override;
			virtual void RemoveListener(IWiFiListener& listener) override;

		protected:
			void SignalOnConnect();
			void SignalOnDisconnect();
			void SignalOnFailed();

		private:
			std::list<IWiFiListener*> listeners;
		};
	}
}