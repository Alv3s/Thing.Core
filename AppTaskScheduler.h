#pragma once

#include "ITaskScheduler.h"
#include "IAppContainer.h"
#include <list>

namespace Thing
{
	namespace Core
	{		
		/// <summary>
		/// A Default non-blocking Task Scheduler. It schedules tasks by running them After an IApp Loop via IAppListener. 
		/// </summary>
		/// <seealso cref="ITaskScheduler" />
		/// <seealso cref="IAppListener" />
		class AppTaskScheduler : public virtual ITaskScheduler, public virtual IAppListener
		{
		public:			
			/// <summary>
			/// Initializes a new instance of the <see cref="AppTaskScheduler"/> class.
			/// </summary>
			/// <param name="AppContainer">The IAppContainer object on which the tasks will be run.</param>
			AppTaskScheduler(IAppContainer& AppContainer);			
			/// <summary>
			/// Finalizes an instance of the <see cref="AppTaskScheduler"/> class.
			/// </summary>
			~AppTaskScheduler();
			
			/// <summary>
			/// Called automatically whdn IApp Loop is finishing.
			/// </summary>
			/// <returns>false if it should still run or true if it shouldn't be called anymore by the parent IApp.</returns>
			virtual bool OnLoop() override;
			

			/// <summary>
			/// Attaches a Task to run only once after a period of time.
			/// </summary>
			/// <param name="milli">The millisseconds this task will have to wait until it runs.</param>
			/// <param name="runnable">The runnable task.</param>
			virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override;
			/// <summary>
			/// Attaches a Task to run only once after a period of time.
			/// </summary>
			/// <param name="milli">The millisseconds this task will have to wait until it runs.</param>
			/// <param name="runnable">The runnable task.</param>
			virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override;
			
			/// <summary>
			/// Attaches a Task to run periodically.
			/// </summary>
			/// <param name="milli">The millisseconds interval which this app will periodically run.</param>
			/// <param name="runnable">The runnable task.</param>
			virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override;
			/// <summary>
			/// Attaches a Task to run periodically.
			/// </summary>
			/// <param name="milli">The millisseconds interval which this app will periodically run.</param>
			/// <param name="runnable">The runnable task.</param>
			virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override;
			
			/// <summary>
			/// Detaches a task, cancelling further executions.
			/// </summary>
			/// <param name="runnable">The runnable task being cancelled.</param>
			virtual void Detach(Thing::Core::IRunnable* runnable) override;
			/// <summary>
			/// Detaches a task, cancelling further executions.
			/// </summary>
			/// <param name="runnable">The runnable task being cancelled.</param>
			virtual void Detach(Thing::Core::IRunnable& runnable) override;
		private:
			const IAppContainer* app;

			struct ScheduledTask
			{
				unsigned long scheduledTimestamp;
				unsigned long interval;
				bool once;
				IRunnable* runnable;
			};
			std::list<ScheduledTask> tasks;
		};
	}
}
