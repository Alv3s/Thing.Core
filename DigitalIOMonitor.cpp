#include "DigitalIOMonitor.h"
#include "IOManager.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		DigitalIOMonitor::DigitalIOMonitor(IOManager& manager, IDigitalInput* input, DigitalInputState action) :
			outputMonitor(manager),
			io(input),
			actionType(action),
			timePressedMillis(0),
			task(this)
		{
			manager.AddDigitalInput(input);
		}

		DigitalIOMonitor::DigitalIOMonitor(IOManager& manager, IDigitalOutput* output, DigitalInputState action) : 
			outputMonitor(manager), 
			io(output), 
			actionType(action),
			timePressedMillis(0),
			task(this)
		{
			manager.AddDigitalOutput(output);
		}

		DigitalIOMonitor::~DigitalIOMonitor()
		{
			TaskScheduler->Detach(task);
		}


#pragma region Digital Input Listener Methods
		void DigitalIOMonitor::OnActivating(IDigitalIO* io, unsigned int count)
		{
			if (this->io != io)
				return;

			if (timePressedMillis)
			{
				if (actionType == DigitalInputState::WasActivated)
				{
					if (task == this)
						TaskScheduler->AttachOnce(timePressedMillis, task);
					else
						TaskScheduler->AttachPeriodic(timePressedMillis, task);
				}
				else
					TaskScheduler->Detach(task);
				return;
			}

			if (actionType == DigitalInputState::WasActivated)
				outputMonitor.Action();
		}

		void DigitalIOMonitor::OnInactivating(IDigitalIO* io, unsigned int count)
		{
			if (this->io != io)
				return;

			if (timePressedMillis)
			{
				if (actionType == DigitalInputState::WasInactivated)
				{
					if (task == this)
						TaskScheduler->AttachOnce(timePressedMillis, task);
					else
						TaskScheduler->AttachPeriodic(timePressedMillis, task);
				}
				else
					TaskScheduler->Detach(task);
				return;
			}

			if (actionType == DigitalInputState::WasInactivated)
				outputMonitor.Action();
		}
#pragma endregion

		void DigitalIOMonitor::Toggle(IDigitalOutput& output)
		{
			Toggle(&output);
		}

		void DigitalIOMonitor::Toggle(IDigitalOutput* output)
		{
			outputMonitor.SetOutput(output, DigitalValue::Toggle);
		}

		IDigitalOutputMonitor& DigitalIOMonitor::SetHigh(IDigitalOutput& output)
		{
			return SetHigh(&output);
		}

		IDigitalOutputMonitor& DigitalIOMonitor::SetHigh(IDigitalOutput* output)
		{
			outputMonitor.SetOutput(output, DigitalValue::High);
			return outputMonitor;
		}

		IDigitalOutputMonitor& DigitalIOMonitor::SetLow(IDigitalOutput& output)
		{
			return SetLow(&output);
		}

		IDigitalOutputMonitor& DigitalIOMonitor::SetLow(IDigitalOutput* output)
		{
			outputMonitor.SetOutput(output, DigitalValue::Low);
			return outputMonitor;
		}

		IDigitalIOMonitor& DigitalIOMonitor::For(int millis)
		{
			timePressedMillis = millis;
			return *this;
		}

		IActionableIOMonitor& DigitalIOMonitor::Each(int millis)
		{
			timePressedMillis = millis;
			return *this;
		}

		void DigitalIOMonitor::Perform(IRunnable& runnable)
		{
			Perform(&runnable);
		}

		void DigitalIOMonitor::Perform(IRunnable* runnable)
		{
			task = runnable;
		}

		void DigitalIOMonitor::Run()
		{
			outputMonitor.Action();
		}
	}
}