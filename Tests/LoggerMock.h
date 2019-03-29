#pragma once

#include "gmock/gmock.h"
#include "../ILogger.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class LoggerMock : public virtual Thing::Core::ILogger
			{
			public:
				MOCK_METHOD2(TraceOverride, void(const char*, va_list));
				MOCK_METHOD2(DebugOverride, void(const char*, va_list));
				MOCK_METHOD2(InfoOverride, void(const char*, va_list));
				MOCK_METHOD2(WarnOverride, void(const char*, va_list));
				MOCK_METHOD2(ErrorOverride, void(const char*, va_list));
				MOCK_METHOD2(FatalOverride, void(const char*, va_list));
			};
		}
	}
}