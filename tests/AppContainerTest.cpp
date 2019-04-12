#include "AppContainerTest.h"
#include "IAppListenerMock.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			void AppContainerTest::SetUp()
			{
				AppContainer.SetApp(App);
			}

			void AppContainerTest::TearDown()
			{
			}

			TEST_F(AppContainerTest, SetupCall)
			{
				EXPECT_CALL(App, Setup()).Times(1);
				AppContainer.Setup();
			}

			TEST_F(AppContainerTest, OnLoopCall)
			{
				EXPECT_CALL(App, Loop()).Times(1);
				AppContainer.Loop();
			}

			TEST_F(AppContainerTest, NotifyListenersViaReference)
			{
				const int totalListeners = 10;
				IAppListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(false));
					AppContainer.AddListener(listeners[i]);
				}

				EXPECT_CALL(App, Loop()).Times(2);
				AppContainer.Loop();
				for (int i = 0; i < totalListeners; ++i)
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(false));
				AppContainer.Loop();
			}

			TEST_F(AppContainerTest, NotifyListenersViaPointer)
			{
				const int totalListeners = 10;
				IAppListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(false));
					AppContainer.AddListener(&listeners[i]);
				}

				EXPECT_CALL(App, Loop()).Times(2);
				AppContainer.Loop();
				for (int i = 0; i < totalListeners; ++i)
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(false));
				AppContainer.Loop();
			}

			TEST_F(AppContainerTest, RemoveListenerViaReference)
			{
				IAppListenerMock listener1;
				EXPECT_CALL(listener1, OnLoop()).Times(1).WillRepeatedly(Return(false));
				AppContainer.AddListener(listener1);

				IAppListenerMock listener3;
				EXPECT_CALL(listener3, OnLoop()).Times(3).WillRepeatedly(Return(false));
				AppContainer.AddListener(listener3);

				IAppListenerMock listener2;
				EXPECT_CALL(listener2, OnLoop()).Times(2).WillRepeatedly(Return(false));
				AppContainer.AddListener(listener2);

				EXPECT_CALL(App, Loop()).Times(3);
				AppContainer.Loop();
				AppContainer.RemoveListener(listener1);
				AppContainer.Loop();
				AppContainer.RemoveListener(listener2);
				AppContainer.Loop();
			}

			TEST_F(AppContainerTest, RemoveListenerViaPointer)
			{
				IAppListenerMock listener1;
				EXPECT_CALL(listener1, OnLoop()).Times(1).WillRepeatedly(Return(false));
				AppContainer.AddListener(&listener1);

				IAppListenerMock listener3;
				EXPECT_CALL(listener3, OnLoop()).Times(3).WillRepeatedly(Return(false));
				AppContainer.AddListener(&listener3);

				IAppListenerMock listener2;
				EXPECT_CALL(listener2, OnLoop()).Times(2).WillRepeatedly(Return(false));
				AppContainer.AddListener(&listener2);

				EXPECT_CALL(App, Loop()).Times(3);
				AppContainer.Loop();
				AppContainer.RemoveListener(&listener1);
				AppContainer.Loop();
				AppContainer.RemoveListener(&listener2);
				AppContainer.Loop();
			}

			TEST_F(AppContainerTest, ListenerRemovedOnLoop)
			{
				const int totalListeners = 10;
				IAppListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners / 2; ++i)
				{
					EXPECT_CALL(listeners[i], OnLoop()).Times(2).WillRepeatedly(Return(false));
					AppContainer.AddListener(listeners[i]);
				}
				for (int i = totalListeners / 2; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(true));
					AppContainer.AddListener(listeners[i]);
				}

				EXPECT_CALL(App, Loop()).Times(2);
				AppContainer.Loop();
				AppContainer.Loop();
			}
		}
	}
}