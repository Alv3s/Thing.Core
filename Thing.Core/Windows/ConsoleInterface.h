#pragma once

#include "../IApp.h"
#include "../ILoggerListener.h"
#include "CLI11.h"
#include <list>
#include "FakeHardware.h"

namespace Thing
{
	namespace Core
	{
		namespace Windows
		{
			class ConsoleInterface : public virtual ILoggerListener, public virtual IAppListener, public virtual IGPIOListener
			{
			public:
				ConsoleInterface();
				~ConsoleInterface();

				std::string GetLoggerName() override;
				void SetMinLevel(LogLevel minLevel) override;
				LogLevel GetMinLevel() override;
				void SetMaxLevel(LogLevel maxLevel) override;
				LogLevel GetMaxLevel() override;

				void OnTrace(std::string message) override;
				void OnDebug(std::string message) override;
				void OnInfo(std::string message) override;
				void OnWarn(std::string message) override;
				void OnError(std::string message) override;
				void OnFatal(std::string message) override;

				bool OnLoop() override;
				void ProcessInput();

				void OnActivating(int gpio) override;
				void OnInactivating(int gpio) override;
			private:
				std::list<std::string> commandHistory;
				int historyCommandIndex;

				CLI::App app;
				bool debug;

				std::string inputCommand;
				std::string commandResponse;

				Thing::Core::LogLevel _minLevel;
				Thing::Core::LogLevel _maxLevel;
				std::string _name;
				
				void clearLine();
				void printTime();
				void printInput();
			};
		}
	}
}