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
			task(this),
			periodic(false)
		{
			manager.AddDigitalInput(input);
		}

		DigitalIOMonitor::DigitalIOMonitor(IOManager& manager, IDigitalOutput* output, DigitalInputState action) : 
			outputMonitor(manager), 
			io(output), 
			actionType(action),
			timePressedMillis(0),
			task(this),
			periodic(false)
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
					if (periodic)
						TaskScheduler->AttachPeriodic(timePressedMillis, task);
					else
						TaskScheduler->AttachOnce(timePressedMillis, task);
				}
				else
					TaskScheduler->Detach(task);
				return;
			}

			if (actionType == DigitalInputState::WasActivated)
				task->Run();
		}

		void DigitalIOMonitor::OnInactivating(IDigitalIO* io, unsigned int count)
		{
			if (this->io != io)
				return;

			if (timePressedMillis)
			{
				if (actionType == DigitalInputState::WasInactivated)
				{
					if (periodic)
						TaskScheduler->AttachPeriodic(timePressedMillis, task);
					else
						TaskScheduler->AttachOnce(timePressedMillis, task);
				}
				else
					TaskScheduler->Detach(task);
				return;
			}

			if (actionType == DigitalInputState::WasInactivated)
				task->Run();
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

		IDigitalIOMonitor& DigitalIOMonitor::Each(int millis)
		{
			timePressedMillis = millis;
			periodic = true;
			return *this;
		}

		void DigitalIOMonitor::Run(IRunnable& runnable)
		{
			Run(&runnable);
		}

		void DigitalIOMonitor::Run(IRunnable* runnable)
		{
			task = runnable;
		}

		void DigitalIOMonitor::Run()
		{
			outputMonitor.Action();
		}
	}
}