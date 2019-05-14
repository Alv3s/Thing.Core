#include "IOManagerTest.h"
#include "DigitalInputListenerMock.h"
#include "DigitalOutputListenerMock.h"
#include "DigitalInputMock.h"
#include "DigitalOutputMock.h"
#include "../IoC.h"
#include "../IOManager.h"
#include "AnalogInputListenerMock.h"
#include "AnalogInputMock.h"
#include "AppContainerMock.h"
#include "../AppTaskScheduler.h"
#include "RunnableMock.h"

using ::testing::Return;
using ::testing::_;
using ::testing::InSequence;

namespace Thing {
	namespace Core {
		namespace Tests {
			void IOManagerTest::SetUp()
			{
				Logger = &LoggerMock;
				Hardware = &HardwareMock;
				TaskScheduler = &TaskSchedulerMock;

				HardwareMock.AddListener(&TaskSchedulerMock);
				HardwareMock.ResetTimer();
			}

			void IOManagerTest::TearDown()
			{

			}

			/// <summary>
			/// Tests when there is no inputs defined.
			/// </summary>
			TEST_F(IOManagerTest, ProcessWithoutInputs) 
			{
				Thing::Core::IOManager Manager;
				Manager.Process(); // Dumb test. Only calls Process() and expects no exception thrown.
			}

#pragma region Inputs Management Tests
#pragma region Analogic Inputs Tests
			void IOManagerTest::NotifyAnalogInputListeners(Thing::Core::IOManager& Manager, const int totalListeners, AnalogInputListenerMock* listeners)
			{
				AnalogInputMock input;

				// Setup the AnalogRead method to be called with value = 10 when AddAnalogInput is called.
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(10));
				Manager.AddAnalogInput(input);

				// Setup call GetPrecision
				EXPECT_CALL(input, GetPrecision()).WillRepeatedly(Return(5));

				// Test: When current value (10) is exactly equal to previous value (10)
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(10));
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnIncreasingValue(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnDecreasingValue(_, _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (14) is not higher than previous value (10) + precision (5)
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(14));
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnIncreasingValue(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnDecreasingValue(_, _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (10) is not lower than previous value (14) - precision (5)
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(10));
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnIncreasingValue(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnDecreasingValue(_, _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (15) is equal to previous value (10) + precision (5)
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(15));
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnIncreasingValue(_, _)).Times(1);
					EXPECT_CALL(listeners[i], OnDecreasingValue(_, _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (10) is equal to previous value (15) - precision (5)
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(10));
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnIncreasingValue(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnDecreasingValue(_, _)).Times(1);
				}
				Manager.Process();

				// Test: When current value (16) is higher than previous value (10) + precision (5)
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(16));
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnIncreasingValue(_, _)).Times(1);
					EXPECT_CALL(listeners[i], OnDecreasingValue(_, _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (10) is lower than previous value (16) - precision (5)
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(10));
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnIncreasingValue(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnDecreasingValue(_, _)).Times(1);
				}
				Manager.Process();

