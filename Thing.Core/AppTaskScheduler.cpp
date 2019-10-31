#include "AppTaskScheduler.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		AppTaskScheduler::AppTaskScheduler(IAppContainer& AppContainer) : app(&AppContainer)
		{
			AppContainer.AddListener(this);
		}

		AppTaskScheduler::~AppTaskScheduler()
		{
		}

		bool AppTaskScheduler::OnLoop()
		{
			unsigned long currentTimestamp = Hardware->Millis();
			for (auto i = tasks.begin(); i != tasks.end();)
			{
				if ((bool)(i->status & AppTaskStatus::ShouldDelete))
				{
					i = tasks.erase(i);
					continue;
				}

				if (currentTimestamp - i->scheduledTimestamp >= i->interval)
				{
					i->runnableFunc();
					if ((bool)(i->status & AppTaskStatus::Once))
						i = tasks.erase(i);
					else
					{
						i->scheduledTimestamp = currentTimestamp;
						++i;
					}
					continue;
				}
				++i;
			}
			return false;
		}

		ScheduledTask AppTaskScheduler::AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable)
		{
			AppScheduledTask task;
			task.status = AppTaskStatus::Once;
			task.scheduledTimestamp = Hardware->Millis();
			task.interval = milli;
			task.runnableFunc = std::bind(&IRunnable::Run, runnable);
			auto it = tasks.insert(tasks.end(), task);
			return &*it;
		}

		ScheduledTask AppTaskScheduler::AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable)
		{
			return AttachOnce(milli, &runnable);
		}

		ScheduledTask AppTaskScheduler::AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable)
		{
			AppScheduledTask task;
			task.status = AppTaskStatus::Once;
			task.scheduledTimestamp = Hardware->Millis();
			task.interval = milli;
			task.runnableFunc = runnable;
			auto it = tasks.insert(tasks.end(), task);
			return &*it;
		}

		ScheduledTask AppTaskScheduler::AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable)
		{
			AppScheduledTask task;
			task.status = AppTaskStatus::Periodic;
			task.scheduledTimestamp = Hardware->Millis();
			task.interval = milli;
			task.runnableFunc = std::bind(&IRunnable::Run, runnable);
			auto it = tasks.insert(tasks.end(), task);
			return &*it;
		}

		ScheduledTask AppTaskScheduler::AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable)
		{
			return AttachPeriodic(milli, &runnable);
		}

		ScheduledTask AppTaskScheduler::AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable)
		{
			AppScheduledTask task;
			task.status = AppTaskStatus::Periodic;
			task.scheduledTimestamp = Hardware->Millis();
			task.interval = milli;
			task.runnableFunc = runnable;
			auto it = tasks.insert(tasks.end(), task);
			return &*it;
		}

		void AppTaskScheduler::Detach(ScheduledTask task)
		{
			for (auto i = tasks.begin(); i != tasks.end(); ++i)
				if (&*i == task)
				{
					i->status = i->status | AppTaskStatus::ShouldDelete;
					return;
				}
		}
	}
}