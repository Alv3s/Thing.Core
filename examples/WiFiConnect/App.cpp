#include "App.h"
#include "IoC.h"

App::App()
{

}

App::~App()
{

}

void App::Setup()
{
	//Some logs. You can open your Serial connection with baud rate 115200 to receive this events.
	Logger->Debug("Starting");
	
	//Inform WiFiConnection that we are interested to know the events
	WiFiConnection->AddListener(this);
	//Tell WiFiConnection to connect to the WiFi
    WiFiConnection->Connect("YourSSID", "YourPassword");
}

void App::Loop()
{
}

void App::OnConnect()
{
    Logger->Debug("Connected to WiFi with IP: %s", WiFiConnection->GetIP().ToString().c_str());
}

void App::OnDisconnect()
{
	Logger->Debug("Disconnected from WiFi");
}

void App::OnFailed()
{
	Logger->Debug("Failed to connect to WiFi. Check your credentials.");
}