				// Test: When the input precision is zero
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(50));
				EXPECT_CALL(input, GetPrecision()).WillOnce(Return(0));
				EXPECT_CALL(LoggerMock, WarnOverride(_, _)).Times(1);
				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnIncreasingValue(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnDecreasingValue(_, _)).Times(0);
				}
				Manager.Process();
			}

			/// <summary>
			/// Tests All notification scenarios with multiple AnalogInputListeners and one AnalogInput when they are passed as a reference.
			/// Scenarios:
			/// - Action: When current value is exactly equal to previous value. Result: Do nothing.
			/// - Action: When current value is not higher than previous value + input precision. Result: Do nothing.
			/// - Action: When current value is not lower than previous value - precision. Result: Do nothing.
			/// - Action: When current value is equal to previous value + precision. Result: Call OnValueIncresing.
			/// - Action: When current value is equal to previous value - precision. Result: Call OnValueDecreasing.
			/// - Action: When current value is higher than previous value + precision. Result: Call OnValueIncresing.
			/// - Action: When current value is lower than previous value - precision. Result: Call OnValueDecreasing.
			/// - Action: When the input precision is zero. Result: Do nothing but logs warning.
			/// </summary>
			TEST_F(IOManagerTest, NotifyAnalogInputListenersViaReference)
			{
				const int totalListeners = 10;

				Thing::Core::IOManager Manager;
				AnalogInputListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i) 
					Manager.AddListener(listeners[i]);
			
				NotifyAnalogInputListeners(Manager, totalListeners, listeners);
			}

			/// <summary>
			/// Tests All notification scenarios with multiple AnalogInputListeners and one AnalogInput when they are passed as a pointer.
			/// Scenarios:
			/// - Action: When current value is exactly equal to previous value. Result: Do nothing.
			/// - Action: When current value is not higher than previous value + input precision. Result: Do nothing.
			/// - Action: When current value is not lower than previous value - precision. Result: Do nothing.
			/// - Action: When current value is equal to previous value + precision. Result: Call OnValueIncresing.
			/// - Action: When current value is equal to previous value - precision. Result: Call OnValueDecreasing.
			/// - Action: When current value is higher than previous value + precision. Result: Call OnValueIncresing.
			/// - Action: When current value is lower than previous value - precision. Result: Call OnValueDecreasing.
			/// - Action: When the input precision is zero. Result: Do nothing but logs warning.
			/// </summary>
			TEST_F(IOManagerTest, NotifyAnalogInputListenersViaPointer)
			{
				const int totalListeners = 10;

				Thing::Core::IOManager Manager;
				AnalogInputListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
					Manager.AddListener(&listeners[i]);

				NotifyAnalogInputListeners(Manager, totalListeners, listeners);
			}

			/// <summary>
			/// Tests Remove AnalogInputListeners from IOManger when they are passed as a reference.
			/// </summary>
			TEST_F(IOManagerTest, RemoveAnalogInputListenersViaReference)
			{
				Thing::Core::IOManager Manager;

				AnalogInputListenerMock listener1;
				EXPECT_CALL(listener1, OnIncreasingValue(_, _)).Times(1);
				Manager.AddListener(listener1);

				AnalogInputListenerMock listener2;
				EXPECT_CALL(listener2, OnIncreasingValue(_, _)).Times(2);
				Manager.AddListener(listener2);

				AnalogInputListenerMock listener3;
				EXPECT_CALL(listener3, OnIncreasingValue(_, _)).Times(3);
				Manager.AddListener(listener3);

				AnalogInputMock input;
				EXPECT_CALL(input, GetPrecision()).WillRepeatedly(Return(1));
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(10));
				Manager.AddAnalogInput(input);

				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(20));
				Manager.Process();
				Manager.RemoveListener(listener1);

				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(30));
				Manager.Process();
				Manager.RemoveListener(listener2);

				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(40));
				Manager.Process();
			}

			/// <summary>
			/// Tests Remove AnalogInputListeners from IOManger when they are passed as a pointer.
			/// </summary>
			TEST_F(IOManagerTest, RemoveAnalogInputListenersViaPointer)
			{
				Thing::Core::IOManager Manager;

				AnalogInputListenerMock listener1;
				EXPECT_CALL(listener1, OnIncreasingValue(_, _)).Times(1);
				Manager.AddListener(&listener1);

				AnalogInputListenerMock listener2;
				EXPECT_CALL(listener2, OnIncreasingValue(_, _)).Times(2);
				Manager.AddListener(&listener2);

				AnalogInputListenerMock listener3;
				EXPECT_CALL(listener3, OnIncreasingValue(_, _)).Times(3);
				Manager.AddListener(&listener3);

				AnalogInputMock input;
				EXPECT_CALL(input, GetPrecision()).WillRepeatedly(Return(1));
				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(10));
				Manager.AddAnalogInput(input);

				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(20));
				Manager.Process();
				Manager.RemoveListener(&listener1);

				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(30));
				Manager.Process();
				Manager.RemoveListener(&listener2);

				EXPECT_CALL(input, AnalogRead()).WillOnce(Return(40));
				Manager.Process();
			}

			void IOManagerTest::TestListenerEventTestWithMultipleAnalogInputs(Thing::Core::IOManager& Manager, const int totalInputs, AnalogInputMock* inputs)
			{
				AnalogInputListenerMock listener;
				Manager.AddListener(listener);

				// Test: When current value (10) is exactly equal to previous value (10)
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(0);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (14) is not higher than previous value (10) + precision (5)
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(14));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(0);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (10) is not lower than previous value (14) - precision (5)
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(0);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (15) is equal to previous value (10) + precision (5)
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(15));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(1);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(0);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (10) is equal to previous value (15) - precision (5)
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(1);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (16) is higher than previous value (10) + precision (5)
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(16));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(1);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(0);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();

				// Test: When current value (10) is lower than previous value (16) - precision (5)
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(1);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();

				// Test: When the current value increases two times without reach the precision but the sum of two values are greater than precision
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(12)).WillOnce(Return(16));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(1);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(0);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();
				Manager.Process();

				// Test: When the current value decreases two times without reach the precision but the sum of two values are greater than precision
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(12)).WillOnce(Return(10));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(1);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();
				Manager.Process();

				// Test: When the precision is too big
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(20));
				EXPECT_CALL(inputs[0], GetPrecision()).WillOnce(Return(5000));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(0);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();

				// Test: When the input precision is zero
				EXPECT_CALL(inputs[0], AnalogRead()).WillOnce(Return(15));
				EXPECT_CALL(inputs[0], GetPrecision()).WillOnce(Return(0));
				EXPECT_CALL(listener, OnIncreasingValue(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnDecreasingValue(&inputs[0], _)).Times(0);
				for (int code = 1; code < totalInputs; ++code)
				{
					EXPECT_CALL(listener, OnIncreasingValue(&inputs[code], _)).Times(0);
					EXPECT_CALL(listener, OnDecreasingValue(&inputs[code], _)).Times(0);
				}
				Manager.Process();
			}

			/// <summary>
			/// Tests All notification scenarios with one AnalogInputListener and multiple AnalogInputs when they are passed as a reference.
			/// Scenarios:
			/// - Action: When current value is exactly equal to previous value. Result: Do nothing.
			/// - Action: When current value is not higher than previous value + input precision. Result: Do nothing.
			/// - Action: When current value is not lower than previous value - precision. Result: Do nothing.
			/// - Action: When current value is equal to previous value + precision. Result: Call OnValueIncresing.
			/// - Action: When current value is equal to previous value - precision. Result: Call OnValueDecreasing.
			/// - Action: When current value is higher than previous value + precision. Result: Call OnValueIncresing.
			/// - Action: When current value is lower than previous value - precision. Result: Call OnValueDecreasing.
			/// - Action: When the input precision is zero. Result: Do nothing but logs warning.
			/// - Action: When the current value increases two times without reach the precision but the sum of two values are greater than precision
			/// - Action: When the current value increases two times without reach the precision but the sum of two values are greater than precision
			/// - Action: When the precision is too big.
			/// </summary>
			TEST_F(IOManagerTest, ListenerEventTestWithMultipleAnalogInputsViaReference)
			{
				const int totalInputs = 10;
				Thing::Core::IOManager Manager;

				AnalogInputMock inputs[totalInputs];
				for (int i = 0; i < totalInputs; ++i)
				{
					EXPECT_CALL(inputs[i], AnalogRead()).WillRepeatedly(Return(10));
					EXPECT_CALL(inputs[i], GetPrecision()).WillRepeatedly(Return(5));
					Manager.AddAnalogInput(inputs[i]);					
				}
				
				TestListenerEventTestWithMultipleAnalogInputs(Manager, totalInputs, inputs);
			}

			/// <summary>
			/// Tests All notification scenarios with one AnalogInputListener and multiple AnalogInputs when they are passed as a pointer.
			/// Scenarios:
			/// - Action: When current value is exactly equal to previous value. Result: Do nothing.
			/// - Action: When current value is not higher than previous value + input precision. Result: Do nothing.
			/// - Action: When current value is not lower than previous value - precision. Result: Do nothing.
			/// - Action: When current value is equal to previous value + precision. Result: Call OnValueIncresing.
			/// - Action: When current value is equal to previous value - precision. Result: Call OnValueDecreasing.
			/// - Action: When current value is higher than previous value + precision. Result: Call OnValueIncresing.
			/// - Action: When current value is lower than previous value - precision. Result: Call OnValueDecreasing.
			/// - Action: When the input precision is zero. Result: Do nothing but logs warning.
			/// - Action: When the last two current values aggregated exceeds the precision but each current value is lower than precision
			/// - Action: When the current value increases two times without reach the precision but the sum of two values are greater than precision
			/// - Action: When the current value increases two times without reach the precision but the sum of two values are greater than precision
			/// - Action: When the precision is too big.
			/// </summary>
			TEST_F(IOManagerTest, ListenerEventTestWithMultipleAnalogInputsViaPointer)
			{
				const int totalInputs = 10;
				Thing::Core::IOManager Manager;

				AnalogInputMock inputs[totalInputs];
				for (int i = 0; i < totalInputs; ++i)
				{
					EXPECT_CALL(inputs[i], AnalogRead()).WillRepeatedly(Return(10));
					EXPECT_CALL(inputs[i], GetPrecision()).WillRepeatedly(Return(5));
					Manager.AddAnalogInput(&inputs[i]);
				}

				TestListenerEventTestWithMultipleAnalogInputs(Manager, totalInputs, inputs);
			}

			void IOManagerTest::TestRemoveAnalogInputs(Thing::Core::IOManager& Manager, AnalogInputMock& input1, AnalogInputMock& input2, AnalogInputMock& input3)
			{
				AnalogInputListenerMock listener;
				Manager.AddListener(listener);

				int readedValue = 15;				

				EXPECT_CALL(listener, OnIncreasingValue(&input1, _)).Times(1);
				EXPECT_CALL(listener, OnIncreasingValue(&input2, _)).Times(2);
				EXPECT_CALL(listener, OnIncreasingValue(&input3, _)).Times(3);
				
				EXPECT_CALL(input1, AnalogRead()).WillOnce(Return(readedValue += 5));
				EXPECT_CALL(input2, AnalogRead()).WillOnce(Return(readedValue += 5));
				EXPECT_CALL(input3, AnalogRead()).WillOnce(Return(readedValue += 5));
				Manager.Process();
				
				EXPECT_CALL(input2, AnalogRead()).WillOnce(Return(readedValue += 5));
				EXPECT_CALL(input3, AnalogRead()).WillOnce(Return(readedValue += 5));
				Manager.RemoveAnalogInput(input1);
				Manager.Process();
				
				EXPECT_CALL(input3, AnalogRead()).WillOnce(Return(readedValue += 5));
				Manager.RemoveAnalogInput(input2);
				Manager.Process();
			}

			/// <summary>
			/// Tests Remove AnalogInputs from IOManger when they are passed as a reference.
			/// </summary>
			TEST_F(IOManagerTest, RemoveAnalogInputsViaReference)
			{
				Thing::Core::IOManager Manager;

				// Setup
				AnalogInputMock input1;
				EXPECT_CALL(input1, AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(input1, GetPrecision()).WillRepeatedly(Return(1));
				Manager.AddAnalogInput(input1);

				AnalogInputMock input2;
				EXPECT_CALL(input2, AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(input2, GetPrecision()).WillRepeatedly(Return(1));
				Manager.AddAnalogInput(input2);

				AnalogInputMock input3;
				EXPECT_CALL(input3, AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(input3, GetPrecision()).WillRepeatedly(Return(1));
				Manager.AddAnalogInput(input3);

				TestRemoveAnalogInputs(Manager, input1, input2, input3);
			}

			/// <summary>
			/// Tests Remove AnalogInputs from IOManger when they are passed as a pointer.
			/// </summary>
			TEST_F(IOManagerTest, RemoveAnalogInputsViaPointer)
			{
				Thing::Core::IOManager Manager;

				// Setup
				AnalogInputMock input1;
				EXPECT_CALL(input1, AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(input1, GetPrecision()).WillRepeatedly(Return(1));
				Manager.AddAnalogInput(&input1);

				AnalogInputMock input2;
				EXPECT_CALL(input2, AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(input2, GetPrecision()).WillRepeatedly(Return(1));
				Manager.AddAnalogInput(&input2);

				AnalogInputMock input3;
				EXPECT_CALL(input3, AnalogRead()).WillOnce(Return(10));
				EXPECT_CALL(input3, GetPrecision()).WillRepeatedly(Return(1));
				Manager.AddAnalogInput(&input3);

				TestRemoveAnalogInputs(Manager, input1, input2, input3);
			}

			TEST_F(IOManagerTest, AnalogInputListenerRemovedInsideEvent)
			{
				AnalogInputMock analogInput;
				EXPECT_CALL(analogInput, GetPrecision()).WillRepeatedly(Return(1));
				EXPECT_CALL(analogInput, AnalogRead()).WillRepeatedly(Return(0));

				Thing::Core::IOManager manager;
				manager.AddAnalogInput(analogInput);

				AnalogInputListenerMock listener;
				manager.AddListener(listener);

				ON_CALL(listener, OnIncreasingValue(_, _)).WillByDefault(testing::Invoke(
					[&manager, &listener](IAnalogIO* io, int value)
					{
						manager.RemoveListener(listener);
					}
				));

				EXPECT_CALL(analogInput, AnalogRead()).WillRepeatedly(Return(1000));
				ASSERT_NO_FATAL_FAILURE(manager.ProcessAnalog());
			}
#pragma endregion

#pragma region Digital Inputs Tests
			TEST_F(IOManagerTest, NotifyDigitalInputListenersViaReference)
			{
				const int totalListeners = 10;
				Thing::Core::IOManager Manager;

				DigitalInputListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
					Manager.AddListener(listeners[i]);

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead())
					.Times(5)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(input);


				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_,1)).Times(1);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.Process();


				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.Process();

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_,1)).Times(1);
				}
				Manager.Process();

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.Process();
			}

			TEST_F(IOManagerTest, NotifyDigitalInputListenersViaPointer)
			{
				const int totalListeners = 10;
				Thing::Core::IOManager Manager;

				DigitalInputListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
					Manager.AddListener(&listeners[i]);

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead())
					.Times(5)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(input);


				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_,1)).Times(1);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.Process();


				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.Process();

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, 1)).Times(1);
				}
				Manager.Process();

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.Process();
			}

			TEST_F(IOManagerTest, NotifyDigitalInputListenerWithTriggerCount)
			{
				Thing::Core::IOManager Manager;

				DigitalInputListenerMock listener;
				Manager.AddListener(listener);

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead())
					.Times(7)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(input);

				// this ensures that the EXPECT_CALL occurs in the specified sequence
				InSequence inSequence; 

				EXPECT_CALL(listener, OnActivating(_, 1));
				EXPECT_CALL(listener, OnInactivating(_, 1));
				EXPECT_CALL(listener, OnActivating(_, 2));
				EXPECT_CALL(listener, OnInactivating(_, 2));
				EXPECT_CALL(listener, OnActivating(_, 1));
				EXPECT_CALL(listener, OnInactivating(_, 1));

				// 2 clicks
				Manager.Process();
				Manager.Process();
				Manager.Process();
				Manager.Process();

				// Waits before click again
				Hardware->Delay(5000);
				Manager.Process();
				Manager.Process();
			}

			TEST_F(IOManagerTest, RemoveDigitalInputListenersViaReference)
			{
				Thing::Core::IOManager Manager;

				DigitalInputListenerMock listener1;
				EXPECT_CALL(listener1, OnActivating(_,_)).Times(1);
				EXPECT_CALL(listener1, OnInactivating(_, _)).Times(0);
				Manager.AddListener(listener1);

				DigitalInputListenerMock listener2;
				EXPECT_CALL(listener2, OnActivating(_, _)).Times(1);
				EXPECT_CALL(listener2, OnInactivating(_, _)).Times(1);
				Manager.AddListener(listener2);

				DigitalInputListenerMock listener3;
				EXPECT_CALL(listener3, OnActivating(_, _)).Times(2);
				EXPECT_CALL(listener3, OnInactivating(_, _)).Times(1);
				Manager.AddListener(listener3);

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalInput(input);

				Manager.Process();
				Manager.RemoveListener(listener1);
				Manager.Process();
				Manager.RemoveListener(listener2);
				Manager.Process();
			}

			TEST_F(IOManagerTest, RemoveDigitalInputListenersViaPointer)
			{
				Thing::Core::IOManager Manager;

				DigitalInputListenerMock listener1;
				EXPECT_CALL(listener1, OnActivating(_, _)).Times(1);
				EXPECT_CALL(listener1, OnInactivating(_, _)).Times(0);
				Manager.AddListener(&listener1);

				DigitalInputListenerMock listener2;
				EXPECT_CALL(listener2, OnActivating(_, _)).Times(1);
				EXPECT_CALL(listener2, OnInactivating(_, _)).Times(1);
				Manager.AddListener(&listener2);

				DigitalInputListenerMock listener3;
				EXPECT_CALL(listener3, OnActivating(_, _)).Times(2);
				EXPECT_CALL(listener3, OnInactivating(_, _)).Times(1);
				Manager.AddListener(&listener3);

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalInput(input);

				Manager.Process();
				Manager.RemoveListener(&listener1);
				Manager.Process();
				Manager.RemoveListener(&listener2);
				Manager.Process();
			}

			TEST_F(IOManagerTest, ListenerEventTestWithMultipleDigitalInputsViaReference)
			{
				const int totalInputs = 10;
				Thing::Core::IOManager Manager;

				DigitalInputMock inputs[totalInputs];
				for (int i = 1; i < totalInputs; ++i)
				{
					EXPECT_CALL(inputs[i], DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
					Manager.AddDigitalInput(inputs[i]);
				}
				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(inputs[0]);

				DigitalInputListenerMock listener;
				Manager.AddListener(listener);

				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
				EXPECT_CALL(listener, OnActivating(&inputs[0], _)).Times(0);
				EXPECT_CALL(listener, OnInactivating(_, _)).Times(0);
				Manager.Process();

				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
				EXPECT_CALL(listener, OnActivating(&inputs[0], _)).Times(1);
				EXPECT_CALL(listener, OnInactivating(_, _)).Times(0);
				Manager.Process();

				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
				EXPECT_CALL(listener, OnActivating(_, _)).Times(0);
				EXPECT_CALL(listener, OnInactivating(_, _)).Times(0);
				Manager.Process();

				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
				EXPECT_CALL(listener, OnActivating(_, _)).Times(0);
				EXPECT_CALL(listener, OnInactivating(&inputs[0],_)).Times(1);
				Manager.Process();
			}

			// TODO: DMARTINS 2019-02-15 fazer um teste para o SetInputCountTimeout

			TEST_F(IOManagerTest, ListenerEventTestWithMultipleDigitalInputsViaPointer)
			{
				const int totalInputs = 10;
				Thing::Core::IOManager Manager;

				DigitalInputMock inputs[totalInputs];
				for (int i = 1; i < totalInputs; ++i)
				{
					EXPECT_CALL(inputs[i], DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
					Manager.AddDigitalInput(&inputs[i]);
				}
				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(&inputs[0]);

				DigitalInputListenerMock listener;
				Manager.AddListener(listener);

				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
				EXPECT_CALL(listener, OnActivating(_, _)).Times(0);
				EXPECT_CALL(listener, OnInactivating(_, _)).Times(0);
				Manager.Process();

				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
				EXPECT_CALL(listener, OnActivating(&inputs[0], _)).Times(1);
				EXPECT_CALL(listener, OnInactivating(_, _)).Times(0);
				Manager.Process();

				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
				EXPECT_CALL(listener, OnActivating(_, _)).Times(0);
				EXPECT_CALL(listener, OnInactivating(_, _)).Times(0);
				Manager.Process();

				EXPECT_CALL(inputs[0], DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
				EXPECT_CALL(listener, OnActivating(_, _)).Times(0);
				EXPECT_CALL(listener, OnInactivating(&inputs[0], _)).Times(1);
				Manager.Process();
			}

			TEST_F(IOManagerTest, RemoveDigitalInputsViaReference)
			{
				Thing::Core::IOManager Manager;

				DigitalInputMock input1;
				EXPECT_CALL(input1, DigitalRead())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalInput(input1);

				DigitalInputMock input2;
				EXPECT_CALL(input2, DigitalRead())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(input2);

				DigitalInputMock input3;
				EXPECT_CALL(input3, DigitalRead())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalInput(input3);

				DigitalInputListenerMock listener;
				EXPECT_CALL(listener, OnActivating(&input1, _)).Times(1);
				EXPECT_CALL(listener, OnInactivating(&input1, _)).Times(0);
				EXPECT_CALL(listener, OnActivating(&input2, _)).Times(1);
				EXPECT_CALL(listener, OnInactivating(&input2, _)).Times(1);
				EXPECT_CALL(listener, OnActivating(&input3, _)).Times(2);
				EXPECT_CALL(listener, OnInactivating(&input3, _)).Times(1);
				Manager.AddListener(listener);

				Manager.Process();
				Manager.RemoveDigitalInput(input1);
				Manager.Process();
				Manager.RemoveDigitalInput(input2);
				Manager.Process();
			}

			TEST_F(IOManagerTest, RemoveDigitalInputsViaPointer)
			{
				Thing::Core::IOManager Manager;

				DigitalInputMock input1;
				EXPECT_CALL(input1, DigitalRead())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalInput(&input1);

				DigitalInputMock input2;
				EXPECT_CALL(input2, DigitalRead())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(&input2);

				DigitalInputMock input3;
				EXPECT_CALL(input3, DigitalRead())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalInput(&input3);

				DigitalInputListenerMock listener;
				EXPECT_CALL(listener, OnActivating(&input1, _)).Times(1);
				EXPECT_CALL(listener, OnInactivating(&input1, _)).Times(0);
				EXPECT_CALL(listener, OnActivating(&input2, _)).Times(1);
				EXPECT_CALL(listener, OnInactivating(&input2, _)).Times(1);
				EXPECT_CALL(listener, OnActivating(&input3, _)).Times(2);
				EXPECT_CALL(listener, OnInactivating(&input3, _)).Times(1);
				Manager.AddListener(listener);

				Manager.Process();
				Manager.RemoveDigitalInput(&input1);
				Manager.Process();
				Manager.RemoveDigitalInput(&input2);
				Manager.Process();
			}

			TEST_F(IOManagerTest, OneDigitalInputAddedMultipleTimes)
			{
				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead()).Times(2).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				Thing::Core::IOManager manager;
				DigitalInputListenerMock listener;
				manager.AddListener(listener);
				
				manager.AddDigitalInput(input);
				manager.AddDigitalInput(input);
				manager.Process();
			}

			TEST_F(IOManagerTest, OneDigitalInputListenerAddedMultipleTimes)
			{
				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low)).WillOnce(Return(Thing::Core::DigitalValue::High));

				DigitalInputListenerMock listener;
				EXPECT_CALL(listener, OnActivating(&input, _)).Times(1);

				Thing::Core::IOManager manager;
				manager.AddDigitalInput(input);
				manager.AddListener(listener);
				manager.AddListener(listener);
				manager.Process();
			}

			TEST_F(IOManagerTest, DigitalInputListenerRemovedInsideEvent)
			{
				DigitalInputMock digitalInput;
				EXPECT_CALL(digitalInput, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				Thing::Core::IOManager manager;
				manager.AddDigitalInput(digitalInput);

				DigitalInputListenerMock listener;
				manager.AddListener(listener);

				ON_CALL(listener, OnActivating(_, _)).WillByDefault(testing::Invoke(
					[&manager, &listener](Thing::Core::IDigitalIO* io, unsigned int count)
					{
						manager.RemoveListener(listener);
					}
				));

				EXPECT_CALL(digitalInput, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
				ASSERT_NO_FATAL_FAILURE(manager.ProcessDigital());
			}
#pragma endregion
#pragma endregion

#pragma region Outputs Management Tests
			TEST_F(IOManagerTest, DigitalWriteSimpleTestViaReference)
			{
				Thing::Core::DigitalValue states[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				Thing::Core::IOManager Manager;

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				Manager.AddDigitalOutput(output);

				for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalValue); ++i)
				{
					EXPECT_CALL(output, DigitalWrite(states[i])).Times(1);
					Manager.DigitalWrite(output, states[i]);
				}
			}

			TEST_F(IOManagerTest, DigitalWriteSimpleTestViaPointer)
			{
				Thing::Core::DigitalValue states[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				Thing::Core::IOManager Manager;

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				Manager.AddDigitalOutput(&output);

				for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalValue); ++i)
				{
					EXPECT_CALL(output, DigitalWrite(states[i])).Times(1);
					Manager.DigitalWrite(&output, states[i]);
				}
			}

			TEST_F(IOManagerTest, DigitalWriteMultipleOutputsTestViaReference)
			{
				const int totalOutputs = 10;

				Thing::Core::DigitalValue states[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				Thing::Core::IOManager Manager;

				DigitalOutputMock output[totalOutputs];
				for (int i = 0; i < totalOutputs; ++i)
				{
					EXPECT_CALL(output[i], GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
					Manager.AddDigitalOutput(output[i]);
				}

				for (int j = 0; j < totalOutputs; ++j)
				{
					for (int aux = 0; aux < totalOutputs; aux++)
					{
						if (j == aux)
							continue;

						EXPECT_CALL(output[aux], DigitalWrite(_)).Times(0);
					}

					for (int k = 0; k < sizeof(states) / sizeof(Thing::Core::DigitalValue); ++k)
					{
						EXPECT_CALL(output[j], DigitalWrite(states[k])).Times(1);
						Manager.DigitalWrite(output[j], states[k]);
					}
				}
			}

			TEST_F(IOManagerTest, DigitalWriteMultipleOutputsTestViaPointer)
			{
				const int totalOutputs = 10;

				Thing::Core::DigitalValue states[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				Thing::Core::IOManager Manager;

				DigitalOutputMock output[totalOutputs];
				for (int i = 0; i < totalOutputs; ++i)
				{
					EXPECT_CALL(output[i], GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
					Manager.AddDigitalOutput(&output[i]);
				}

				for (int j = 0; j < totalOutputs; ++j)
				{
					for (int aux = 0; aux < totalOutputs; aux++)
					{
						if (j == aux)
							continue;

						EXPECT_CALL(output[aux], DigitalWrite(_)).Times(0);
					}

					for (int k = 0; k < sizeof(states) / sizeof(Thing::Core::DigitalValue); ++k)
					{
						EXPECT_CALL(output[j], DigitalWrite(states[k])).Times(1);
						Manager.DigitalWrite(&output[j], states[k]);
					}
				}
			}

			TEST_F(IOManagerTest, NotifyOutputListenersViaReference)
			{
				const int totalListeners = 10;
				Thing::Core::IOManager Manager;

				DigitalOutputListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
					Manager.AddListener(listeners[i]);

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState())
					.Times(8)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalOutput(output);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::Low);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(&output, _)).Times(1);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(&output, _)).Times(1);
				}
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::Low);
			}

			TEST_F(IOManagerTest, NotifyOutputListenersViaPointer)
			{
				const int totalListeners = 10;
				Thing::Core::IOManager Manager;

				DigitalOutputListenerMock listeners[totalListeners];
				for (int i = 0; i < totalListeners; ++i)
					Manager.AddListener(&listeners[i]);

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState())
					.Times(8)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalOutput(&output);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.DigitalWrite(&output, Thing::Core::DigitalValue::Low);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(&output, _)).Times(1);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.DigitalWrite(&output, Thing::Core::DigitalValue::High);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(_, _)).Times(0);
				}
				Manager.DigitalWrite(&output, Thing::Core::DigitalValue::High);

				for (int i = 0; i < totalListeners; ++i)
				{
					EXPECT_CALL(listeners[i], OnActivating(_, _)).Times(0);
					EXPECT_CALL(listeners[i], OnInactivating(&output, _)).Times(1);
				}
				Manager.DigitalWrite(&output, Thing::Core::DigitalValue::Low);
			}

			TEST_F(IOManagerTest, NotifyOutputListenerWithTriggerCount)
			{
				Thing::Core::IOManager Manager;

				DigitalOutputListenerMock listener;
				Manager.AddListener(listener);

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState())
					.Times(12)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalOutput(output);

				// this ensures that the EXPECT_CALL occurs in the specified sequence
				InSequence inSequence;

				EXPECT_CALL(listener, OnActivating(_, 1));
				EXPECT_CALL(listener, OnInactivating(_, 1));
				EXPECT_CALL(listener, OnActivating(_, 2));
				EXPECT_CALL(listener, OnInactivating(_, 2));
				EXPECT_CALL(listener, OnActivating(_, 1));
				EXPECT_CALL(listener, OnInactivating(_, 1));

				// 2 clicks
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::Low);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::Low);
				Hardware->Delay(500);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::Low);
			}

			TEST_F(IOManagerTest, RemoveOutputListenersViaReference)
			{
				Thing::Core::IOManager Manager;

				DigitalOutputListenerMock listener1;
				EXPECT_CALL(listener1, OnActivating(_, _)).Times(1);
				Manager.AddListener(listener1);

				DigitalOutputListenerMock listener2;
				EXPECT_CALL(listener2, OnActivating(_, _)).Times(2);
				Manager.AddListener(listener2);

				DigitalOutputListenerMock listener3;
				EXPECT_CALL(listener3, OnActivating(_, _)).Times(3);
				Manager.AddListener(listener3);

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalOutput(output);

				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
				Manager.RemoveListener(listener1);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
				Manager.RemoveListener(listener2);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
			}

			TEST_F(IOManagerTest, RemoveOutputListenersViaPointer)
			{
				Thing::Core::IOManager Manager;

				DigitalOutputListenerMock listener1;
				EXPECT_CALL(listener1, OnActivating(_, _)).Times(1);
				Manager.AddListener(&listener1);

				DigitalOutputListenerMock listener2;
				EXPECT_CALL(listener2, OnActivating(_, _)).Times(2);
				Manager.AddListener(&listener2);

				DigitalOutputListenerMock listener3;
				EXPECT_CALL(listener3, OnActivating(_, _)).Times(3);
				Manager.AddListener(&listener3);

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalOutput(output);

				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
				Manager.RemoveListener(&listener1);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
				Manager.RemoveListener(&listener2);
				Manager.DigitalWrite(output, Thing::Core::DigitalValue::High);
			}

			TEST_F(IOManagerTest, ListenerEventTestWithMultipleOutputsViaReference)
			{
				const int totalOutputs = 10;
				Thing::Core::IOManager Manager;

				DigitalOutputMock outputs[totalOutputs];
				for (int i = 1; i < totalOutputs; ++i)
				{
					EXPECT_CALL(outputs[i], GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
					EXPECT_CALL(outputs[i], DigitalWrite(_)).Times(0);
					Manager.AddDigitalOutput(outputs[i]);
				}
				EXPECT_CALL(outputs[0], DigitalWrite(_)).WillRepeatedly(Return());
				Manager.AddDigitalOutput(outputs[0]);

				DigitalOutputListenerMock listener;
				Manager.AddListener(listener);
				
				EXPECT_CALL(outputs[0], GetState()).Times(2).WillOnce(Return(Thing::Core::DigitalValue::Low)).WillOnce(Return(Thing::Core::DigitalValue::High));
				EXPECT_CALL(listener, OnActivating(&outputs[0], _)).Times(1);
				EXPECT_CALL(listener, OnInactivating(_, _)).Times(0);
				Manager.DigitalWrite(outputs[0], Thing::Core::DigitalValue::High);

				EXPECT_CALL(outputs[0], GetState()).Times(2).WillOnce(Return(Thing::Core::DigitalValue::High)).WillOnce(Return(Thing::Core::DigitalValue::Low));
				EXPECT_CALL(listener, OnActivating(_, _)).Times(0);
				EXPECT_CALL(listener, OnInactivating(&outputs[0], _)).Times(1);
				Manager.DigitalWrite(outputs[0], Thing::Core::DigitalValue::Low);
			}

			TEST_F(IOManagerTest, ListenerEventTestWithMultipleOutputsViaPointer)
			{
				const int totalOutputs = 10;
				Thing::Core::IOManager Manager;

				DigitalOutputMock outputs[totalOutputs];
				for (int i = 1; i < totalOutputs; ++i)
				{
					EXPECT_CALL(outputs[i], GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
					EXPECT_CALL(outputs[i], DigitalWrite(_)).Times(0);
					Manager.AddDigitalOutput(&outputs[i]);
				}
				EXPECT_CALL(outputs[0], DigitalWrite(_)).WillRepeatedly(Return());
				Manager.AddDigitalOutput(&outputs[0]);

				DigitalOutputListenerMock listener;
				Manager.AddListener(&listener);
				
				EXPECT_CALL(outputs[0], GetState()).Times(2).WillOnce(Return(Thing::Core::DigitalValue::Low)).WillOnce(Return(Thing::Core::DigitalValue::High));
				EXPECT_CALL(listener, OnActivating(&outputs[0], _)).Times(1);
				EXPECT_CALL(listener, OnInactivating(_, _)).Times(0);
				Manager.DigitalWrite(&outputs[0], Thing::Core::DigitalValue::High);

				EXPECT_CALL(outputs[0], GetState()).Times(2).WillOnce(Return(Thing::Core::DigitalValue::High)).WillOnce(Return(Thing::Core::DigitalValue::Low));
				EXPECT_CALL(listener, OnActivating(_, _)).Times(0);
				EXPECT_CALL(listener, OnInactivating(&outputs[0], _)).Times(1);
				Manager.DigitalWrite(&outputs[0], Thing::Core::DigitalValue::Low);
			}

			TEST_F(IOManagerTest, RemoveOutputsViaReference)
			{
				Thing::Core::IOManager Manager;

				DigitalOutputMock output1;
				EXPECT_CALL(output1, GetState()).Times(2)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalOutput(output1);

				DigitalOutputMock output2;
				EXPECT_CALL(output2, GetState()).Times(4)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalOutput(output2);

				DigitalOutputMock output3;
				EXPECT_CALL(output3, GetState()).Times(6)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalOutput(output3);

				DigitalOutputListenerMock listener;
				EXPECT_CALL(listener, OnActivating(&output1, _)).Times(1);
				EXPECT_CALL(listener, OnActivating(&output2, _)).Times(2);
				EXPECT_CALL(listener, OnActivating(&output3, _)).Times(3);
				Manager.AddListener(listener);

				Manager.DigitalWrite(output1, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(output2, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(output3, Thing::Core::DigitalValue::High);

				Manager.RemoveDigitalOutput(&output1);
				Manager.DigitalWrite(output2, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(output3, Thing::Core::DigitalValue::High);

				Manager.RemoveDigitalOutput(&output2);
				Manager.DigitalWrite(output3, Thing::Core::DigitalValue::High);
			}

			TEST_F(IOManagerTest, RemoveOutputsViaPointer)
			{
				Thing::Core::IOManager Manager;

				DigitalOutputMock output1;
				EXPECT_CALL(output1, GetState()).Times(2)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalOutput(&output1);

				DigitalOutputMock output2;
				EXPECT_CALL(output2, GetState()).Times(4)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalOutput(&output2);

				DigitalOutputMock output3;
				EXPECT_CALL(output3, GetState()).Times(6)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalOutput(&output3);

				DigitalOutputListenerMock listener;
				EXPECT_CALL(listener, OnActivating(&output1, _)).Times(1);
				EXPECT_CALL(listener, OnActivating(&output2, _)).Times(2);
				EXPECT_CALL(listener, OnActivating(&output3, _)).Times(3);
				Manager.AddListener(listener);

				Manager.DigitalWrite(&output1, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(&output2, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(&output3, Thing::Core::DigitalValue::High);

				Manager.RemoveDigitalOutput(&output1);
				Manager.DigitalWrite(&output2, Thing::Core::DigitalValue::High);
				Manager.DigitalWrite(&output3, Thing::Core::DigitalValue::High);

				Manager.RemoveDigitalOutput(&output2);
				Manager.DigitalWrite(&output3, Thing::Core::DigitalValue::High);
			}

			TEST_F(IOManagerTest, OneOutputAddedMultipleTimes)
			{
				DigitalOutputMock output;
				EXPECT_CALL(output, GetState()).Times(2).WillOnce(Return(Thing::Core::DigitalValue::Low)).WillOnce(Return(Thing::Core::DigitalValue::High));

				Thing::Core::IOManager manager;
				manager.AddDigitalOutput(output);
				manager.AddDigitalOutput(output);

				DigitalOutputListenerMock listener;
				manager.AddListener(listener);

				EXPECT_CALL(listener, OnActivating(_, _)).Times(1);
				manager.DigitalWrite(output, Thing::Core::DigitalValue::Low);
			}

			TEST_F(IOManagerTest, OneOutputListenerAddedMultipleTimes)
			{
				DigitalOutputMock output;
				EXPECT_CALL(output, GetState()).WillOnce(Return(Thing::Core::DigitalValue::Low)).WillOnce(Return(Thing::Core::DigitalValue::High));

				DigitalOutputListenerMock listener;
				EXPECT_CALL(listener, OnActivating(&output, _)).Times(1);

				Thing::Core::IOManager manager;
				manager.AddDigitalOutput(output);
				manager.AddListener(listener);
				manager.AddListener(listener);
				manager.DigitalWrite(output, Thing::Core::DigitalValue::Low);
			}

			TEST_F(IOManagerTest, DigitalWriteWithTimerViaReference)
			{
				const Thing::Core::DigitalValue states[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				const unsigned long time[] = { 1000, 2000, 3000 };

				Thing::Core::IOManager Manager;
				AppContainerMock containerMock;
				AppTaskScheduler scheduler(containerMock);
				TaskScheduler = &scheduler;

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				Manager.AddDigitalOutput(output);

				for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalValue); ++i)
					for(int j = 0; j < sizeof(time) / sizeof(unsigned long); ++j)
					{
						EXPECT_CALL(output, DigitalWrite(states[i])).Times(1);
						Manager.DigitalWrite(output, states[i], time[i]);

						Hardware->Delay(time[i] - 1);
						scheduler.OnLoop();
						switch (states[i])
						{
						case Thing::Core::DigitalValue::Low:
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High)).Times(1);
							break;
						case Thing::Core::DigitalValue::High:
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low)).Times(1);
							break;
						case Thing::Core::DigitalValue::Toggle:
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Toggle)).Times(1);
							break;
						}
						Hardware->Delay(1);
						scheduler.OnLoop();

						//Make sure it doesn't trigger again
						Hardware->Delay(time[i]);
						scheduler.OnLoop();
					}
			}

			TEST_F(IOManagerTest, DigitalWriteWithTimerViaPointer)
			{
				const Thing::Core::DigitalValue states[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				const unsigned long time[] = { 1000, 2000, 3000 };

				Thing::Core::IOManager Manager;
				AppContainerMock containerMock;
				AppTaskScheduler scheduler(containerMock);
				TaskScheduler = &scheduler;

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				Manager.AddDigitalOutput(&output);

				for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalValue); ++i)
					for (int j = 0; j < sizeof(time) / sizeof(unsigned long); ++j)
					{
						EXPECT_CALL(output, DigitalWrite(states[i])).Times(1);
						Manager.DigitalWrite(&output, states[i], time[i]);

						Hardware->Delay(time[i] - 1);
						scheduler.OnLoop();
						switch (states[i])
						{
						case Thing::Core::DigitalValue::Low:
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High)).Times(1);
							break;
						case Thing::Core::DigitalValue::High:
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low)).Times(1);
							break;
						case Thing::Core::DigitalValue::Toggle:
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Toggle)).Times(1);
							break;
						}
						Hardware->Delay(1);
						scheduler.OnLoop();

						//Make sure it doesn't trigger again
						Hardware->Delay(time[i]);
						scheduler.OnLoop();
					}
			}

			TEST_F(IOManagerTest, DigitalWriteWithTimerAndValueChangedViaReference)
			{
				const Thing::Core::DigitalValue states[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle};
				const unsigned long time[] = { 1000, 2000, 3000};

				Thing::Core::IOManager Manager;
				AppContainerMock containerMock;
				AppTaskScheduler scheduler(containerMock);
				TaskScheduler = &scheduler;

				DigitalOutputMock output;

				Manager.AddDigitalOutput(output);

				for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalValue); ++i)
					for (int j = 0; j < sizeof(time) / sizeof(unsigned long); ++j)
					{
						EXPECT_CALL(output, GetState())
							.WillOnce(Return(states[i] == Thing::Core::DigitalValue::Low ? Thing::Core::DigitalValue::High : Thing::Core::DigitalValue::Low))
							.WillOnce(Return(states[i] == Thing::Core::DigitalValue::Low ? Thing::Core::DigitalValue::Low : Thing::Core::DigitalValue::High));
						EXPECT_CALL(output, DigitalWrite(states[i])).Times(1);
						Manager.DigitalWrite(output, states[i], time[i]);

						Hardware->Delay(time[i] - 1);
						scheduler.OnLoop();

						EXPECT_CALL(output, GetState())
							.WillOnce(Return(states[i] == Thing::Core::DigitalValue::Low ? Thing::Core::DigitalValue::Low : Thing::Core::DigitalValue::High))
							.WillOnce(Return(states[i] == Thing::Core::DigitalValue::Low ? Thing::Core::DigitalValue::High : Thing::Core::DigitalValue::Low));
						EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Toggle)).Times(1);
						Manager.DigitalWrite(output, Thing::Core::DigitalValue::Toggle);
						Hardware->Delay(1);
						scheduler.OnLoop();
					}
			}
			
			TEST_F(IOManagerTest, DigitalWriteWithTimerAndValueChangedViaPointer)
			{
				const Thing::Core::DigitalValue states[] = { Thing::Core::DigitalValue::Low };// , Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle};
				const unsigned long time[] = { 1000 };// , 2000, 3000};

				Thing::Core::IOManager Manager;
				AppContainerMock containerMock;
				AppTaskScheduler scheduler(containerMock);
				TaskScheduler = &scheduler;

				DigitalOutputMock output;

				Manager.AddDigitalOutput(&output);

				for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalValue); ++i)
					for (int j = 0; j < sizeof(time) / sizeof(unsigned long); ++j)
					{
						EXPECT_CALL(output, GetState())
							.WillOnce(Return(states[i] == Thing::Core::DigitalValue::Low ? Thing::Core::DigitalValue::High : Thing::Core::DigitalValue::Low))
							.WillOnce(Return(states[i] == Thing::Core::DigitalValue::Low ? Thing::Core::DigitalValue::Low : Thing::Core::DigitalValue::High));
						EXPECT_CALL(output, DigitalWrite(states[i])).Times(1);
						Manager.DigitalWrite(&output, states[i], time[i]);

						Hardware->Delay(time[i] - 1);
						scheduler.OnLoop();

						EXPECT_CALL(output, GetState())
							.WillOnce(Return(states[i] == Thing::Core::DigitalValue::Low ? Thing::Core::DigitalValue::Low : Thing::Core::DigitalValue::High))
							.WillOnce(Return(states[i] == Thing::Core::DigitalValue::Low ? Thing::Core::DigitalValue::High : Thing::Core::DigitalValue::Low));
						EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Toggle)).Times(1);
						Manager.DigitalWrite(&output, Thing::Core::DigitalValue::Toggle);
						Hardware->Delay(1);
						scheduler.OnLoop();
					}
			}

			TEST_F(IOManagerTest, DigitalOutputListenerRemovedInsideEvent)
			{
				DigitalOutputMock output;

				Thing::Core::IOManager manager;
				manager.AddDigitalOutput(output);

				DigitalOutputListenerMock listener;
				manager.AddListener(listener);

				ON_CALL(listener, OnActivating(_, _)).WillByDefault(testing::Invoke(
					[&manager, &listener](Thing::Core::IDigitalIO* io, unsigned int count)
					{
						manager.RemoveListener(listener);
					}
				));

				EXPECT_CALL(output, GetState())
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High));
				ASSERT_NO_FATAL_FAILURE(manager.DigitalWrite(output, Thing::Core::DigitalValue::High));
			}
