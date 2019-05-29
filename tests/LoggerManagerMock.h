#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/ILoggerManager.h"
#include "LoggerMock.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class LoggerManagerMock : public virtual Thing::Core::ILoggerManager, public LoggerMock
			{
			public:
				MOCK_METHOD1(AddListener, void(Thing::Core::ILoggerListener*));
				MOCK_METHOD1(AddListener, void(Thing::Core::ILoggerListener&));
				MOCK_METHOD1(RemoveListener, void(Thing::Core::ILoggerListener*));
				MOCK_METHOD1(RemoveListener, void(Thing::Core::ILoggerListener&));
			};
		}
	}
}