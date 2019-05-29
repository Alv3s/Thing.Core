#include "WiFiBase.h"
#include "Utils.h"

namespace Thing
{
	namespace Core
	{
		void WiFiBase::AddListener(IWiFiListener* listener)
		{
			if (!Utils::Contains(listeners, listener))
				listeners.push_back(listener);
		}

		void WiFiBase::AddListener(IWiFiListener& listener)
		{
			AddListener(&listener);
		}

		void WiFiBase::RemoveListener(IWiFiListener* listener)
		{
			listeners.remove(listener);
		}

		void WiFiBase::RemoveListener(IWiFiListener& listener)
		{
			RemoveListener(&listener);
		}

		void WiFiBase::SignalOnConnect()
		{
			for (auto l : listeners)
				l->OnConnect();
		}

		void WiFiBase::SignalOnDisconnect()
		{
			for (auto l : listeners)
				l->OnDisconnect();
		}

		void WiFiBase::SignalOnFailed()
		{
			for (auto l : listeners)
				l->OnFailed();
		}
	}
}