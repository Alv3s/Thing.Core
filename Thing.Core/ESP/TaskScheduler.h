#pragma once

#include "Ticker.h"
#include "../ITaskScheduler.h"
#include <map>

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
					for (std::map<IRunnable*, ScheduledTask*>::iterator it = periodicTasks.begin(); it != periodicTasks.end();)
					{
						deleteScheduledTask(it->second);
						it = periodicTasks.erase(it);
					}
				}

				void AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					Detach(runnable);

					ScheduledTask* scheduled = createScheduledTask(runnable);
					scheduled->task->attach(milli, ESP8266TaskScheduler::taskDelegate, scheduled);
				}

				void AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					AttachOnce(milli, &runnable);
				}

				void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					Detach(runnable);

					ScheduledTask* scheduled = createScheduledTask(runnable);
					scheduled->task->attach_ms(milli, ESP8266TaskScheduler::taskDelegate, scheduled);
				}

				void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					AttachPeriodic(milli, &runnable);
				}

				void Detach(Thing::Core::IRunnable* runnable) override
				{
					std::map<IRunnable*, ScheduledTask*>::iterator it = periodicTasks.find(runnable);
					if (it != periodicTasks.end())
						deleteScheduledTask(it->second);
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
					periodicTasks.insert(std::pair<IRunnable*, ScheduledTask*>(runnable, task));
					return task;
				}

				void deleteScheduledTask(ScheduledTask* task)
				{
					task->task->detach();
					delete task->task;
					delete task;
				}

				std::map<Thing::Core::IRunnable*, ScheduledTask*> periodicTasks;
			};
		}
	}
}