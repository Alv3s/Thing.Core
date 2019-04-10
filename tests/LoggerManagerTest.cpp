#include "LoggerManagerTest.h"
#include "LoggerListenerMock.h"

using ::testing::Return;
using ::testing::_;
using ::testing::StrCaseEq;

namespace Thing {
	namespace Core {
		namespace Tests {
			void LoggerManagerTest::SetUp()
			{
			}

			void LoggerManagerTest::TearDown()
			{
			}

			#pragma region AddListener Tests

			TEST_F(LoggerManagerTest, AddListener_ViaPointer)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnTrace(logMsgStr)).Times(1);

				// Act
				loggerManager.Trace(logMsgStr.c_str());
				loggerManager.AddListener(&listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, AddListener_ViaPointer_NullListener)
			{
				// Mock
				LoggerListenerMock *listenerMock = NULL;

				// Act
				loggerManager.Trace(logMsgStr.c_str());
				loggerManager.AddListener(listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, AddListener_ViaReference)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnTrace(logMsgStr)).Times(1);

				// Act
				loggerManager.Trace(logMsgStr.c_str());
				loggerManager.AddListener(listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, AddListener_ViaReference_NullListener)
			{
				// Mock
				LoggerListenerMock *listenerMock = NULL;

				// Act
				loggerManager.Trace(logMsgStr.c_str());
				loggerManager.AddListener(*listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			#pragma endregion

			#pragma region RemoveListener Tests

			TEST_F(LoggerManagerTest, RemoveListener_ViaPointer)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, OnTrace(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(&listenerMock);
				loggerManager.RemoveListener(&listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, RemoveListener_ViaPointer_NullListener)
			{
				// Mock
				LoggerListenerMock* listenerMock = NULL;

				// Act
				loggerManager.RemoveListener(listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, RemoveListener_ViaReference)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, OnTrace(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.RemoveListener(listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, RemoveListener_ViaReference_NullListener)
			{
				// Mock
				LoggerListenerMock* listenerMock = NULL;

				// Act
				loggerManager.RemoveListener(*listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			#pragma endregion

			#pragma region Trace Tests

			TEST_F(LoggerManagerTest, Trace_LevelEnabled)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, OnTrace(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Trace_MinLevelGreaterThanTrace)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnTrace(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Trace(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Trace_MaxLevelLowerThanDebug)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, OnTrace(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Debug(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Trace_TwoListeners)
			{
				// Mock
				LoggerListenerMock listenerMock1;
				LoggerListenerMock listenerMock2;
				EXPECT_CALL(listenerMock1, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock1, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock1, OnTrace(logMsgStr)).Times(1);
				EXPECT_CALL(listenerMock2, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock2, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock2, OnTrace(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock1);
				loggerManager.AddListener(listenerMock2);
				loggerManager.Trace(logMsgStr.c_str());
			}

			/// <summary>
			/// Tests the formatting sent to a Log Listener with Trace.
			/// </summary>
			TEST_F(LoggerManagerTest, Trace_TestFormat)
			{
				const std::string fomartInput = "This is a test: %s %d stars!";
				const char* input1 = "IoT";
				const int input2 = 5;
				const std::string expectedResult = "This is a test: IoT 5 stars!";

				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, OnTrace(expectedResult)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Trace(fomartInput.c_str(), input1, input2);
			}
			#pragma endregion

			#pragma region Debug Tests

			TEST_F(LoggerManagerTest, Debug_LevelEnabled)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Debug));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Debug));
				EXPECT_CALL(listenerMock, OnDebug(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Debug(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Debug_MinLevelGreaterThanDebug)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnDebug(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Debug(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Debug_MaxLevelLowerThanDebug)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, OnDebug(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Debug(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Debug_TwoListeners)
			{
				// Mock
				LoggerListenerMock listenerMock1;
				LoggerListenerMock listenerMock2;
				EXPECT_CALL(listenerMock1, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock1, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Debug));
				EXPECT_CALL(listenerMock1, OnDebug(logMsgStr)).Times(1);
				EXPECT_CALL(listenerMock2, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock2, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Debug));
				EXPECT_CALL(listenerMock2, OnDebug(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock1);
				loggerManager.AddListener(listenerMock2);
				loggerManager.Debug(logMsgStr.c_str());
			}

			/// <summary>
			/// Tests the formatting sent to a Log Listener with Debug.
			/// </summary>
			TEST_F(LoggerManagerTest, Debug_TestFormat)
			{
				const std::string fomartInput = "This is a test: %s %d stars!";
				const char* input1 = "IoT";
				const int input2 = 5;
				const std::string expectedResult = "This is a test: IoT 5 stars!";

				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Debug));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Debug));
				EXPECT_CALL(listenerMock, OnDebug(expectedResult)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Debug(fomartInput.c_str(), input1, input2);
			}
			#pragma endregion

			#pragma region Info Tests

			TEST_F(LoggerManagerTest, Info_LevelEnabled)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Info));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Info));
				EXPECT_CALL(listenerMock, OnInfo(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Info(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Info_MinLevelGreaterThanInfo)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnInfo(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Info(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Info_MaxLevelLowerThanInfo)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, OnInfo(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Info(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Info_TwoListeners)
			{
				// Mock
				LoggerListenerMock listenerMock1;
				LoggerListenerMock listenerMock2;
				EXPECT_CALL(listenerMock1, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock1, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Info));
				EXPECT_CALL(listenerMock1, OnInfo(logMsgStr)).Times(1);
				EXPECT_CALL(listenerMock2, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock2, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Info));
				EXPECT_CALL(listenerMock2, OnInfo(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock1);
				loggerManager.AddListener(listenerMock2);
				loggerManager.Info(logMsgStr.c_str());
			}

			/// <summary>
			/// Tests the formatting sent to a Log Listener with Info.
			/// </summary>
			TEST_F(LoggerManagerTest, Info_TestFormat)
			{
				const std::string fomartInput = "This is a test: %s %d stars!";
				const char* input1 = "IoT";
				const int input2 = 5;
				const std::string expectedResult = "This is a test: IoT 5 stars!";

				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Info));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Info));
				EXPECT_CALL(listenerMock, OnInfo(expectedResult)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Info(fomartInput.c_str(), input1, input2);
			}
			#pragma endregion

			#pragma region Warn Tests

			TEST_F(LoggerManagerTest, Warn_LevelEnabled)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Warn));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Warn));
				EXPECT_CALL(listenerMock, OnWarn(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Warn(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Warn_MinLevelGreaterThanWarn)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnWarn(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Warn(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Warn_MaxLevelLowerThanWarn)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, OnWarn(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Warn(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Warn_TwoListeners)
			{
				// Mock
				LoggerListenerMock listenerMock1;
				LoggerListenerMock listenerMock2;
				EXPECT_CALL(listenerMock1, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock1, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Warn));
				EXPECT_CALL(listenerMock1, OnWarn(logMsgStr)).Times(1);
				EXPECT_CALL(listenerMock2, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock2, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Warn));
				EXPECT_CALL(listenerMock2, OnWarn(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock1);
				loggerManager.AddListener(listenerMock2);
				loggerManager.Warn(logMsgStr.c_str());
			}


			/// <summary>
			/// Tests the formatting sent to a Log Listener with Warn.
			/// </summary>
			TEST_F(LoggerManagerTest, Warn_TestFormat)
			{
				const std::string fomartInput = "This is a test: %s %d stars!";
				const char* input1 = "IoT";
				const int input2 = 5;
				const std::string expectedResult = "This is a test: IoT 5 stars!";

				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Warn));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Warn));
				EXPECT_CALL(listenerMock, OnWarn(expectedResult)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Warn(fomartInput.c_str(), input1, input2);
			}
			#pragma endregion

			#pragma region Error Tests

			TEST_F(LoggerManagerTest, Error_LevelEnabled)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Error));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Error));
				EXPECT_CALL(listenerMock, OnError(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Error(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Error_MinLevelGreaterThanError)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnError(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Error(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Error_MaxLevelLowerThanError)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, OnError(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Error(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Error_TwoListeners)
			{
				// Mock
				LoggerListenerMock listenerMock1;
				LoggerListenerMock listenerMock2;
				EXPECT_CALL(listenerMock1, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock1, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Error));
				EXPECT_CALL(listenerMock1, OnError(logMsgStr)).Times(1);
				EXPECT_CALL(listenerMock2, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock2, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Error));
				EXPECT_CALL(listenerMock2, OnError(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock1);
				loggerManager.AddListener(listenerMock2);
				loggerManager.Error(logMsgStr.c_str());
			}

			/// <summary>
			/// Tests the formatting sent to a Log Listener with Error.
			/// </summary>
			TEST_F(LoggerManagerTest, Error_TestFormat)
			{
				const std::string fomartInput = "This is a test: %s %d stars!";
				const char* input1 = "IoT";
				const int input2 = 5;
				const std::string expectedResult = "This is a test: IoT 5 stars!";

				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Error));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Error));
				EXPECT_CALL(listenerMock, OnError(expectedResult)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Error(fomartInput.c_str(), input1, input2);
			}
			#pragma endregion

			#pragma region Fatal Tests

			TEST_F(LoggerManagerTest, Fatal_LevelEnabled)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnFatal(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Fatal(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Fatal_MaxLevelLowerThanFatal)
			{
				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillRepeatedly(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock, OnFatal(logMsgStr)).Times(0);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Fatal(logMsgStr.c_str());
			}

			TEST_F(LoggerManagerTest, Fatal_TwoListeners)
			{
				// Mock
				LoggerListenerMock listenerMock1;
				LoggerListenerMock listenerMock2;
				EXPECT_CALL(listenerMock1, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock1, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock1, OnFatal(logMsgStr)).Times(1);
				EXPECT_CALL(listenerMock2, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Trace));
				EXPECT_CALL(listenerMock2, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock2, OnFatal(logMsgStr)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock1);
				loggerManager.AddListener(listenerMock2);
				loggerManager.Fatal(logMsgStr.c_str());
			}

			/// <summary>
			/// Tests the formatting sent to a Log Listener with Fatal.
			/// </summary>
			TEST_F(LoggerManagerTest, Fatal_TestFormat)
			{
				const std::string fomartInput = "This is a test: %s %d stars!";
				const char* input1 = "IoT";
				const int input2 = 5;
				const std::string expectedResult = "This is a test: IoT 5 stars!";

				// Mock
				LoggerListenerMock listenerMock;
				EXPECT_CALL(listenerMock, GetMinLevel()).WillOnce(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, GetMaxLevel()).WillOnce(Return(Thing::Core::LogLevel::Fatal));
				EXPECT_CALL(listenerMock, OnFatal(expectedResult)).Times(1);

				// Act
				loggerManager.AddListener(listenerMock);
				loggerManager.Fatal(fomartInput.c_str(), input1, input2);
			}
			#pragma endregion
		}
	}
}