#pragma endregion

#pragma region DigitalIOMonitor Input Test
			TEST_F(IOManagerTest, DigitalIOMonitorSetInputActionSimpleViaReference)
			{
				Thing::Core::DigitalInputState states[] =
				{
					Thing::Core::DigitalInputState::WasActivated,
					Thing::Core::DigitalInputState::WasInactivated
				};

				Thing::Core::DigitalValue outputStates[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				for (int j = 0; j < sizeof(outputStates) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalInputState); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));

						DigitalOutputMock output;
						EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.AddDigitalInput(input);

						switch (states[i])
						{
						case Thing::Core::DigitalInputState::WasActivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnActivating(&input).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnActivating(&input).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnActivating(&input).Toggle(&output); break;
							}
							break;
						case Thing::Core::DigitalInputState::WasInactivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnInactivating(&input).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnInactivating(&input).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnInactivating(&input).Toggle(&output); break;
							}
							break;
						}

						EXPECT_CALL(input, DigitalRead())
							.Times(4)
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::Low));

						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Inactive ? 1 : 0);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasActivated ? 1 : 0);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Active ? 1 : 0);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasInactivated ? 1 : 0);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorSetInputActionSimpleViaPointer)
			{
				Thing::Core::DigitalInputState states[] =
				{
					Thing::Core::DigitalInputState::WasActivated,
					Thing::Core::DigitalInputState::WasInactivated
				};

				Thing::Core::DigitalValue outputStates[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				for (int j = 0; j < sizeof(outputStates) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalInputState); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));

						DigitalOutputMock output;
						EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.AddDigitalInput(input);

						switch (states[i])
						{
						case Thing::Core::DigitalInputState::WasActivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnActivating(&input).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnActivating(&input).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnActivating(&input).Toggle(&output); break;
							}
							break;
						case Thing::Core::DigitalInputState::WasInactivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnInactivating(&input).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnInactivating(&input).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnInactivating(&input).Toggle(&output); break;
							}
							break;
						}

						EXPECT_CALL(input, DigitalRead())
							.Times(4)
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::Low));

						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Inactive ? 1 : 0);;
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasActivated ? 1 : 0);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Active ? 1 : 0);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasInactivated ? 1 : 0);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorSetInputActionMultipleInputsTriggeringCorrectInput)
			{
				Thing::Core::DigitalInputState states[] =
				{
					Thing::Core::DigitalInputState::WasActivated,
					Thing::Core::DigitalInputState::WasInactivated
				};

				Thing::Core::DigitalValue outputStates[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				for (int j = 0; j < sizeof(outputStates) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalInputState); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						DigitalInputMock inputNever;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
						EXPECT_CALL(inputNever, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
						DigitalOutputMock output;
						EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.AddDigitalInput(input);
						Manager.AddDigitalInput(inputNever);

						switch (states[i])
						{
						case Thing::Core::DigitalInputState::WasActivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnActivating(&input).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnActivating(&input).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnActivating(&input).Toggle(&output); break;
							}
							break;
						case Thing::Core::DigitalInputState::WasInactivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnInactivating(&input).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnInactivating(&input).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnInactivating(&input).Toggle(&output); break;
							}
							break;
						}

						EXPECT_CALL(input, DigitalRead())
							.Times(4)
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::Low));

						EXPECT_CALL(inputNever, DigitalRead())
							.Times(4)
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::Low));

						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Inactive ? 1 : 0);;
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasActivated ? 1 : 0);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Active ? 1 : 0);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasInactivated ? 1 : 0);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorSetInputActionMultipleInputsNotTriggeredCorrectInput)
			{
				Thing::Core::IOManager Manager;

				DigitalInputMock input;
				DigitalInputMock inputChangingState;
				EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
				EXPECT_CALL(inputChangingState, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
				DigitalOutputMock output;
				Manager.AddDigitalInput(input);
				Manager.AddDigitalInput(inputChangingState);

				Manager.OnActivating(&input).Toggle(&output);

				EXPECT_CALL(input, DigitalRead())
					.Times(4)
					.WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				EXPECT_CALL(inputChangingState, DigitalRead())
					.Times(4)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));

				EXPECT_CALL(output, DigitalWrite(_)).Times(0);
				Manager.Process();
				Manager.Process();
				Manager.Process();
				Manager.Process();
			}

			TEST_F(IOManagerTest, DigitalOutputMonitorForInputMultipleTimes)
			{
				const int millis = 1000;
				const int triggerTimes = 3;

				Thing::Core::IOManager Manager;

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));

				DigitalOutputMock output;
				EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				Manager.AddDigitalInput(input);

				Manager.OnActivating(input).SetHigh(&output).For(millis);

				for (int attempt = 0; attempt < triggerTimes; ++attempt)
				{
					EXPECT_CALL(input, DigitalRead())
						.WillOnce(Return(Thing::Core::DigitalValue::Low))
						.WillOnce(Return(Thing::Core::DigitalValue::High))
						.WillOnce(Return(Thing::Core::DigitalValue::High))
						.WillOnce(Return(Thing::Core::DigitalValue::Low))
						.WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

					EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High)).Times(1);
					Manager.Process();
					Manager.Process();

					EXPECT_CALL(output, DigitalWrite(_)).Times(0);
					Hardware->Delay(millis - 1);
					Manager.Process();

					EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low)).Times(1);
					Hardware->Delay(1);
					Manager.Process();
				}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputActiveFor)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const long time[] = { 1000, 2000, 3000 };
				for(int j = 0; j < sizeof(values)/sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));

						DigitalOutputMock output;

						Manager.AddDigitalInput(input);
						if(values[j] == Thing::Core::DigitalValue::High)
							Manager.OnActive(input).For(time[i]).SetHigh(output);
						else
							Manager.OnActive(input).For(time[i]).SetLow(output);

						EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						if (values[j] == Thing::Core::DigitalValue::High)
						{
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High));
							EXPECT_CALL(output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::Low))
								.WillOnce(Return(Thing::Core::DigitalValue::High));
						}
						else
						{
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low));
							EXPECT_CALL(output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::High))
								.WillOnce(Return(Thing::Core::DigitalValue::Low));
						}
						Hardware->Delay(1);
						EXPECT_CALL(output, DigitalWrite(_)).Times(0);
						EXPECT_CALL(output, GetState()).Times(0);
						Hardware->Delay(1);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputInactiveFor)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const long time[] = { 1000, 2000, 3000 };
				for (int j = 0; j < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));

						DigitalOutputMock output;

						Manager.AddDigitalInput(input);
						if (values[j] == Thing::Core::DigitalValue::High)
							Manager.OnInactive(input).For(time[i]).SetHigh(output);
						else
							Manager.OnInactive(input).For(time[i]).SetLow(output);

						EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						if (values[j] == Thing::Core::DigitalValue::High)
						{
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High));
							EXPECT_CALL(output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::Low))
								.WillOnce(Return(Thing::Core::DigitalValue::High));
						}
						else
						{
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low));
							EXPECT_CALL(output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::High))
								.WillOnce(Return(Thing::Core::DigitalValue::Low));
						}
						Hardware->Delay(1);
						EXPECT_CALL(output, DigitalWrite(_)).Times(0);
						EXPECT_CALL(output, GetState()).Times(0);
						Hardware->Delay(1);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputActiveForNotTriggered)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const long time[] = { 1000, 2000, 3000 };
				for (int j = 0; j < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));

						DigitalOutputMock output;

						Manager.AddDigitalInput(input);
						if (values[j] == Thing::Core::DigitalValue::High)
							Manager.OnActive(input).For(time[i]).SetHigh(output);
						else
							Manager.OnActive(input).For(time[i]).SetLow(output);

						EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(_)).Times(0);
						EXPECT_CALL(output, GetState()).Times(0);
						Hardware->Delay(1);
						Hardware->Delay(1);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputInactiveForNotTriggered)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const long time[] = { 1000, 2000, 3000 };
				for (int j = 0; j < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));

						DigitalOutputMock output;

						Manager.AddDigitalInput(input);
						if (values[j] == Thing::Core::DigitalValue::High)
							Manager.OnInactive(input).For(time[i]).SetHigh(output);
						else
							Manager.OnInactive(input).For(time[i]).SetLow(output);

						EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(_)).Times(0);
						EXPECT_CALL(output, GetState()).Times(0);
						Hardware->Delay(1);
						Hardware->Delay(1);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputActiveEach)
			{
				const long time[] = { 100, 200, 300 };
				const int testCount = 1;
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				for (int k = 0; k < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++k)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
						Manager.AddDigitalInput(input);

						DigitalOutputMock output;
						if (values[k] == Thing::Core::DigitalValue::High)
							Manager.OnActive(input).Each(time[i]).SetHigh(output);
						else
							Manager.OnActive(input).Each(time[i]).SetLow(output);

						for (int j = 0; j < testCount; ++j)
						{
							EXPECT_CALL(output, DigitalWrite(_)).Times(0);
							EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
							Manager.Process();
							Hardware->Delay(time[i] - 1);
							Manager.Process();
							if (values[k] == Thing::Core::DigitalValue::High)
							{
								EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High));
								EXPECT_CALL(output, GetState()).Times(2)
									.WillOnce(Return(Thing::Core::DigitalValue::Low))
									.WillOnce(Return(Thing::Core::DigitalValue::High));
							}
							else
							{
								EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low));
								EXPECT_CALL(output, GetState()).Times(2)
									.WillOnce(Return(Thing::Core::DigitalValue::High))
									.WillOnce(Return(Thing::Core::DigitalValue::Low));
							}
							Hardware->Delay(1);
							EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
							Manager.Process();
						}
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputInactiveEach)
			{
				const long time[] = { 100, 200, 300 };
				const int testCount = 1;
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				for (int k = 0; k < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++k)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalInputMock input;
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
						Manager.AddDigitalInput(input);

						DigitalOutputMock output;
						if (values[k] == Thing::Core::DigitalValue::High)
							Manager.OnInactive(input).Each(time[i]).SetHigh(output);
						else
							Manager.OnInactive(input).Each(time[i]).SetLow(output);

						for (int j = 0; j < testCount; ++j)
						{
							EXPECT_CALL(output, DigitalWrite(_)).Times(0);
							EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
							Manager.Process();
							Hardware->Delay(time[i] - 1);
							Manager.Process();
							if (values[k] == Thing::Core::DigitalValue::High)
							{
								EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High));
								EXPECT_CALL(output, GetState()).Times(2)
									.WillOnce(Return(Thing::Core::DigitalValue::Low))
									.WillOnce(Return(Thing::Core::DigitalValue::High));
							}
							else
							{
								EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low));
								EXPECT_CALL(output, GetState()).Times(2)
									.WillOnce(Return(Thing::Core::DigitalValue::High))
									.WillOnce(Return(Thing::Core::DigitalValue::Low));
							}
							Hardware->Delay(1);
							EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
							Manager.Process();
						}
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputActiveEachRunViaReference)
			{
				const long time[] = { 100, 200, 300 };
				const int testCount = 4;
				for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
				{
					Thing::Core::IOManager Manager;

					DigitalInputMock input;
					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
					Manager.AddDigitalInput(input);

					RunnableMock runnable;
					Manager.OnActive(input).Each(time[i]).Run(runnable);

					for (int j = 0; j < testCount; ++j)
					{
						EXPECT_CALL(runnable, Run()).Times(0);
						EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						EXPECT_CALL(runnable, Run()).Times(1);
						Hardware->Delay(1);
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
						Manager.Process();
					}
				}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputActiveEachRunViaPointer)
			{
				const long time[] = { 100, 200, 300 };
				const int testCount = 4;
				for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
				{
					Thing::Core::IOManager Manager;

					DigitalInputMock input;
					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
					Manager.AddDigitalInput(input);

					RunnableMock runnable;
					Manager.OnActive(input).Each(time[i]).Run(&runnable);

					for (int j = 0; j < testCount; ++j)
					{
						EXPECT_CALL(runnable, Run()).Times(0);
						EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						EXPECT_CALL(runnable, Run()).Times(1);
						Hardware->Delay(1);
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
						Manager.Process();
					}
				}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputInactiveEachRunViaReference)
			{
				const long time[] = { 100, 200, 300 };
				const int testCount = 4;
				for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
				{
					Thing::Core::IOManager Manager;

					DigitalInputMock input;
					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
					Manager.AddDigitalInput(input);

					RunnableMock runnable;
					Manager.OnInactive(input).Each(time[i]).Run(runnable);

					for (int j = 0; j < testCount; ++j)
					{
						EXPECT_CALL(runnable, Run()).Times(0);
						EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						EXPECT_CALL(runnable, Run()).Times(1);
						Hardware->Delay(1);
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
						Manager.Process();
					}
				}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorInputInactiveEachRunViaPointer)
			{
				const long time[] = { 100, 200, 300 };
				const int testCount = 4;
				for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
				{
					Thing::Core::IOManager Manager;

					DigitalInputMock input;
					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
					Manager.AddDigitalInput(input);

					RunnableMock runnable;
					Manager.OnInactive(input).Each(time[i]).Run(&runnable);

					for (int j = 0; j < testCount; ++j)
					{
						EXPECT_CALL(runnable, Run()).Times(0);
						EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						EXPECT_CALL(runnable, Run()).Times(1);
						Hardware->Delay(1);
						EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
						Manager.Process();
					}
				}
			}
