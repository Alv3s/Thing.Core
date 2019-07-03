#ifdef _WIN32

#include "pch.h"
#include "CLI11.h"
#include "../IAppContainer.h"
#include "../IoC.h"
#include <chrono>
#include <thread>
#include "ConsoleInterface.h"

Thing::Core::Windows::ConsoleInterface ConsoleInterface;
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