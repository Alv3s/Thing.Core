#include "DigitalIOMonitor.h"
#include "IOManager.h"

namespace Thing
{
	namespace Core
	{
		DigitalIOMonitor::DigitalIOMonitor(IOManager& manager, IDigitalInput* input, DigitalInputState action) : outputMonitor(manager), io(input), actionType(action)
		{
			manager.AddDigitalInput(input);
		}

		DigitalIOMonitor::DigitalIOMonitor(IOManager& manager, IDigitalOutput* output, DigitalInputState action) : outputMonitor(manager), io(output), actionType(action)
		{
			manager.AddDigitalOutput(output);
		}

		DigitalIOMonitor::~DigitalIOMonitor()
		{
		}


#pragma region Digital Input Listener Methods
		void DigitalIOMonitor::OnActivating(int code)
		{
			if (io->GetCode() != code)
				return;

			if (actionType == DigitalInputState::WasActivated)
				outputMonitor.Action();
		}

		void DigitalIOMonitor::OnActivating(int code, unsigned int count)
		{
			this->OnActivating(code);
		}

		void DigitalIOMonitor::OnInactivating(int code)
		{
			if (io->GetCode() != code)
				return;

			if (actionType == DigitalInputState::WasInactivated)
				outputMonitor.Action();
		}
		void DigitalIOMonitor::OnInactivating(int code, unsigned int count)
		{
			this->OnInactivating(code);
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
	}
}