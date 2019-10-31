#pragma once

#include "IDigitalOutput.h"
#include "DigitalOutputMonitor.h"
#include "IDigitalOutputMonitor.h"

namespace Thing
{
	namespace Core
	{
		class IActionableIOMonitor
		{
		public:
			virtual void Run(IRunnable& runnable) = 0;
			virtual void Run(IRunnable* runnable) = 0;
			virtual void Run(RunnableCallback f) = 0;
		};

		class IDigitalIOMonitor : public virtual IActionableIOMonitor
		{
		public:
			virtual void Toggle(IDigitalOutput& output) = 0;
			virtual void Toggle(IDigitalOutput* output) = 0;

			virtual IDigitalOutputMonitor& SetHigh(IDigitalOutput& output) = 0;
			virtual IDigitalOutputMonitor& SetHigh(IDigitalOutput* output) = 0;

			virtual IDigitalOutputMonitor& SetLow(IDigitalOutput& output) = 0;
			virtual IDigitalOutputMonitor& SetLow(IDigitalOutput* output) = 0;
		};

		class ITimedDigitalIOMonitor
		{
		public:
			virtual IDigitalIOMonitor& For(int millis) = 0;
			virtual IDigitalIOMonitor& Each(int millis) = 0;
		};

	}
}