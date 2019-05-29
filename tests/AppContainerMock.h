#pragma once

#include "gmock/gmock.h"
#include "../Thing.Core/IAppContainer.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class AppContainerMock : public virtual IAppContainer
			{
			public:
				MOCK_METHOD1(SetApp, void(IApp*));
				MOCK_METHOD1(SetApp, void(IApp&));

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