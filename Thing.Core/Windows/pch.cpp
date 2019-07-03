#ifdef _WIN32
#include "pch.h"
#include "../IoC.h"

#include "../LoggerManager.h"
#include "FakeHardware.h"
#include "../AppTaskScheduler.h"
#include "WindowsWiFi.h"
#include "../AppContainer.h"
#include "FileSystem.h"

Thing::Core::IAppContainer* AppContainer;
Thing::Core::ILoggerManager* Logger;
Thing::Core::IHardware* Hardware;
Thing::Core::ITaskScheduler* TaskScheduler;
Thing::Core::IFileSystem* FileSystem;
Thing::Core::IWiFi* WiFiConnection;


Thing::Core::ILoggerManager* InitializeLogger()
{
	return new Thing::Core::LoggerManager<200>();
}

Thing::Core::IHardware* InitializeHardware()
{
	return new Thing::Core::Windows::FakeHardware();
}

Thing::Core::ITaskScheduler* InitializeTaskScheduler()
{
	return new Thing::Core::AppTaskScheduler(*AppContainer);
}

Thing::Core::IWiFi* InitializeWiFi()
{
	return new Thing::Core::Windows::WiFi();
}

Thing::Core::IFileSystem* InitializeFileSystem()
{
	return new Thing::Core::Windows::FileSystem();
}

Thing::Core::IAppContainer* InitializeAppContainer()
{
	return new Thing::Core::AppContainer();
}
#endif