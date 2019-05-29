#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/IRunnable.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class RunnableMock : public virtual Thing::Core::IRunnable
			{
			public:
				MOCK_METHOD0(Run, void());
			};
		}
	}
}