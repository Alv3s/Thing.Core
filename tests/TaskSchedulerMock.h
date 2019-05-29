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
					for (auto t : tasks)
						delete t;
				}

				virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					PeriodicTask* task = new PeriodicTask;
					task->next_run_at = Hardware->Millis() + milli;
					task->period_ms = 0;
					task->runnable = TaskSchedulerMock::RunTask;
					task->obj = runnable;
					task->shouldBeDeleted = false;

					tasks.push_back(task);
				}
				virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					AttachOnce(milli, &runnable);
				}
				virtual void AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable) override
				{
					AttachOnce(milli, runnable, NULL);
				}
				virtual void AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable, void* obj) override
				{
					PeriodicTask* task = new PeriodicTask;
					task->next_run_at = Hardware->Millis() + milli;
					task->period_ms = 0;
					task->runnable = runnable;
					task->obj = obj;
					task->shouldBeDeleted = false;

					tasks.push_back(task);
				}

				virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override
				{
					PeriodicTask* task = new PeriodicTask;
					task->next_run_at = Hardware->Millis() + milli;
					task->period_ms = milli;
					task->runnable = TaskSchedulerMock::RunTask;
					task->obj = runnable;
					task->shouldBeDeleted = false;

					tasks.push_back(task);
				}
				virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override
				{
					AttachPeriodic(milli, &runnable);
				}
				virtual void AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable) override
				{
					AttachPeriodic(milli, runnable, NULL);
				}

				virtual void AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable, void* obj) override
				{
					PeriodicTask* task = new PeriodicTask;
					task->next_run_at = Hardware->Millis() + milli;
					task->period_ms = milli;
					task->runnable = runnable;
					task->obj = obj;
					task->shouldBeDeleted = false;

					tasks.push_back(task);
				}

				virtual void Detach(Thing::Core::IRunnable* runnable) override
				{
					std::list<PeriodicTask*> removeTasks;
					for (auto t : tasks)
						if (t->obj == runnable)
						{
							t->shouldBeDeleted = true;
							break;
						}
				}

				virtual void Detach(Thing::Core::IRunnable& runnable) override
				{
					Detach(&runnable);
				}

				virtual void OnMillisChange(unsigned long millis) override
				{
					for (auto it = tasks.begin(); it != tasks.end();)
					{
						auto t = *it;
						if (t->shouldBeDeleted)
						{
							it = tasks.erase(it);
							delete t;
							continue;
						}
						else if (t->next_run_at == millis)
						{
							t->runnable(t->obj);
							if (t->period_ms > 0)
								t->next_run_at = millis + t->period_ms;
							else
								t->shouldBeDeleted = true;
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
					void* obj;
					bool shouldBeDeleted;
				};

				static void RunTask(void* obj)
				{
					IRunnable* runnable = static_cast<IRunnable*>(obj);
					runnable->Run();
				}

				std::list<PeriodicTask*> tasks;
			};
		}
	}
}