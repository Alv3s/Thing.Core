#pragma once

#include "../ITaskScheduler.h"
#include <list>
#include "IMillisListener.h"
#include "../IoC.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class TaskSchedulerMock : public virtual Thing::Core::ITaskScheduler, public virtual IMillisListener
			{
			public:
				~TaskSchedulerMock()
				{
					for (auto t : tasks)
						delete t;
				}

				virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					PeriodicTask* task = new PeriodicTask;
					task->next_run_at = Hardware->Millis() + milli;
					task->period_ms = 0;
					task->runnable = runnable;

					tasks.push_back(task);
				}
				virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					AttachOnce(milli, &runnable);
				}

				virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					PeriodicTask* task = new PeriodicTask;
					task->next_run_at = Hardware->Millis() + milli;
					task->period_ms = milli;
					task->runnable = runnable;

					tasks.push_back(task);
				}
				virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					AttachPeriodic(milli, &runnable);
				}

				virtual void Detach(Thing::Core::IRunnable* runnable) override
				{
					std::list<PeriodicTask*> removeTasks;
					for (auto t : tasks)
						if (t->runnable == runnable)
							removeTasks.push_back(t);

					for (auto t : removeTasks)
					{
						tasks.remove(t);
						delete t;
					}
				}
				virtual void Detach(Thing::Core::IRunnable& runnable) override
				{
					Detach(&runnable);
				}

				virtual void OnMillisChange(unsigned long millis) override
				{
					std::list<Thing::Core::IRunnable*> detachRunnables;

					for (auto t : tasks)
						if (t->next_run_at == millis)
						{
							t->runnable->Run();
							if (t->period_ms > 0)
								t->next_run_at = millis + t->period_ms;
							else
								detachRunnables.push_back(t->runnable);
						}

					for (auto& r : detachRunnables)
						Detach(r);
				}
			private:
				class PeriodicTask
				{
				public:
					unsigned long next_run_at;
					unsigned long period_ms;
					Thing::Core::IRunnable* runnable;
				};

				std::list<PeriodicTask*> tasks;
			};
		}
	}
}