#pragma once

#include "ITaskScheduler.h"
#include "IAppContainer.h"
#include <list>

namespace Thing
{
	namespace Core
	{
		enum class AppTaskStatus : uint8_t
		{
			Periodic = 0b1,
			Once = 0b10,
			ShouldDelete = 0b100
		};

		inline AppTaskStatus operator|(AppTaskStatus a, AppTaskStatus b)
		{
			return static_cast<AppTaskStatus>(static_cast<int>(a) | static_cast<int>(b));
		}

		inline AppTaskStatus operator&(AppTaskStatus a, AppTaskStatus b)
		{
			return static_cast<AppTaskStatus>(static_cast<int>(a) & static_cast<int>(b));
		}

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
			bool OnLoop() override;


			/// <summary>
			/// Attaches a Task to run only once after a period of time.
			/// </summary>
			/// <param name="milli">The millisseconds this task will have to wait until it runs.</param>
			/// <param name="runnable">The runnable task.</param>
			ScheduledTask AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override;
			/// <summary>
			/// Attaches a Task to run only once after a period of time.
			/// </summary>
			/// <param name="milli">The millisseconds this task will have to wait until it runs.</param>
			/// <param name="runnable">The runnable task.</param>
			ScheduledTask AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override;

			ScheduledTask AttachOnce(unsigned long milli, Thing::Core::RunnableCallback runnable) override;

			/// <summary>
			/// Attaches a Task to run periodically.
			/// </summary>
			/// <param name="milli">The millisseconds interval which this app will periodically run.</param>
			/// <param name="runnable">The runnable task.</param>
			ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override;
			/// <summary>
			/// Attaches a Task to run periodically.
			/// </summary>
			/// <param name="milli">The millisseconds interval which this app will periodically run.</param>
			/// <param name="runnable">The runnable task.</param>
			ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override;

			ScheduledTask AttachPeriodic(unsigned long milli, Thing::Core::RunnableCallback runnable) override;

			/// <summary>
			/// Detaches a task, cancelling further executions.
			/// </summary>
			/// <param name="task">The scheduled task being cancelled.</param>
			void Detach(ScheduledTask task) override;
		private:
			const IAppContainer* app;

			struct AppScheduledTask
			{
				unsigned long scheduledTimestamp;
				unsigned long interval;
				AppTaskStatus status;
				RunnableCallback runnableFunc;
			};
			std::list<AppScheduledTask> tasks;
		};
	}
}
