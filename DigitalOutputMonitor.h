#pragma once

#include "IDigitalIOListeners.h"
#include "IDigitalOutput.h"
#include "ITaskScheduler.h"
#include "IDigitalOutputMonitor.h"

namespace Thing
{
	namespace Core
	{
		class DigitalOutputMonitor : public virtual IDigitalOutputMonitor, private virtual IRunnable
		{
			friend class DigitalIOMonitor;
		public:
			virtual ~DigitalOutputMonitor();
			void For(int millis) override;

		protected:
			DigitalOutputMonitor(class IOManager& manager);
			void SetOutput(IDigitalOutput* output, DigitalValue state);
			virtual void Action();
			void Action(DigitalValue state, unsigned long millis);

			IDigitalOutput* GetOutput() const;
		private:
			virtual void Run() override;

			IOManager& manager;

			IDigitalOutput* output;
			DigitalValue state;
			unsigned long stateDuration_ms;
		};
	}
}