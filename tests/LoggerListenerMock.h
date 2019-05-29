#pragma once

#include "../Thing.Core/ILoggerListener.h"
#include "gmock/gmock.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class LoggerListenerMock : public Thing::Core::ILoggerListener
			{
			public:
				MOCK_METHOD0(GetLoggerName, std::string());
				MOCK_METHOD1(SetMinLevel, void(Thing::Core::LogLevel));
				MOCK_METHOD0(GetMinLevel, Thing::Core::LogLevel());
				MOCK_METHOD1(SetMaxLevel, void(Thing::Core::LogLevel));
				MOCK_METHOD0(GetMaxLevel, Thing::Core::LogLevel());

				MOCK_METHOD1(OnTrace, void(std::string));
				MOCK_METHOD1(OnDebug, void(std::string));
				MOCK_METHOD1(OnInfo, void(std::string));
				MOCK_METHOD1(OnWarn, void(std::string));
				MOCK_METHOD1(OnError, void(std::string));
				MOCK_METHOD1(OnFatal, void(std::string));
			};
		}
	}
}