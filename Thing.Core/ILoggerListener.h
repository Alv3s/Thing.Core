#pragma once
#include <string>
#include "LogLevel.h"

namespace Thing
{
	namespace Core
	{
		class ILoggerListener
		{
		public:
			virtual std::string GetLoggerName() = 0;
			virtual void SetMinLevel(LogLevel minLevel) = 0;
			virtual LogLevel GetMinLevel() = 0;
			virtual void SetMaxLevel(LogLevel maxLevel) = 0;
			virtual LogLevel GetMaxLevel() = 0;

			virtual void OnTrace(std::string message) = 0;
			virtual void OnDebug(std::string message) = 0;
			virtual void OnInfo(std::string message) = 0;
			virtual void OnWarn(std::string message) = 0;
			virtual void OnError(std::string message) = 0;
			virtual void OnFatal(std::string message) = 0;
		};
	}
}