#pragma once

#include "Ticker.h"
#include "../ITaskScheduler.h"
#include <list>

namespace Thing
{
	namespace Core
	{
		namespace ESP
		{
			class TaskScheduler : public virtual Thing::Core::ITaskScheduler
			{
			public:
				TaskScheduler()
				{

				}

				~TaskScheduler()
				{
					for(auto& t : periodicTasks)
						t.task.detach();
				}

				ScheduledTask AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					TickerScheduledTask& scheduled = createScheduledTask(runnable);
					scheduled.task.once_ms(milli, TaskScheduler::taskDelegate, &scheduled);
					return &scheduled.task;
				}

				ScheduledTask AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					return AttachOnce(milli, &runnable);
				}
				
				ScheduledTask AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable) override
				{
					TickerScheduledTask& scheduled = createScheduledTask(runnable);
					scheduled.task.once_ms(milli, [runnable](){
						runnable();
					});
					return &scheduled.task;
				}

				ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					TickerScheduledTask& scheduled = createScheduledTask(runnable);
					scheduled.task.attach_ms(milli, TaskScheduler::taskDelegate, &scheduled);
					return &scheduled.task;
				}

				ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					return AttachPeriodic(milli, &runnable);
				}

				ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable) override
				{
					TickerScheduledTask& scheduled = createScheduledTask(runnable);
					scheduled.task.attach_ms(milli, [runnable](){
						runnable();
					});
					return &scheduled.task;
				}

				void Detach(Thing::Core::ScheduledTask task) override
				{
					for(auto it = periodicTasks.begin(); it != periodicTasks.end(); ++it)
						if(&it->task == task)
						{
							it->task.detach();
							periodicTasks.erase(it);
							break;
						}
				}
			private:
				struct TickerScheduledTask
				{
					Thing::Core::IRunnable* runnable;
					Ticker task;
				};

				static void taskDelegate(TickerScheduledTask* task)
				{
					task->runnable->Run();
				}

				TickerScheduledTask& createScheduledTask(Thing::Core::IRunnable* runnable)
				{
					TickerScheduledTask task;
					task.runnable = runnable;
					auto it = periodicTasks.insert(periodicTasks.end(), task);
					return *it;
				}

				TickerScheduledTask& createScheduledTask(Thing::Core::RunnableCallback runnable)
				{
					TickerScheduledTask task;
					task.runnable = NULL;
					auto it = periodicTasks.insert(periodicTasks.end(), task);
					return *it;
				}

				std::list<TickerScheduledTask> periodicTasks;
			};
		}
	}
}