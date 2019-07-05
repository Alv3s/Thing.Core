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
	static Thing::Core::LoggerManager<200> logger;
	static Thing::Core::Arduino::SerialLogger serialLogger("Arduino Serial", 115200);

	serialLogger.SetMinLevel(Thing::Core::LogLevel::Trace);
	logger.AddListener(serialLogger);

	return &logger;
}

Thing::Core::IHardware* InitializeHardware()
{
	static Thing::Core::Arduino::Hardware hardware;
	return &hardware;
}

Thing::Core::ITaskScheduler* InitializeTaskScheduler()
{
	static Thing::Core::AppTaskScheduler scheduler(*AppContainer);
	return &scheduler;
}

Thing::Core::IWiFi* InitializeWiFi()
{
	static Thing::Core::ESP_8266::WiFi wifi;
	return &wifi;
}

Thing::Core::IFileSystem* InitializeFileSystem()
{
	static Thing::Core::ESP::FileSystem fileSystem;
	return &fileSystem;
}


Thing::Core::IAppContainer* InitializeAppContainer()
{
    static Thing::Core::AppContainer container;
	return &container;
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