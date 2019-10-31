#include "AppTaskSchedulerTest.h"
#include "../Thing.Core/IoC.h"
#include "RunnableMock.h"
#include "../Thing.Core/AppTaskScheduler.h"

using ::testing::Return;
using ::testing::_;
using ::testing::An;

namespace Thing {
	namespace Core {
		namespace Tests {
			void AppTaskSchedulerTest::SetUp()
			{
				Hardware = &hardwareMock;
			}

			void AppTaskSchedulerTest::TearDown()
			{
			}
			
			/// <summary>
			/// Tests if AppTaskScheduler is added as listener to the AbstractApp.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, AddedAsListenerTest)
			{
				EXPECT_CALL(appContainerMock, AddListener(An<IAppListener*>())).Times(1);
				AppTaskScheduler taskScheduler(appContainerMock);
			}

			/// <summary>
			/// Tests if a task when scheduled, will be run once after the time has pass.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, AttachOnceViaReferenceTest)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appContainerMock);

				RunnableMock runnable;
				taskScheduler.AttachOnce(millis, runnable);

				//Checks if the task will not run right away
				EXPECT_CALL(runnable, Run()).Times(0);
				taskScheduler.OnLoop();

				//Checks if the task will not run before the time has passed
				EXPECT_CALL(runnable, Run()).Times(0);
				hardwareMock.Delay(millis - 1);
				taskScheduler.OnLoop();

				//Checks if the task will run at least exactly after the interval has passed
				EXPECT_CALL(runnable, Run()).Times(1);
				hardwareMock.Delay(1);
				taskScheduler.OnLoop();

				//Checks if the task will not run anymore
				EXPECT_CALL(runnable, Run()).Times(0);
				hardwareMock.Delay(2 * millis);
				taskScheduler.OnLoop();
			}

			TEST_F(AppTaskSchedulerTest, AttachOnceRunnableCallback)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appContainerMock);

				testing::MockFunction<void()> callbackFunctionMock;
				taskScheduler.AttachOnce(millis, [&callbackFunctionMock]() {
					callbackFunctionMock.Call();
				});

				//Checks if the task will not run right away
				EXPECT_CALL(callbackFunctionMock, Call()).Times(0);
				taskScheduler.OnLoop();

				//Checks if the task will not run before the time has passed
				EXPECT_CALL(callbackFunctionMock, Call()).Times(0);
				hardwareMock.Delay(millis - 1);
				taskScheduler.OnLoop();

				//Checks if the task will run at least exactly after the interval has passed
				EXPECT_CALL(callbackFunctionMock, Call()).Times(1);
				hardwareMock.Delay(1);
				taskScheduler.OnLoop();

