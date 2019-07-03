// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <windows.h>

#include "../IHardware.h"
#include "../ILoggerManager.h"
#include "../ITaskScheduler.h"
#include "../IWiFi.h"
#include "../IAppContainer.h"

Thing::Core::ILoggerManager* InitializeLogger();
Thing::Core::IHardware* InitializeHardware();
Thing::Core::ITaskScheduler* InitializeTaskScheduler();
Thing::Core::IWiFi* InitializeWiFi();
Thing::Core::IAppContainer* InitializeAppContainer();
Thing::Core::IApp* InitializeApp();

inline std::string convertToString(Thing::Core::DigitalValue value)
{
	return value == Thing::Core::DigitalValue::Low ? "Low" : (value == Thing::Core::DigitalValue::High ? "High" : "Toggle");
}
#endif //PCH_H
