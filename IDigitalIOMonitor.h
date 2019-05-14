#pragma once

#include "IDigitalOutput.h"
#include "DigitalOutputMonitor.h"
#include "IDigitalOutputMonitor.h"

namespace Thing
{
	namespace Core
	{
		class IDigitalIOMonitor
		{
		public:
			virtual void Toggle(IDigitalOutput& output) = 0;
			virtual void Toggle(IDigitalOutput* output) = 0;

			virtual IDigitalOutputMonitor& SetHigh(IDigitalOutput& output) = 0;
			virtual IDigitalOutputMonitor& SetHigh(IDigitalOutput* output) = 0;

			virtual IDigitalOutputMonitor& SetLow(IDigitalOutput& output) = 0;
			virtual IDigitalOutputMonitor& SetLow(IDigitalOutput* output) = 0;
		};

		class IActionableIOMonitor
		{
		public:
			virtual void Perform(IRunnable& runnable) = 0;
			virtual void Perform(IRunnable* runnable) = 0;
		};

		class ITimedDigitalIOMonitor
		{
		public:
			virtual IDigitalIOMonitor& For(int millis) = 0;
			virtual IActionableIOMonitor& Each(int millis) = 0;
		};

	}
}