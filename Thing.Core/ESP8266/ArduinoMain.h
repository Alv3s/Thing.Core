#pragma once

#include "Thing.Core/IHardware.h"
#include "Thing.Core/ILoggerManager.h"
#include "Thing.Core/ITaskScheduler.h"
#include "Thing.Core/IWiFi.h"
#include "Thing.Core/IAppContainer.h"

Thing::Core::ILoggerManager* InitializeLogger();
Thing::Core::IHardware* InitializeHardware();
Thing::Core::ITaskScheduler* InitializeTaskScheduler();
Thing::Core::IWiFi* InitializeWiFi();
Thing::Core::IAppContainer* InitializeAppContainer();
Thing::Core::IApp* InitializeApp();