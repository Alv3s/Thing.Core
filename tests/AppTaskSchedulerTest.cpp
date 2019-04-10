#include "AppTaskSchedulerTest.h"
#include "../IoC.h"
#include "RunnableMock.h"
#include "../AppTaskScheduler.h"

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
				EXPECT_CALL(appMock, AddListener(An<IAppListener*>())).Times(1);
				AppTaskScheduler taskScheduler(appMock);
			}

			/// <summary>
			/// Tests if a task when scheduled, will be run once after the time has pass.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, AttachOnceViaReferenceTest)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appMock);

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

				//Checks if the task will run anymore
				EXPECT_CALL(runnable, Run()).Times(0);
				hardwareMock.Delay(2 * millis);
				taskScheduler.OnLoop();
			}

			/// <summary>
			/// Tests if a task when scheduled, will be run once after the time has pass.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, AttachOnceViaPointerTest)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appMock);

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

				AppTaskScheduler taskScheduler(appMock);

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

				AppTaskScheduler taskScheduler(appMock);

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
			/// Tests if a task is detached. It will schedule a periodic taks and cancel it to be sure that the task was detached.
			/// </summary>
			TEST_F(AppTaskSchedulerTest, DetachViaReferenceTest)
			{
				const int millis = 100;

				AppTaskScheduler taskScheduler(appMock);

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

				AppTaskScheduler taskScheduler(appMock);

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

				AppTaskScheduler taskScheduler(appMock);

				RunnableMock runnable;
				taskScheduler.AttachPeriodic(millis, runnable);

				ASSERT_NO_FATAL_FAILURE(taskScheduler.Detach(NULL));
			}
		}
	}
}