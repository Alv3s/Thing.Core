#ifdef ESP8266
#include "../Arduino/Main.h"
#include "../IoC.h"

#include "../IHardware.h"
#include "../ILogger.h"
#include "../ITaskScheduler.h"

#include "../LoggerManager.h"
#include "../LogLevel.h"
#include "../Arduino/SerialLogger.h"
#include "../Arduino/Hardware.h"
#include "../AppTaskScheduler.h"
#include "WiFi.h"
#include "../ESP/FileSystem.h"

#include "../AppContainer.h"

Thing::Core::IAppContainer* AppContainer;
Thing::Core::ILoggerManager* Logger;
Thing::Core::IHardware* Hardware;
Thing::Core::ITaskScheduler* TaskScheduler;
Thing::Core::IWiFi* WiFiConnection;
Thing::Core::IFileSystem* FileSystem;

Thing::Core::ILoggerManager* InitializeLogger()
{
	Thing::Core::ILoggerManager* logger = new Thing::Core::LoggerManager<200>();

	Thing::Core::Arduino::SerialLogger* serialLogger = new Thing::Core::Arduino::SerialLogger("Arduino Serial", 115200);
	serialLogger->SetMinLevel(Thing::Core::LogLevel::Trace);
	logger->AddListener(serialLogger);

	return logger;
}

Thing::Core::IHardware* InitializeHardware()
{
	Thing::Core::Arduino::Hardware* hardware = new Thing::Core::Arduino::Hardware();
	return hardware;
}

Thing::Core::ITaskScheduler* InitializeTaskScheduler()
{
	Thing::Core::AppTaskScheduler* scheduler = new Thing::Core::AppTaskScheduler(*AppContainer);
	return scheduler;
}

Thing::Core::IWiFi* InitializeWiFi()
{
	Thing::Core::ESP_8266::WiFi* wifi = new Thing::Core::ESP_8266::WiFi();
	return wifi;
}

Thing::Core::IFileSystem* InitializeFileSystem()
{
	return new Thing::Core::ESP::FileSystem();
}


Thing::Core::IAppContainer* InitializeAppContainer()
{
    return new Thing::Core::AppContainer();
}

extern "C" void setup() 
{
	Logger = InitializeLogger();
#ifdef INIT_DEBUG
	Logger->Debug("\nArduino Initialization");
	Logger->Debug("Initializing Global Instances...");
#endif

#ifdef INIT_DEBUG
	Logger->Debug("Initializing App Container...");
#endif
    AppContainer = InitializeAppContainer();
#ifdef INIT_DEBUG
	Logger->Debug("App Container Initialized Successfully!");
#endif

#ifdef INIT_DEBUG
	Logger->Debug("Initializing Task Scheduler...");
#endif
	TaskScheduler = InitializeTaskScheduler();
#ifdef INIT_DEBUG
	Logger->Debug("Task Scheduler Initialized Successfully!");
#endif
	
#ifdef INIT_DEBUG
	Logger->Debug("Initializing Hardware...");
#endif
    Hardware = InitializeHardware();
#ifdef INIT_DEBUG
	Logger->Debug("Hardware Initialized Successfully!");
#endif

#ifdef INIT_DEBUG
	Logger->Debug("Initializing WiFi...");
#endif
    WiFiConnection = InitializeWiFi();
#ifdef INIT_DEBUG
	Logger->Debug("WiFi Initialized Successfully");
#endif

#ifdef INIT_DEBUG
	Logger->Debug("Initializing File System...");
#endif
    FileSystem = InitializeFileSystem();
#ifdef INIT_DEBUG
	Logger->Debug("File System Initialized Successfully");
#endif


#ifdef INIT_DEBUG
	Logger->Debug("Initializing WiFi...");
#endif
    WiFiConnection = InitializeWiFi();
#ifdef INIT_DEBUG
	Logger->Debug("WiFi Initialized Successfully");
#endif

	Logger->Debug("Initializing Application");
	AppContainer->SetApp(InitializeApp());
	AppContainer->Setup();
	Logger->Debug("Setup Finished");
}

extern "C" void loop() {
	AppContainer->Loop();
}
#endif