#pragma endregion

#pragma region DigitalIOMonitor Output Test
			TEST_F(IOManagerTest, DigitalIOMonitorSetOutputActionSimpleViaReference)
			{
				Thing::Core::DigitalInputState states[] =
				{
					Thing::Core::DigitalInputState::WasActivated,
					Thing::Core::DigitalInputState::WasInactivated
				};

				Thing::Core::DigitalValue outputStates[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				for (int j = 0; j < sizeof(outputStates) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalInputState); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalOutputMock in_output;
						DigitalOutputMock output;
						EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.AddDigitalOutput(in_output);

						switch (states[i])
						{
						case Thing::Core::DigitalInputState::WasActivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnActivating(&in_output).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnActivating(&in_output).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnActivating(&in_output).Toggle(&output); break;
							}
							break;
						case Thing::Core::DigitalInputState::WasInactivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnInactivating(&in_output).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnInactivating(&in_output).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnInactivating(&in_output).Toggle(&output); break;
							}
							break;
						}

						EXPECT_CALL(in_output, GetState())
							.Times(8)
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::Low));

						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Inactive ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasActivated ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Active ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasInactivated ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
					}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorSetOutputActionSimpleViaPointer)
			{
				Thing::Core::DigitalInputState states[] =
				{
					Thing::Core::DigitalInputState::WasActivated,
					Thing::Core::DigitalInputState::WasInactivated
				};

				Thing::Core::DigitalValue outputStates[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				for (int j = 0; j < sizeof(outputStates) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalInputState); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalOutputMock in_output;
						DigitalOutputMock output;
						EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.AddDigitalOutput(in_output);

						switch (states[i])
						{
						case Thing::Core::DigitalInputState::WasActivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnActivating(&in_output).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnActivating(&in_output).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnActivating(&in_output).Toggle(&output); break;
							}
							break;
						case Thing::Core::DigitalInputState::WasInactivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnInactivating(&in_output).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnInactivating(&in_output).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnInactivating(&in_output).Toggle(&output); break;
							}
							break;
						}

						EXPECT_CALL(in_output, GetState())
							.Times(8)
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::Low));

						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Inactive ? 1 : 0);;
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasActivated ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Active ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasInactivated ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
					}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorSetOututActionMultipleOutputsTriggeringCorrectOutput)
			{
				Thing::Core::DigitalInputState states[] =
				{
					Thing::Core::DigitalInputState::WasActivated,
					Thing::Core::DigitalInputState::WasInactivated
				};

				Thing::Core::DigitalValue outputStates[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High, Thing::Core::DigitalValue::Toggle };
				for (int j = 0; j < sizeof(outputStates) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(states) / sizeof(Thing::Core::DigitalInputState); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalOutputMock in_output;
						DigitalOutputMock outputNever;
						DigitalOutputMock output;
						EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.AddDigitalOutput(in_output);
						Manager.AddDigitalOutput(outputNever);

						switch (states[i])
						{
						case Thing::Core::DigitalInputState::WasActivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnActivating(&in_output).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnActivating(&in_output).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnActivating(&in_output).Toggle(&output); break;
							}
							break;
						case Thing::Core::DigitalInputState::WasInactivated:
							switch (outputStates[j])
							{
							case Thing::Core::DigitalValue::Low: Manager.OnInactivating(&in_output).SetLow(&output); break;
							case Thing::Core::DigitalValue::High: Manager.OnInactivating(&in_output).SetHigh(&output); break;
							case Thing::Core::DigitalValue::Toggle: Manager.OnInactivating(&in_output).Toggle(&output); break;
							}
							break;
						}

						EXPECT_CALL(in_output, GetState())
							.Times(8)
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::Low));

						EXPECT_CALL(outputNever, GetState())
							.Times(8)
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillOnce(Return(Thing::Core::DigitalValue::Low));

						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Inactive ? 1 : 0);;
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
						Manager.DigitalWrite(outputNever, Thing::Core::DigitalValue::Low);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasActivated ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);
						Manager.DigitalWrite(outputNever, Thing::Core::DigitalValue::High);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::Active ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);
						Manager.DigitalWrite(outputNever, Thing::Core::DigitalValue::High);
						EXPECT_CALL(output, DigitalWrite(outputStates[j])).Times(states[i] == Thing::Core::DigitalInputState::WasInactivated ? 1 : 0);
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
						Manager.DigitalWrite(outputNever, Thing::Core::DigitalValue::Low);
					}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorSetOuputActionMultipleOutputsNotTriggeredCorrectOutput)
			{
				Thing::Core::IOManager Manager;

				DigitalOutputMock in_output;
				DigitalOutputMock outputChangingState;
				DigitalOutputMock output;
				Manager.AddDigitalOutput(in_output);
				Manager.AddDigitalOutput(outputChangingState);

				Manager.OnActivating(&in_output).Toggle(&output);

				EXPECT_CALL(in_output, GetState())
					.Times(8)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));

				EXPECT_CALL(outputChangingState, GetState())
					.Times(8)
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::Low))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::High))
					.WillOnce(Return(Thing::Core::DigitalValue::Low));

				EXPECT_CALL(output, DigitalWrite(_)).Times(0);
				Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
				Manager.DigitalWrite(outputChangingState, Thing::Core::DigitalValue::Low);

				Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
				Manager.DigitalWrite(outputChangingState, Thing::Core::DigitalValue::High);

				Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
				Manager.DigitalWrite(outputChangingState, Thing::Core::DigitalValue::High);

				Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
				Manager.DigitalWrite(outputChangingState, Thing::Core::DigitalValue::Low);
			}

			TEST_F(IOManagerTest, DigitalOutputMonitorForOutputMultipleTimes)
			{
				const int millis = 1000;
				const int triggerTimes = 3;

				Thing::Core::IOManager Manager;

				DigitalOutputMock in_output;
				DigitalOutputMock output;

				EXPECT_CALL(output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));

				Manager.AddDigitalOutput(in_output);

				Manager.OnActivating(in_output).SetHigh(&output).For(millis);

				for (int attempt = 0; attempt < triggerTimes; ++attempt)
				{
					EXPECT_CALL(in_output, GetState())
						.WillOnce(Return(Thing::Core::DigitalValue::Low))
						.WillOnce(Return(Thing::Core::DigitalValue::Low))
						.WillOnce(Return(Thing::Core::DigitalValue::Low))
						.WillOnce(Return(Thing::Core::DigitalValue::High));

					EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High)).Times(1);
					Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
					Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);

					EXPECT_CALL(output, DigitalWrite(_)).Times(0);
					Hardware->Delay(millis - 1);
					Manager.Process();

					EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low)).Times(1);
					Hardware->Delay(1);
					Manager.Process();
				}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorOutputActiveFor)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const long time[] = { 1000, 2000, 3000 };
				for (int j = 0; j < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalOutputMock in_output;
						DigitalOutputMock output;

						Manager.AddDigitalOutput(in_output);
						if (values[j] == Thing::Core::DigitalValue::High)
							Manager.OnActive(in_output).For(time[i]).SetHigh(output);
						else
							Manager.OnActive(in_output).For(time[i]).SetLow(output);

						EXPECT_CALL(in_output, GetState())
							.WillOnce(Return(Thing::Core::DigitalValue::Low))
							.WillRepeatedly(Return(Thing::Core::DigitalValue::High));
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						if (values[j] == Thing::Core::DigitalValue::High)
						{
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High));
							EXPECT_CALL(output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::Low))
								.WillOnce(Return(Thing::Core::DigitalValue::High));
						}
						else
						{
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low));
							EXPECT_CALL(output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::High))
								.WillOnce(Return(Thing::Core::DigitalValue::Low));
						}
						Hardware->Delay(1);
						EXPECT_CALL(output, DigitalWrite(_)).Times(0);
						EXPECT_CALL(output, GetState()).Times(0);
						Hardware->Delay(1);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorOutputInactiveFor)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const long time[] = { 1000, 2000, 3000 };
				for (int j = 0; j < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalOutputMock in_output;
						DigitalOutputMock output;

						Manager.AddDigitalOutput(in_output);
						if (values[j] == Thing::Core::DigitalValue::High)
							Manager.OnInactive(in_output).For(time[i]).SetHigh(output);
						else
							Manager.OnInactive(in_output).For(time[i]).SetLow(output);

						EXPECT_CALL(in_output, GetState())
							.WillOnce(Return(Thing::Core::DigitalValue::High))
							.WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						if (values[j] == Thing::Core::DigitalValue::High)
						{
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::High));
							EXPECT_CALL(output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::Low))
								.WillOnce(Return(Thing::Core::DigitalValue::High));
						}
						else
						{
							EXPECT_CALL(output, DigitalWrite(Thing::Core::DigitalValue::Low));
							EXPECT_CALL(output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::High))
								.WillOnce(Return(Thing::Core::DigitalValue::Low));
						}
						Hardware->Delay(1);
						EXPECT_CALL(output, DigitalWrite(_)).Times(0);
						EXPECT_CALL(output, GetState()).Times(0);
						Hardware->Delay(1);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorOutputActiveForNotTriggered)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const long time[] = { 1000, 2000, 3000 };
				for (int j = 0; j < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalOutputMock in_output;
						DigitalOutputMock output;

						Manager.AddDigitalOutput(in_output);
						if (values[j] == Thing::Core::DigitalValue::High)
							Manager.OnActive(in_output).For(time[i]).SetHigh(output);
						else
							Manager.OnActive(in_output).For(time[i]).SetLow(output);

						EXPECT_CALL(in_output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(_)).Times(0);
						EXPECT_CALL(output, GetState()).Times(0);
						Hardware->Delay(1);
						Hardware->Delay(1);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorOutputInactiveForNotTriggered)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low, Thing::Core::DigitalValue::High };
				const long time[] = { 1000, 2000, 3000 };
				for (int j = 0; j < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalOutputMock in_output;
						DigitalOutputMock output;

						Manager.AddDigitalOutput(in_output);
						if (values[j] == Thing::Core::DigitalValue::High)
							Manager.OnInactive(in_output).For(time[i]).SetHigh(output);
						else
							Manager.OnInactive(in_output).For(time[i]).SetLow(output);

						EXPECT_CALL(in_output, GetState()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
						Manager.Process();
						Hardware->Delay(time[i] - 1);
						Manager.Process();
						EXPECT_CALL(output, DigitalWrite(_)).Times(0);
						EXPECT_CALL(output, GetState()).Times(0);
						Hardware->Delay(1);
						Hardware->Delay(1);
						Manager.Process();
					}
			}

			TEST_F(IOManagerTest, ITimedDigitalIOMonitorOutputSelfTriggering)
			{
				Thing::Core::DigitalValue values[] = { Thing::Core::DigitalValue::Low };// , Thing::Core::DigitalValue::High
				const long time[] = { 1000 };// , 2000, 3000
				for (int j = 0; j < sizeof(values) / sizeof(Thing::Core::DigitalValue); ++j)
					for (int i = 0; i < sizeof(time) / sizeof(long); ++i)
					{
						Thing::Core::IOManager Manager;

						DigitalOutputMock in_output;

						Manager.AddDigitalOutput(in_output);
						if (values[j] == Thing::Core::DigitalValue::High)
						{
							Manager.OnActive(in_output).For(time[i]).SetLow(in_output);
							EXPECT_CALL(in_output, GetState())
								.Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::Low))
								.WillOnce(Return(Thing::Core::DigitalValue::High));

							Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::High);
						}
						else
						{
							Manager.OnInactive(in_output).For(time[i]).SetHigh(in_output);
							EXPECT_CALL(in_output, GetState())
								.Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::High))
								.WillOnce(Return(Thing::Core::DigitalValue::Low));

							Manager.DigitalWrite(in_output, Thing::Core::DigitalValue::Low);
						}

						if (values[j] == Thing::Core::DigitalValue::High)
						{
							EXPECT_CALL(in_output, DigitalWrite(Thing::Core::DigitalValue::Low));
							EXPECT_CALL(in_output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::High))
								.WillOnce(Return(Thing::Core::DigitalValue::Low));
						}
						else
						{
							EXPECT_CALL(in_output, DigitalWrite(Thing::Core::DigitalValue::High));
							EXPECT_CALL(in_output, GetState()).Times(2)
								.WillOnce(Return(Thing::Core::DigitalValue::Low))
								.WillOnce(Return(Thing::Core::DigitalValue::High));
						}
						Hardware->Delay(time[i]);
					}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorOnActivatingRunViaReference)
			{
				const int testCount = 4;
				Thing::Core::IOManager Manager;

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(input);

				RunnableMock runnable;
				Manager.OnActivating(input).Run(runnable);

				for (int i = 0; i < testCount; ++i)
				{
					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
					EXPECT_CALL(runnable, Run()).Times(0);
					Manager.Process();

					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
					EXPECT_CALL(runnable, Run()).Times(1);
					Manager.Process();

					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
					EXPECT_CALL(runnable, Run()).Times(0);
					Manager.Process();
				}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorOnActivatingRunViaPointer)
			{
				const int testCount = 4;
				Thing::Core::IOManager Manager;

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::Low));
				Manager.AddDigitalInput(input);

				RunnableMock runnable;
				Manager.OnActivating(input).Run(&runnable);

				for (int i = 0; i < testCount; ++i)
				{
					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
					EXPECT_CALL(runnable, Run()).Times(0);
					Manager.Process();

					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
					EXPECT_CALL(runnable, Run()).Times(1);
					Manager.Process();

					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
					EXPECT_CALL(runnable, Run()).Times(0);
					Manager.Process();
				}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorOnInactivatingRunViaReference)
			{
				const int testCount = 4;
				Thing::Core::IOManager Manager;

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalInput(input);

				RunnableMock runnable;
				Manager.OnInactivating(input).Run(runnable);
				for (int i = 0; i < testCount; ++i)
				{
					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
					EXPECT_CALL(runnable, Run()).Times(0);
					Manager.Process();

					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
					EXPECT_CALL(runnable, Run()).Times(1);
					Manager.Process();

					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
					EXPECT_CALL(runnable, Run()).Times(0);
					Manager.Process();
				}
			}

			TEST_F(IOManagerTest, DigitalIOMonitorOnInactivatingRunViaPointer)
			{
				const int testCount = 4;
				Thing::Core::IOManager Manager;

				DigitalInputMock input;
				EXPECT_CALL(input, DigitalRead()).WillRepeatedly(Return(Thing::Core::DigitalValue::High));
				Manager.AddDigitalInput(input);

				RunnableMock runnable;
				Manager.OnInactivating(input).Run(runnable);

				for (int i = 0; i < testCount; ++i)
				{
					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
					EXPECT_CALL(runnable, Run()).Times(0);
					Manager.Process();

					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::Low));
					EXPECT_CALL(runnable, Run()).Times(1);
					Manager.Process();

					EXPECT_CALL(input, DigitalRead()).WillOnce(Return(Thing::Core::DigitalValue::High));
					EXPECT_CALL(runnable, Run()).Times(0);
					Manager.Process();
				}
			}
#pragma endregion
		}
	}
}