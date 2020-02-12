#pragma once

#include "Thing.Core/IApp.h"
#include "Thing.Core/IWiFi.h"

class App : public virtual Thing::Core::IApp,
			public virtual Thing::Core::IWiFiListener //Our App is a listener to WiFi Events
{
public:
	App();
	~App();

	void Setup() override;
	void Loop() override;
	
	//Our WiFi Events. These Functions will be called automatically
	void OnConnect() override; //This function will be called automatically when connection to WiFi was performed
    void OnDisconnect() override; //This function will be called automatically when disconnected from WiFi
    void OnFailed() override; //This function will be called automatically when connection to WiFi failed to connect
private:
	Thing::Core::IOManager Manager;
};