				//Checks if the task will not run anymore
				EXPECT_CALL(callbackFunctionMock, Call()).Times(0);
				hardwareMock.Delay(2 * millis);
				taskScheduler.OnLoop();
			}

			/// <summary>
			/// Tests if a task when scheduled, will be run once after the time has pass.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, AttachOnceViaPointerTest)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appContainerMock);

				RunnableMock runnable;
				taskScheduler.AttachOnce(millis, &runnable);

				//Checks if the task will not run right away
				EXPECT_CALL(runnable, Run()).Times(0);
				taskScheduler.OnLoop();

				//Checks if the task will not run before the time has passed
				EXPECT_CALL(runnable, Run()).Times(0);
				hardwareMock.Delay(millis - 1);
				taskScheduler.OnLoop();

				//Checks if the task will run at least exactly after the interval has passed
				EXPECT_CALL(runnable, Run()).Times(1);
				hardwareMock.Delay(1);
				taskScheduler.OnLoop();

				//Checks if the task will run anymore
				EXPECT_CALL(runnable, Run()).Times(0);
				hardwareMock.Delay(2 * millis);
				taskScheduler.OnLoop();
			}

			/// <summary>
			/// Tests if a task when scheduled, will be run periodically after the time has pass.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, AttachPeriodicViaReferenceTest)
			{
				const int millis = 100;
				const int retries = 4;

				AppTaskScheduler taskScheduler(appContainerMock);

				RunnableMock runnable;
				taskScheduler.AttachPeriodic(millis, runnable);

				//Checks if the task will not run right away
				EXPECT_CALL(runnable, Run()).Times(0);
				taskScheduler.OnLoop();

				//Checks if the task will not run before the time has passed
				EXPECT_CALL(runnable, Run()).Times(0);
				hardwareMock.Delay(millis - 1);
				taskScheduler.OnLoop();

				//Checks if the task will run at least exactly after the interval has passed
				EXPECT_CALL(runnable, Run()).Times(1);
				hardwareMock.Delay(1);
				taskScheduler.OnLoop();
				
				for (int i = 0; i < retries; ++i)
				{
					//Checks if the task will not run before the time has passed
					EXPECT_CALL(runnable, Run()).Times(0);
					hardwareMock.Delay(millis - 1);
					taskScheduler.OnLoop();

					//Checks if the task will run at least exactly after the interval has passed
					EXPECT_CALL(runnable, Run()).Times(1);
					hardwareMock.Delay(1);
					taskScheduler.OnLoop();
				}
			}

			/// <summary>
			/// Tests if a task when scheduled, will be run periodically after the time has pass.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, AttachPeriodicViaPointerTest)
			{
				const int millis = 100;
				const int retries = 4;

				AppTaskScheduler taskScheduler(appContainerMock);

				RunnableMock runnable;
				taskScheduler.AttachPeriodic(millis, &runnable);

				//Checks if the task will not run right away
				EXPECT_CALL(runnable, Run()).Times(0);
				taskScheduler.OnLoop();

				//Checks if the task will not run before the time has passed
				EXPECT_CALL(runnable, Run()).Times(0);
				hardwareMock.Delay(millis - 1);
				taskScheduler.OnLoop();

				//Checks if the task will run at least exactly after the interval has passed
				EXPECT_CALL(runnable, Run()).Times(1);
				hardwareMock.Delay(1);
				taskScheduler.OnLoop();

				for (int i = 0; i < retries; ++i)
				{
					//Checks if the task will not run before the time has passed
					EXPECT_CALL(runnable, Run()).Times(0);
					hardwareMock.Delay(millis - 1);
					taskScheduler.OnLoop();

					//Checks if the task will run at least exactly after the interval has passed
					EXPECT_CALL(runnable, Run()).Times(1);
					hardwareMock.Delay(1);
					taskScheduler.OnLoop();
				}
			}

			/// <summary>
			/// Tests if a task when scheduled, will be run periodically after the time has pass.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, AttachPeriodicRunnableCallback)
			{
				const int millis = 100;
				const int retries = 4;

				AppTaskScheduler taskScheduler(appContainerMock);

				testing::MockFunction<void()> callbackFunctionMock;
				taskScheduler.AttachPeriodic(millis, [&callbackFunctionMock]() {
					callbackFunctionMock.Call();
				});

				//Checks if the task will not run right away
				EXPECT_CALL(callbackFunctionMock, Call()).Times(0);
				taskScheduler.OnLoop();

				//Checks if the task will not run before the time has passed
				EXPECT_CALL(callbackFunctionMock, Call()).Times(0);
				hardwareMock.Delay(millis - 1);
				taskScheduler.OnLoop();

				//Checks if the task will run at least exactly after the interval has passed
				EXPECT_CALL(callbackFunctionMock, Call()).Times(1);
				hardwareMock.Delay(1);
				taskScheduler.OnLoop();

				for (int i = 0; i < retries; ++i)
				{
					//Checks if the task will not run before the time has passed
					EXPECT_CALL(callbackFunctionMock, Call()).Times(0);
					hardwareMock.Delay(millis - 1);
					taskScheduler.OnLoop();

					//Checks if the task will run at least exactly after the interval has passed
					EXPECT_CALL(callbackFunctionMock, Call()).Times(1);
					hardwareMock.Delay(1);
					taskScheduler.OnLoop();
				}
			}

			/// <summary>
			/// Tests if a task is detached. It will schedule a periodic taks and cancel it to be sure that the task was detached.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, DetachViaReferenceTest)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appContainerMock);

				RunnableMock runnable;
				taskScheduler.AttachPeriodic(millis, runnable);

				//Checks if the task will run at least exactly after the interval has passed
				EXPECT_CALL(runnable, Run()).Times(1);
				hardwareMock.Delay(millis);
				taskScheduler.OnLoop();

				//Detaches the task and expects it to never be called again
				EXPECT_CALL(runnable, Run()).Times(0);
				taskScheduler.Detach(runnable);
				hardwareMock.Delay(2 * millis);
				taskScheduler.OnLoop();
			}

			/// <summary>
			/// Tests if a task is detached. It will schedule a periodic taks and cancel it to be sure that the task was detached.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, DetachViaPointerTest)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appContainerMock);

				RunnableMock runnable;
				taskScheduler.AttachPeriodic(millis, runnable);

				//Checks if the task will run at least exactly after the interval has passed
				EXPECT_CALL(runnable, Run()).Times(1);
				hardwareMock.Delay(millis);
				taskScheduler.OnLoop();

				//Detaches the task and expects it to never be called again
				EXPECT_CALL(runnable, Run()).Times(0);
				taskScheduler.Detach(runnable);
				hardwareMock.Delay(2 * millis);
				taskScheduler.OnLoop();
			}

			/// <summary>
			/// Tests if a null task doesn't crash detach method.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, DetachNullPointerTest)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appContainerMock);

				RunnableMock runnable;
				taskScheduler.AttachPeriodic(millis, runnable);

				ASSERT_NO_FATAL_FAILURE(taskScheduler.Detach((Thing::Core::IRunnable*)NULL));
			}

			TEST_F(AppTaskSchedulerTest, DetachRunonceTaskInsideOwnRun)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appContainerMock);

				RunnableMock runnable;
				taskScheduler.AttachOnce(millis, runnable);

				ON_CALL(runnable, Run()).WillByDefault(testing::Invoke(
					[&taskScheduler, &runnable]()
					{
						taskScheduler.Detach(runnable);
					}
				));

				Hardware->Delay(millis);
				ASSERT_NO_FATAL_FAILURE(taskScheduler.OnLoop());
			}
		}
	}
}