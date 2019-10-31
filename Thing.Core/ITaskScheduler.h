#pragma once

#include "IRunnable.h"

namespace Thing
{
	namespace Core
	{
		class ITaskScheduler
		{
		public:
			virtual ScheduledTask AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) = 0;
			virtual ScheduledTask AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) = 0;

			virtual ScheduledTask AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable) = 0;

			virtual ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) = 0;
			virtual ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) = 0;

			virtual ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable) = 0;

			virtual void Detach(ScheduledTask task) = 0;
		};
	}
}