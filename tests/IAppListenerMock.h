#pragma once

#include "gmock/gmock.h"
#include "../IApp.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class IAppListenerMock : public virtual Thing::Core::IAppListener
			{
			public:
				MOCK_METHOD0(OnLoop, bool());
			};
		}
	}
}