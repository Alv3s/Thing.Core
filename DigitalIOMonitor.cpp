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
			timePressedMillis(0)
		{
			manager.AddDigitalInput(input);
		}

		DigitalIOMonitor::DigitalIOMonitor(IOManager& manager, IDigitalOutput* output, DigitalInputState action) : 
			outputMonitor(manager), 
			io(output), 
			actionType(action),
			timePressedMillis(0)
		{
			manager.AddDigitalOutput(output);
		}

		DigitalIOMonitor::~DigitalIOMonitor()
		{
		}


#pragma region Digital Input Listener Methods
		void DigitalIOMonitor::OnActivating(int code, unsigned int count)
		{
			if (io->GetCode() != code)
				return;

			if (timePressedMillis)
			{
				if (actionType == DigitalInputState::WasActivated)
					TaskScheduler->AttachOnce(timePressedMillis, this);
				else
					TaskScheduler->Detach(this);
				return;
			}

			if (actionType == DigitalInputState::WasActivated)
				outputMonitor.Action();
		}

		void DigitalIOMonitor::OnInactivating(int code, unsigned int count)
		{
			if (io->GetCode() != code)
				return;

			if (timePressedMillis)
			{
				if (actionType == DigitalInputState::WasInactivated)
					TaskScheduler->AttachOnce(timePressedMillis, this);
				else
					TaskScheduler->Detach(this);
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

		void DigitalIOMonitor::Run()
		{
			outputMonitor.Action();
		}
	}
}