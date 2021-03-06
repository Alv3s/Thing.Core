#pragma once

#include "IDigitalIOListeners.h"
#include "IDigitalInput.h"
#include "IDigitalIOMonitor.h"
#include "IRunnable.h"

namespace Thing
{
	namespace Core
	{
		class DigitalIOMonitor :	public virtual IDigitalIOMonitor, 
									public virtual ITimedDigitalIOMonitor, 
									public virtual IActionableIOMonitor,
									private virtual IRunnable, 
									private virtual IDigitalInputListener, 
									private virtual IDigitalOutputListener
		{
			friend class IOManager;
		public:
			~DigitalIOMonitor();

			void Toggle(IDigitalOutput& output) override;
			void Toggle(IDigitalOutput* output) override;

			IDigitalOutputMonitor& SetHigh(IDigitalOutput& output) override;
			IDigitalOutputMonitor& SetHigh(IDigitalOutput* output) override;

			IDigitalOutputMonitor& SetLow(IDigitalOutput& output) override;
			IDigitalOutputMonitor& SetLow(IDigitalOutput* output) override;

			IDigitalIOMonitor& For(int millis) override;
			IDigitalIOMonitor& Each(int millis) override;

			void Run(IRunnable& runnable) override;
			void Run(IRunnable* runnable) override;
			void Run(RunnableCallback f) override;

		private:
			DigitalIOMonitor(class IOManager& manager, IDigitalInput* input, DigitalInputState action);
			DigitalIOMonitor(class IOManager& manager, IDigitalOutput* output, DigitalInputState action);

			DigitalOutputMonitor outputMonitor;
			unsigned long int timePressedMillis;
			const IDigitalIO* io;
			const DigitalInputState actionType;
			RunnableCallback callback;
			bool periodic;
			ScheduledTask handle;

			void OnActivating(IDigitalIO* io, unsigned int count) override;
			void OnInactivating(IDigitalIO* io, unsigned int count) override;

			void Run() override;
			void Run2();

			void RunMe();
		};
	}
}