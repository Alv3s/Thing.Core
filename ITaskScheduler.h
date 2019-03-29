#pragma once

#include "IRunnable.h"

namespace Thing
{
	namespace Core
	{
		class ITaskScheduler
		{
		public:
			virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) = 0;
			virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) = 0;

			virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) = 0;
			virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) = 0;

			virtual void Detach(Thing::Core::IRunnable* runnable) = 0;
			virtual void Detach(Thing::Core::IRunnable& runnable) = 0;
		};
	}
}