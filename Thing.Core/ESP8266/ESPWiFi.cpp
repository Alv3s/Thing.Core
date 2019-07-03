#ifdef ESP8266
#include "ESPWiFi.h"
#include "../IoC.h"
#include <ESP8266mDNS.h>

void ESP8266WiFi::Connect(std::string SSID, std::string password)
{
	Logger->Debug("Trying to connect WiFi to SSID: %s", SSID.c_str());

	Disconnect();
	WiFi.persistent(false); // This is very important!! Ensures that the ESP board doesn't crash in case of invalid password.
	WiFi.begin(SSID.c_str(), password.c_str());

	TaskScheduler->AttachPeriodic(1000, this);
}

void ESP8266WiFi::Disconnect()
{
	TaskScheduler->Detach(this);
	WiFi.disconnect();
}

Thing::Core::WiFiStatus ESP8266WiFi::Status()
{
	wl_status_t status = WiFi.status();

	switch (status)
	{
	case WL_NO_SHIELD:
		break;
	case WL_IDLE_STATUS:
		break;
	case WL_NO_SSID_AVAIL:
		break;
	case WL_SCAN_COMPLETED:
		break;
	case WL_CONNECTED:
		return Thing::Core::WiFiStatus::Connected;
	case WL_CONNECT_FAILED:
		return Thing::Core::WiFiStatus::ConnectionFailed;
	case WL_CONNECTION_LOST:
		return Thing::Core::WiFiStatus::ConnectionLost;
	case WL_DISCONNECTED:
		return Thing::Core::WiFiStatus::Disconnected;
	}
	return Thing::Core::WiFiStatus::Disconnected;
}

Thing::Core::IPAddress ESP8266WiFi::GetIP()
{
	IPAddress ip = WiFi.localIP();
	return Thing::Core::IPAddress(ip[0], ip[1], ip[2], ip[3]);
}

Thing::Core::MACAddress ESP8266WiFi::GetMACAddress()
{
	uint8_t address[6];
	return Thing::Core::MACAddress(WiFi.macAddress((uint8_t*)address));
}


void ESP8266WiFi::Run()
 {
	yield();

	wl_status_t status = WiFi.status();
	if (previousStatus == status)
		return;

	previousStatus = status;

	switch (status)
	{
	case WL_CONNECTED:
	{
		IPAddress ip = WiFi.localIP();
		Logger->Info("WiFi connected. IP address: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
		SignalOnConnect();
		break;
	}
	case WL_IDLE_STATUS:
	{
		Logger->Info("WiFi is in Iddle Status.");
		break;
	}
	case WL_NO_SSID_AVAIL:
	{
		Logger->Info("WiFi has no SSID Available.");
		break;
	}
	case WL_SCAN_COMPLETED:
	{
		Logger->Info("WiFi scan was completed.");
		break;
	}
	case WL_CONNECT_FAILED:
	{
		Logger->Info("WiFi Connection Failed.");
		SignalOnFailed();
		break;
	}
	case WL_CONNECTION_LOST:
	{
		Logger->Info("WiFi Connection Was Lost.");
		SignalOnDisconnect();
		break;
	}
	default:
		Logger->Info("WiFi status unknown.");
		break;
	}
}
#endif