#pragma once

#include "../IHardware.h"
#include "../ILoggerManager.h"
#include "../ITaskScheduler.h"
#include "../IWiFi.h"
#include "../IAppContainer.h"
#include "../IFileSystem.h"

Thing::Core::ILoggerManager* InitializeLogger();
Thing::Core::IHardware* InitializeHardware();
Thing::Core::ITaskScheduler* InitializeTaskScheduler();
Thing::Core::IWiFi* InitializeWiFi();
Thing::Core::IFileSystem* InitializeFileSystem();
Thing::Core::IAppContainer* InitializeAppContainer();
Thing::Core::IApp* InitializeApp(int argc, char* argv[]);