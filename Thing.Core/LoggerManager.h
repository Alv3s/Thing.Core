#pragma once

#include "ILoggerManager.h"
#include "ILoggerListener.h"
#include <list>
#include <stdarg.h>
#include <stdio.h>

namespace Thing
{
	namespace Core
	{
		template <int SIZE>
		class LoggerManager : public virtual ILoggerManager
		{
		private:
			std::list<ILoggerListener*> _listeners;
			char buffer[SIZE];

			bool LevelIsEnable(LogLevel level, ILoggerListener* logger)
			{
				int intLevel = static_cast<int>(level);
				return intLevel >= static_cast<int>(logger->GetMinLevel()) && intLevel <= static_cast<int>(logger->GetMaxLevel());
			}
		public:
			void AddListener(ILoggerListener* listener) override
			{
				if (listener)
					this->_listeners.push_back(listener);
			}

			void AddListener(ILoggerListener& listener) override
			{
				AddListener(&listener);
			}

			void RemoveListener(ILoggerListener* listener) override
			{
				if (listener)
					_listeners.remove(listener);
			}

			void RemoveListener(ILoggerListener& listener) override
			{
				RemoveListener(&listener);
			}

			void TraceOverride(const char* message, va_list args) override
			{
				vsprintf(buffer, message, args);

				for (auto listener : this->_listeners)
					if (this->LevelIsEnable(LogLevel::Trace, listener))
						listener->OnTrace(buffer);
			}

			void DebugOverride(const char* message, va_list args) override
			{
				vsprintf(buffer, message, args);

				for (auto listener : this->_listeners)
					if (this->LevelIsEnable(LogLevel::Debug, listener))
						listener->OnDebug(buffer);
			}

			void InfoOverride(const char* message, va_list args) override
			{
				vsprintf(buffer, message, args);

				for (auto listener : this->_listeners)
					if (this->LevelIsEnable(LogLevel::Info, listener))
						listener->OnInfo(buffer);
			}

			void WarnOverride(const char* message, va_list args) override
			{
				vsprintf(buffer, message, args);

				for (auto listener : this->_listeners)
					if (this->LevelIsEnable(LogLevel::Warn, listener))
						listener->OnWarn(buffer);
			}

			void ErrorOverride(const char* message, va_list args) override
			{
				vsprintf(buffer, message, args);

				for (auto listener : this->_listeners)
					if (this->LevelIsEnable(LogLevel::Error, listener))
						listener->OnError(buffer);
			}

			void FatalOverride(const char* message, va_list args) override
			{
				vsprintf(buffer, message, args);

				for (auto listener : this->_listeners)
					if (this->LevelIsEnable(LogLevel::Fatal, listener))
						listener->OnFatal(buffer);
			}
		};
	}
}