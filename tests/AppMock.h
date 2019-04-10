#pragma once

#include "gmock/gmock.h"
#include "../IApp.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AppMock : public virtual IApp
			{
			public:
				MOCK_METHOD1(AddListener, void(IAppListener*));
				MOCK_METHOD1(AddListener, void(IAppListener&));

				MOCK_METHOD1(RemoveListener, void(IAppListener*));
				MOCK_METHOD1(RemoveListener, void(IAppListener&));

				MOCK_METHOD0(Setup, void());
				MOCK_METHOD0(Loop, void());
			};
		}
	}
}