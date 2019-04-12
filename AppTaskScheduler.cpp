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
			for(std::list<ScheduledTask>::iterator i = tasks.begin(); i != tasks.end();)
			{
				if (currentTimestamp - i->scheduledTimestamp >= i->interval)
				{
					i->runnable->Run();
					if (i->once)
						tasks.erase(i++);
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

		void AppTaskScheduler::AttachOnce(unsigned long milli, Thing::Core::IRunnable * runnable)
		{
			ScheduledTask task;
			task.once = true;
			task.scheduledTimestamp = Hardware->Millis();
			task.interval = milli;
			task.runnable = runnable;
			tasks.push_back(task);
		}

		void AppTaskScheduler::AttachOnce(unsigned long milli, Thing::Core::IRunnable & runnable)
		{
			AttachOnce(milli, &runnable);
		}

		void AppTaskScheduler::AttachPeriodic(unsigned long milli, Thing::Core::IRunnable * runnable)
		{
			ScheduledTask task;
			task.once = false;
			task.scheduledTimestamp = Hardware->Millis();
			task.interval = milli;
			task.runnable = runnable;
			tasks.push_back(task);
		}

		void AppTaskScheduler::AttachPeriodic(unsigned long milli, Thing::Core::IRunnable & runnable)
		{
			AttachPeriodic(milli, &runnable);
		}

		void AppTaskScheduler::Detach(Thing::Core::IRunnable * runnable)
		{
			for (std::list<ScheduledTask>::iterator i = tasks.begin(); i != tasks.end(); ++i)
				if(i->runnable == runnable)
				{
					tasks.erase(i);
					return;
				}
		}

		void AppTaskScheduler::Detach(Thing::Core::IRunnable & runnable)
		{
			Detach(&runnable);
		}
	}
}