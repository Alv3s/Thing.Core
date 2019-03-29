#pragma once

#include "ILogger.h"
#include "ILoggerListener.h"

namespace Thing
{
	namespace Core
	{
		class ILoggerManager : public virtual ILogger
		{
		public:
			virtual void AddListener(ILoggerListener* listener) = 0;
			virtual void AddListener(ILoggerListener& listener) = 0;
			virtual void RemoveListener(ILoggerListener* listener) = 0;
			virtual void RemoveListener(ILoggerListener& listener) = 0;
		};
	}
}