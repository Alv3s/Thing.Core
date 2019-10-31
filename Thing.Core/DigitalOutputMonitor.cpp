#include "DigitalOutputMonitor.h"
#include "IOManager.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		DigitalOutputMonitor::DigitalOutputMonitor(IOManager& manager) : 
			manager(manager), 
			output(nullptr), 
			stateDuration_ms(0),
			handle(NULL)
		{
		}


		DigitalOutputMonitor::~DigitalOutputMonitor()
		{
			TaskScheduler->Detach(handle);
		}

		void DigitalOutputMonitor::SetOutput(IDigitalOutput * output, DigitalValue state)
		{
			if (this->output) //Guarantee we don't change output nor state when already configured
				return;

			this->output = output;
			this->state = state;

			manager.AddDigitalOutput(output);
		}

		void DigitalOutputMonitor::Action()
		{
			Action(state, stateDuration_ms);
		}

		void DigitalOutputMonitor::Action(DigitalValue state, unsigned long millis)
		{
			if (!output)
				return;

			manager.DigitalWrite(output, state);
			if (stateDuration_ms > 0)
				handle = TaskScheduler->AttachOnce(millis, this);
		}

		void DigitalOutputMonitor::For(int millis)
		{
			if (state == DigitalValue::Toggle)
				return;

			stateDuration_ms = millis;
		}

		void DigitalOutputMonitor::Run()
		{
			switch (state)
			{
			case DigitalValue::Low: manager.DigitalWrite(output, DigitalValue::High); break;
			case DigitalValue::High: manager.DigitalWrite(output, DigitalValue::Low); break;
			case DigitalValue::Toggle: manager.DigitalWrite(output, state); break;
			}
		}

		IDigitalOutput* DigitalOutputMonitor::GetOutput() const
		{
			return output;
		}
	}
}