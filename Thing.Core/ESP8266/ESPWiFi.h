#pragma once

#include "Thing.Core/WiFiBase.h"
#include "Thing.Core/IRunnable.h"
#include <ESP8266WiFi.h>

class ESP8266WiFi : public Thing::Core::WiFiBase, public virtual Thing::Core::IWiFi, public virtual Thing::Core::IRunnable
{
public:
	virtual void Connect(std::string SSID, std::string password) override;
	virtual void Disconnect() override;
	virtual Thing::Core::WiFiStatus Status() override;
	virtual Thing::Core::IPAddress GetIP() override;
	virtual Thing::Core::MACAddress GetMACAddress() override;

	virtual void Run() override;
private:
	wl_status_t previousStatus;
};