#ifdef _WIN32
#include "../OS/Main.h"
#include "../IoC.h"

#include "../LoggerManager.h"
#include "../OS/FakeHardware.h"
#include "../AppTaskScheduler.h"
#include "../OS/WiFi.h"
#include "../AppContainer.h"
#include "FileSystem.h"
#include "../OS/ConsoleInterface.h"
#include <thread>

Thing::Core::IAppContainer* AppContainer;
Thing::Core::ILoggerManager* Logger;
Thing::Core::IHardware* Hardware;
Thing::Core::ITaskScheduler* TaskScheduler;
Thing::Core::IFileSystem* FileSystem;
Thing::Core::IWiFi* WiFiConnection;


Thing::Core::ILoggerManager* InitializeLogger()
{
	static Thing::Core::LoggerManager<200> logger;
	return &logger;
}

Thing::Core::IHardware* InitializeHardware()
{
	static Thing::Core::OS::FakeHardware hardware;
	return &hardware;
}

Thing::Core::ITaskScheduler* InitializeTaskScheduler()
{
	static Thing::Core::AppTaskScheduler taskScheduler(*AppContainer);
	return &taskScheduler;
}

Thing::Core::IWiFi* InitializeWiFi()
{
	static Thing::Core::OS::WiFi wifi;
	return &wifi;
}

Thing::Core::IFileSystem* InitializeFileSystem()
{
	static Thing::Core::Windows::FileSystem fileSystem;
	return &fileSystem;
}

Thing::Core::IAppContainer* InitializeAppContainer()
{
	static Thing::Core::AppContainer container;
	return &container;
}

Thing::Core::OS::ConsoleInterface ConsoleInterface;
static void InitializeInstances(int argc, char* argv[])
{
	Logger = InitializeLogger();
	ConsoleInterface.SetMinLevel(Thing::Core::LogLevel::Trace);
	Logger->AddListener(ConsoleInterface);

	Logger->Debug("Initializating Instances For Windows");

	Logger->Debug("Initializing App Container...");
	AppContainer = InitializeAppContainer();
	Logger->Debug("App Container Initialized Successfully!");

	Logger->Debug("Initializing Task Scheduler...");
	TaskScheduler = InitializeTaskScheduler();
	Logger->Debug("Task Scheduler Initialized Successfully!");

	Logger->Debug("Initializing Hardware...");
	Hardware = InitializeHardware();
	Logger->Debug("Hardware Initialized Successfully!");

	Logger->Debug("Initializing WiFi...");
	WiFiConnection = InitializeWiFi();
	Logger->Debug("WiFi Initialized Successfully");

	Logger->Debug("Initializing File System...");
	FileSystem = InitializeFileSystem();
	Logger->Debug("File System Initialized Successfully");

	Logger->Debug("Initializing Application");
	AppContainer->SetApp(InitializeApp(argc, argv));
	AppContainer->Setup();
	Logger->Debug("Setup Finished");
}


DWORD WINAPI AppThread(LPVOID lpParameter)
{
	for (;;)
	{
		AppContainer->Loop();
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	return 0;
}

int main(int argc, char* argv[])
{
	InitializeInstances(argc, argv);

	DWORD appThreadId;
	HANDLE appHandle = CreateThread(0, 0, AppThread, NULL, 0, &appThreadId);

	for (;;)
	{
		ConsoleInterface.ProcessInput();
	}

	CloseHandle(appHandle);
	return 0;
}
#endif