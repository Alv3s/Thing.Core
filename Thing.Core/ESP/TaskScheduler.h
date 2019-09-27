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
					for (std::list<ScheduledTask*>::iterator it = periodicTasks.begin(); it != periodicTasks.end();)
					{
						deleteScheduledTask(*it);
						it = periodicTasks.erase(it);
					}
				}

				void AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					Detach(runnable);

					ScheduledTask* scheduled = createScheduledTask(runnable);
					scheduled->task->once_ms(milli, TaskScheduler::taskDelegate, scheduled);
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
					ScheduledTask* scheduled = createScheduledTask(runnable, obj);
					scheduled->task->once_ms(milli, [runnable, obj](){
						runnable(obj);
					});
				}

				void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					Detach(runnable);

					ScheduledTask* scheduled = createScheduledTask(runnable);
					scheduled->task->attach_ms(milli, TaskScheduler::taskDelegate, scheduled);
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
					ScheduledTask* scheduled = createScheduledTask(runnable, obj);
					scheduled->task->attach_ms(milli, [runnable, obj](){
						runnable(obj);
					});
				}

				void Detach(Thing::Core::IRunnable* runnable) override
				{
					for(std::list<ScheduledTask*>::iterator it = periodicTasks.begin(); it != periodicTasks.end(); ++it)
						if((*it)->runnable == runnable)
						{
							deleteScheduledTask(*it);
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
					Ticker* task;
				};

				static void taskDelegate(ScheduledTask* task)
				{
					task->runnable->Run();
				}

				ScheduledTask* createScheduledTask(Thing::Core::IRunnable* runnable)
				{
					ScheduledTask* task = new ScheduledTask;
					task->runnable = runnable;
					task->task = new Ticker;
					periodicTasks.push_back(task);
					return task;
				}

				ScheduledTask* createScheduledTask(Thing::Core::RunnableCallback runnable, void* obj)
				{
					ScheduledTask* task = new ScheduledTask;
					task->runnable = NULL;
					task->task = new Ticker;
					periodicTasks.push_back(task);
					return task;
				}

				void deleteScheduledTask(ScheduledTask* task)
				{
					task->task->detach();
					delete task->task;
					delete task;
				}

				std::list<ScheduledTask*> periodicTasks;
			};
		}
	}
}