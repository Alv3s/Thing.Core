#pragma once

#include "../Thing.Core/ITaskScheduler.h"
#include <list>
#include "IMillisListener.h"
#include "../Thing.Core/IoC.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class TaskSchedulerMock : public virtual Thing::Core::ITaskScheduler, public virtual IMillisListener
			{
			public:
				~TaskSchedulerMock()
				{
				}

				virtual ScheduledTask AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					PeriodicTask task;
					task.next_run_at = Hardware->Millis() + milli;
					task.period_ms = 0;
					task.runnable = std::bind(&IRunnable::Run, runnable);
					task.shouldBeDeleted = false;

					auto it = tasks.insert(tasks.end(), task);
					return &*it;
				}

				virtual ScheduledTask AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					return AttachOnce(milli, &runnable);
				}

				virtual ScheduledTask AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable) override
				{
					PeriodicTask task;
					task.next_run_at = Hardware->Millis() + milli;
					task.period_ms = 0;
					task.runnable = runnable;
					task.shouldBeDeleted = false;

					auto it = tasks.insert(tasks.end(), task);
					return &*it;
				}

				virtual ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					PeriodicTask task;
					task.next_run_at = Hardware->Millis() + milli;
					task.period_ms = milli;
					task.runnable = std::bind(&IRunnable::Run, runnable);
					task.shouldBeDeleted = false;

					auto it = tasks.insert(tasks.end(), task);
					return &*it;
				}

				virtual ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					return AttachPeriodic(milli, &runnable);
				}

				virtual ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable) override
				{
					PeriodicTask task;
					task.next_run_at = Hardware->Millis() + milli;
					task.period_ms = milli;
					task.runnable = runnable;
					task.shouldBeDeleted = false;

					auto it = tasks.insert(tasks.end(), task);
					return &*it;
				}

				virtual void Detach(ScheduledTask task) override
				{
					for(auto t = tasks.begin(); t != tasks.end(); ++t)
						if (&*t == task)
						{
							t->shouldBeDeleted = true;
							break;
						}
				}

				virtual void OnMillisChange(unsigned long millis) override
				{
					for (auto it = tasks.begin(); it != tasks.end();)
					{
						if (it->shouldBeDeleted)
						{
							it = tasks.erase(it);
							continue;
						}
						else if (it->next_run_at == millis)
						{
							it->runnable();
							if (it->period_ms > 0)
								it->next_run_at = millis + it->period_ms;
							else
								it->shouldBeDeleted = true;
						}
						++it;
					}
				}
			private:
				class PeriodicTask
				{
				public:
					unsigned long next_run_at;
					unsigned long period_ms;
					Thing::Core::RunnableCallback runnable;
					bool shouldBeDeleted;
				};

				std::list<PeriodicTask> tasks;
			};
		}
	}
}