#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/IApp.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AppMock : public virtual IApp
			{
			public:
				MOCK_METHOD0(Setup, void());
				MOCK_METHOD0(Loop, void());
			};
		}
	}
}