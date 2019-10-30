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

				void AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					Detach(runnable);

					ScheduledTask& scheduled = createScheduledTask(runnable);
					scheduled.task.once_ms(milli, TaskScheduler::taskDelegate, &scheduled);
				}

				void AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					AttachOnce(milli, &runnable);
				}
				
				void AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable) 
				{
					AttachOnce(milli, runnable, NULL);
				}

				void AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable, void* obj)
				{
					ScheduledTask& scheduled = createScheduledTask(runnable, obj);
					scheduled.task.once_ms(milli, [runnable, obj](){
						runnable(obj);
					});
				}

				void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					Detach(runnable);

					ScheduledTask& scheduled = createScheduledTask(runnable);
					scheduled.task.attach_ms(milli, TaskScheduler::taskDelegate, &scheduled);
				}

				void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					AttachPeriodic(milli, &runnable);
				}

				void AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable)
				{
					AttachPeriodic(milli, runnable, NULL);
				}

				void AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable, void* obj)
				{
					ScheduledTask& scheduled = createScheduledTask(runnable, obj);
					scheduled.task.attach_ms(milli, [runnable, obj](){
						runnable(obj);
					});
				}

				void Detach(Thing::Core::IRunnable* runnable) override
				{
					for(auto it = periodicTasks.begin(); it != periodicTasks.end(); ++it)
						if(it->runnable == runnable)
						{
							it->task.detach();
							periodicTasks.erase(it);
							break;
						}
				}

				void Detach(Thing::Core::IRunnable& runnable) override
				{
					Detach(&runnable);
				}
			private:
				struct ScheduledTask
				{
					Thing::Core::IRunnable* runnable;
					Ticker task;
				};

				static void taskDelegate(ScheduledTask* task)
				{
					task->runnable->Run();
				}

				ScheduledTask& createScheduledTask(Thing::Core::IRunnable* runnable)
				{
					ScheduledTask task;
					task.runnable = runnable;
					auto it = periodicTasks.insert(periodicTasks.end(), task);
					return *it;
				}

				ScheduledTask& createScheduledTask(Thing::Core::RunnableCallback runnable, void* obj)
				{
					ScheduledTask task;
					task.runnable = NULL;
					auto it = periodicTasks.insert(periodicTasks.end(), task);
					return *it;
				}

				std::list<ScheduledTask> periodicTasks;
			};
		}
	}
}