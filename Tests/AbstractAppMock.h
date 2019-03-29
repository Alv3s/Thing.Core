#pragma once

#include "gmock/gmock.h"
#include "../AbstractApp.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AbstractAppMock : public Thing::Core::AbstractApp
			{
			public:
				MOCK_METHOD0(OnSetup, void());
				MOCK_METHOD0(OnLoop, void());
			};
		}
	}
}