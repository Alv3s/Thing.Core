#include "WiFiBaseTest.h"
#include "IWiFiListenerMock.h"
#include "WiFiBaseMock.h"

using ::testing::Return;
using ::testing::InSequence;

namespace Thing {
	namespace Core {
		namespace Tests {
			void WiFiBaseTest::SetUp()
			{

			}

			void WiFiBaseTest::TearDown()
			{

			}

			TEST_F(WiFiBaseTest, NotifyDigitalInputListenersViaPointer)
			{
				const int totalListeners = 10;
				WiFiBaseMock WiFi;

				IWiFiListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
					WiFi.AddListener(&listeners[i]);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnConnect()).Times(1);
					EXPECT_CALL(listeners[i], OnDisconnect()).Times(0);
				}
				WiFi.SignalOnConnectMock();

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnConnect()).Times(0);
					EXPECT_CALL(listeners[i], OnDisconnect()).Times(1);
				}
				WiFi.SignalOnDisconnectMock();
			}

			TEST_F(WiFiBaseTest, NotifyDigitalInputListenersViaReference)
			{
				const int totalListeners = 10;
				WiFiBaseMock WiFi;

				IWiFiListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
					WiFi.AddListener(listeners[i]);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnConnect()).Times(1);
					EXPECT_CALL(listeners[i], OnDisconnect()).Times(0);
				}
				WiFi.SignalOnConnectMock();

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnConnect()).Times(0);
					EXPECT_CALL(listeners[i], OnDisconnect()).Times(1);
				}
				WiFi.SignalOnDisconnectMock();
			}

			TEST_F(WiFiBaseTest, RemoveDigitalInputListenersViaReference)
			{
				WiFiBaseMock WiFi;

				IWiFiListenerMock listener1;
				EXPECT_CALL(listener1, OnConnect()).Times(1);
				EXPECT_CALL(listener1, OnDisconnect()).Times(0);
				WiFi.AddListener(listener1);

				IWiFiListenerMock listener2;
				EXPECT_CALL(listener2, OnConnect()).Times(1);
				EXPECT_CALL(listener2, OnDisconnect()).Times(1);
				WiFi.AddListener(listener2);

				IWiFiListenerMock listener3;
				EXPECT_CALL(listener3, OnConnect()).Times(2);
				EXPECT_CALL(listener3, OnDisconnect()).Times(1);
				WiFi.AddListener(listener3);

				WiFi.SignalOnConnectMock();
				WiFi.RemoveListener(listener1);
				WiFi.SignalOnDisconnectMock();
				WiFi.RemoveListener(listener2);
				WiFi.SignalOnConnectMock();
			}

			TEST_F(WiFiBaseTest, RemoveDigitalInputListenersViaPointer)
			{
				WiFiBaseMock WiFi;

				IWiFiListenerMock listener1;
				EXPECT_CALL(listener1, OnConnect()).Times(1);
				EXPECT_CALL(listener1, OnDisconnect()).Times(0);
				WiFi.AddListener(&listener1);

				IWiFiListenerMock listener2;
				EXPECT_CALL(listener2, OnConnect()).Times(1);
				EXPECT_CALL(listener2, OnDisconnect()).Times(1);
				WiFi.AddListener(&listener2);

				IWiFiListenerMock listener3;
				EXPECT_CALL(listener3, OnConnect()).Times(2);
				EXPECT_CALL(listener3, OnDisconnect()).Times(1);
				WiFi.AddListener(&listener3);

				WiFi.SignalOnConnectMock();
				WiFi.RemoveListener(&listener1);
				WiFi.SignalOnDisconnectMock();
				WiFi.RemoveListener(&listener2);
				WiFi.SignalOnConnectMock();
			}
		}
	}
}