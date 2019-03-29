#include "AbstractAppTest.h"
#include "IAppListenerMock.h"

using ::testing::Return;
using ::testing::_;

namespace Thing {
	namespace Core {
		namespace Tests {
			void AbstractAppTest::SetUp()
			{
			}

			void AbstractAppTest::TearDown()
			{
			}

			TEST_F(AbstractAppTest, OnSetupCall)
			{
				EXPECT_CALL(App, OnSetup()).Times(1);
				App.Setup();
			}

			TEST_F(AbstractAppTest, OnLoopCall)
			{
				EXPECT_CALL(App, OnLoop()).Times(1);
				App.Loop();
			}

			TEST_F(AbstractAppTest, NotifyListenersViaReference)
			{
				const int totalListeners = 10;
				IAppListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(false));
					App.AddListener(listeners[i]);
				}

				App.Loop();
				for (int i = 0; i < totalListeners; ++i)
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(false));
				App.Loop();
			}

			TEST_F(AbstractAppTest, NotifyListenersViaPointer)
			{
				const int totalListeners = 10;
				IAppListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(false));
					App.AddListener(&listeners[i]);
				}

				App.Loop();
				for (int i = 0; i < totalListeners; ++i)
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(false));
				App.Loop();
			}

			TEST_F(AbstractAppTest, RemoveListenerViaReference)
			{
				IAppListenerMock listener1;
				EXPECT_CALL(listener1, OnLoop()).Times(1).WillRepeatedly(Return(false));
				App.AddListener(listener1);

				IAppListenerMock listener3;
				EXPECT_CALL(listener3, OnLoop()).Times(3).WillRepeatedly(Return(false));
				App.AddListener(listener3);

				IAppListenerMock listener2;
				EXPECT_CALL(listener2, OnLoop()).Times(2).WillRepeatedly(Return(false));
				App.AddListener(listener2);

				App.Loop();
				App.RemoveListener(listener1);
				App.Loop();
				App.RemoveListener(listener2);
				App.Loop();
			}

			TEST_F(AbstractAppTest, RemoveListenerViaPointer)
			{
				IAppListenerMock listener1;
				EXPECT_CALL(listener1, OnLoop()).Times(1).WillRepeatedly(Return(false));
				App.AddListener(&listener1);

				IAppListenerMock listener3;
				EXPECT_CALL(listener3, OnLoop()).Times(3).WillRepeatedly(Return(false));
				App.AddListener(&listener3);

				IAppListenerMock listener2;
				EXPECT_CALL(listener2, OnLoop()).Times(2).WillRepeatedly(Return(false));
				App.AddListener(&listener2);

				App.Loop();
				App.RemoveListener(&listener1);
				App.Loop();
				App.RemoveListener(&listener2);
				App.Loop();
			}

			TEST_F(AbstractAppTest, ListenerRemovedOnLoop)
			{
				const int totalListeners = 10;
				IAppListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners / 2; ++i)
				{
					EXPECT_CALL(listeners[i], OnLoop()).Times(2).WillRepeatedly(Return(false));
					App.AddListener(listeners[i]);
				}
				for (int i = totalListeners / 2; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnLoop()).Times(1).WillRepeatedly(Return(true));
					App.AddListener(listeners[i]);
				}

				App.Loop();
				App.Loop();
			}
		}
	}
}