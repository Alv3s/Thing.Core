#include "IOManager.h"
#include "Utils.h"
#include "InputHelpers.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		IOManager::IOManager()
		{

		}

		IOManager::~IOManager()
		{
		}


#pragma region Inputs Management
#pragma region Analogic
		void IOManager::AddListener(IAnalogInputListener & listener)
		{
			AddListener(&listener);
		}

		void IOManager::RemoveListener(IAnalogInputListener & listener)
		{
			RemoveListener(&listener);
		}

		void IOManager::AddListener(IAnalogInputListener * listener)
		{
			if (std::find(analogInputListeners.begin(), analogInputListeners.end(), listener) == analogInputListeners.end())
				analogInputListeners.push_back(listener);
		}

		void IOManager::RemoveListener(IAnalogInputListener * listener)
		{
			auto l = std::find(analogInputListeners.begin(), analogInputListeners.end(), listener);
			if (l != analogInputListeners.end())
				l->shouldDelete = true;
		}

		void IOManager::AddAnalogInput(IAnalogInput * input)
		{
			if (!IOManagerAnalogInput::Contains(analogInputs, input))
			{
				IOManagerAnalogInput managerAnalog;
				managerAnalog.AnalogInput = input;
				managerAnalog.LastReadValue = input->AnalogRead();
				analogInputs.push_back(managerAnalog);
			}
		}

		void IOManager::AddAnalogInput(IAnalogInput & input)
		{
			AddAnalogInput(&input);
		}

		void IOManager::RemoveAnalogInput(IAnalogInput * input)
		{
			for (auto item = analogInputs.begin(); item != analogInputs.end(); ++item)
			{
				if (item->AnalogInput == input)
				{
					analogInputs.erase(item);
					return;
				}
			}
		}

		void IOManager::RemoveAnalogInput(IAnalogInput & input)
		{
			RemoveAnalogInput(&input);
		}
#pragma endregion

#pragma region Digital
		void IOManager::AddListener(IDigitalInputListener & listener)
		{
			AddListener(&listener);
		}

		void IOManager::RemoveListener(IDigitalInputListener & listener)
		{
			RemoveListener(&listener);
		}

		void IOManager::AddListener(IDigitalInputListener * listener)
		{
			if (std::find(digitalInputListeners.begin(), digitalInputListeners.end(), listener) == digitalInputListeners.end())
				digitalInputListeners.push_back(listener);
		}

		void IOManager::RemoveListener(IDigitalInputListener * listener)
		{
			auto l = std::find(digitalInputListeners.begin(), digitalInputListeners.end(), listener);
			if (l != digitalInputListeners.end())
				l->shouldDelete = true;
		}

		void IOManager::AddDigitalInput(IDigitalInput & input)
		{
			AddDigitalInput(&input);
		}

		void IOManager::RemoveDigitalInput(IDigitalInput & input)
		{
			RemoveDigitalInput(&input);
		}

		void IOManager::AddDigitalInput(IDigitalInput * input)
		{
			if (!IOManagerDigitalInput::Contains(digitalInputs, input))
			{
				IOManagerDigitalInput managerInput;
				managerInput.DigitalIO = input;
				managerInput.LastReadValue = input->DigitalRead();
				digitalInputs.push_back(managerInput);
			}
		}

		void IOManager::RemoveDigitalInput(IDigitalInput * input)
		{
			for (auto item = digitalInputs.begin(); item != digitalInputs.end(); ++item)
			{
				if (item->DigitalIO == input)
				{
					digitalInputs.erase(item);
					return;
				}
			}
		}
#pragma endregion
#pragma endregion

#pragma region Outputs Management
		void IOManager::AddListener(IDigitalOutputListener & listener)
		{
			AddListener(&listener);
		}

		void IOManager::RemoveListener(IDigitalOutputListener & listener)
		{
			RemoveListener(&listener);
		}

		void IOManager::AddListener(IDigitalOutputListener * listener)
		{
			if (std::find(digitalOutputListeners.begin(), digitalOutputListeners.end(), listener) == digitalOutputListeners.end())
				digitalOutputListeners.push_back(listener);
		}

		void IOManager::RemoveListener(IDigitalOutputListener * listener)
		{
			auto l = std::find(digitalOutputListeners.begin(), digitalOutputListeners.end(), listener);
			if (l != digitalOutputListeners.end())
				l->shouldDelete = true;
		}



		void IOManager::AddDigitalOutput(IDigitalOutput & output)
		{
			AddDigitalOutput(&output);
		}

		void IOManager::RemoveDigitalOutput(IDigitalOutput & output)
		{
			RemoveDigitalOutput(&output);
		}

		void IOManager::AddDigitalOutput(IDigitalOutput * output)
		{
			if (!IOManagerDigitalOutput::Contains(digitalOutputs, output))
			{
				IOManagerDigitalOutput managerOutput;
				managerOutput.DigitalIO = output;
				digitalOutputs.push_back(managerOutput);
			}
		}

		void IOManager::RemoveDigitalOutput(IDigitalOutput * output)
		{
			for (auto item = digitalOutputs.begin(); item != digitalOutputs.end(); ++item)
			{
				if (item->DigitalIO == output)
				{
					digitalOutputs.erase(item);
					return;
				}
			}
		}
#pragma endregion


		void IOManager::DigitalWrite(IDigitalOutput * output, DigitalValue writeState)
		{
			AddDigitalOutput(output);
			auto entry = IOManagerDigitalOutput::Find(digitalOutputs, output);

			DigitalValue previousValue = output->GetState();
			output->DigitalWrite(writeState);
			DigitalValue currentValue = output->GetState();

			DigitalInputState state = DigitalReadToInputState(previousValue, currentValue);

			switch (state)
			{
			case DigitalInputState::WasActivated:
			{
				auto triggerCount = entry->IncrementWasActivatedCount();
				for (auto it = digitalOutputListeners.begin(); it != digitalOutputListeners.end();)
					if (it->shouldDelete)
						it = digitalOutputListeners.erase(it);
					else
					{
						it->listener.OnActivating(output, triggerCount);
						++it;
					}
				break;
			}
			case DigitalInputState::WasInactivated:
			{
				auto triggerCount = entry->IncrementWasInactivatedCount();
				for (auto it = digitalOutputListeners.begin(); it != digitalOutputListeners.end();)
					if (it->shouldDelete)
						it = digitalOutputListeners.erase(it);
					else
					{
						it->listener.OnInactivating(output, triggerCount);
						++it;
					}
				break;
			}
			}
		}

		void IOManager::DigitalWrite(IDigitalOutput & output, DigitalValue writeState)
		{
			DigitalWrite(&output, writeState);
		}

		void IOManager::DigitalWrite(IDigitalOutput * output, DigitalValue state, unsigned long millis)
		{
			DigitalWrite(output, state);
			DigitalValue newState = state == DigitalValue::High ? DigitalValue::Low : state == DigitalValue::Low ? DigitalValue::High : DigitalValue::Toggle;

			new IOManager::IOManagerRevertDigitalWrite(*this, *output, newState, millis);
		}

		void IOManager::DigitalWrite(IDigitalOutput & output, DigitalValue state, unsigned long millis)
		{
			DigitalWrite(&output, state, millis);
		}

#pragma region Bindings and Configurations
		IDigitalIOMonitor& IOManager::OnActivating(IDigitalInput & input)
		{
			return On(input, DigitalInputState::WasActivated);
		}

		IDigitalIOMonitor& IOManager::OnActivating(IDigitalInput * input)
		{
			return OnActivating(*input);
		}

		IDigitalIOMonitor& IOManager::OnInactivating(IDigitalInput & input)
		{
			return On(input, DigitalInputState::WasInactivated);
		}

		IDigitalIOMonitor& IOManager::OnInactivating(IDigitalInput * input)
		{
			return OnInactivating(*input);
		}


		IDigitalIOMonitor& IOManager::OnActivating(IDigitalOutput & output)
		{
			return On(output, DigitalInputState::WasActivated);
		}

		IDigitalIOMonitor& IOManager::OnActivating(IDigitalOutput * output)
		{
			return OnActivating(*output);
		}

		IDigitalIOMonitor& IOManager::OnInactivating(IDigitalOutput & output)
		{
			return On(output, DigitalInputState::WasInactivated);
		}

		IDigitalIOMonitor& IOManager::OnInactivating(IDigitalOutput * output)
		{
			return OnInactivating(*output);
		}

		DigitalIOMonitor& IOManager::On(IDigitalInput & input, DigitalInputState state)
		{
			AddDigitalInput(input);

			auto it = monitors.insert(monitors.end(), DigitalIOMonitor(*this, &input, state));
			DigitalIOMonitor* monitor = &*it;
			monitor->RunMe();
			AddListener((IDigitalInputListener*)monitor);
			return *monitor;
		}

		DigitalIOMonitor& IOManager::On(IDigitalOutput & output, DigitalInputState state)
		{
			AddDigitalOutput(output);

			auto it = monitors.insert(monitors.end(), DigitalIOMonitor(*this, &output, state));
			DigitalIOMonitor* monitor = &*it;
			monitor->RunMe();
			AddListener((IDigitalOutputListener*)monitor);
			return *monitor;
		}

		ITimedDigitalIOMonitor& IOManager::OnActive(IDigitalInput * input)
		{
			return OnActive(*input);
		}

		ITimedDigitalIOMonitor& IOManager::OnActive(IDigitalInput & input)
		{
			return On(input, DigitalInputState::WasActivated);
		}

		ITimedDigitalIOMonitor& IOManager::OnInactive(IDigitalInput * input)
		{
			return OnInactive(*input);
		}

		ITimedDigitalIOMonitor& IOManager::OnInactive(IDigitalInput & input)
		{
			return On(input, DigitalInputState::WasInactivated);
		}

		ITimedDigitalIOMonitor& IOManager::OnActive(IDigitalOutput * output)
		{
			return OnActive(*output);
		}

		ITimedDigitalIOMonitor& IOManager::OnActive(IDigitalOutput & output)
		{
			return On(output, DigitalInputState::WasActivated);
		}

		ITimedDigitalIOMonitor& IOManager::OnInactive(IDigitalOutput * output)
		{
			return OnInactive(*output);
		}

		ITimedDigitalIOMonitor& IOManager::OnInactive(IDigitalOutput & output)
		{
			return On(output, DigitalInputState::WasInactivated);
		}
#pragma endregion

		void IOManager::ProcessAnalog()
		{
			if (analogInputListeners.size() == 0)
				return;

			for (auto& entry : analogInputs)
			{
				IAnalogInput* input = entry.AnalogInput;
				int previousAnalogValue = entry.LastReadValue;

				int currentAnalogValue = input->AnalogRead();
				int precision = input->GetPrecision();

				if (currentAnalogValue == previousAnalogValue)
					continue;

				if (precision == 0U)
					continue;

				if (currentAnalogValue - precision >= previousAnalogValue)
				{
					for (auto it = analogInputListeners.begin(); it != analogInputListeners.end();)
						if (it->shouldDelete)
							it = analogInputListeners.erase(it);
						else
						{
							it->listener.OnIncreasingValue(input, currentAnalogValue);
							++it;
						}
					entry.LastReadValue = currentAnalogValue;
				}
				else if (currentAnalogValue + precision <= previousAnalogValue)
				{
					for (auto it = analogInputListeners.begin(); it != analogInputListeners.end();)
						if (it->shouldDelete)
							it = analogInputListeners.erase(it);
						else
						{
							it->listener.OnDecreasingValue(input, currentAnalogValue);
							++it;
						}
					entry.LastReadValue = currentAnalogValue;
				}
			}
		}

		void IOManager::ProcessDigital()
		{
			if (digitalInputListeners.size() == 0)
				return;

			for (auto& entry : digitalInputs)
			{
				IDigitalInput* input = entry.DigitalIO;
				DigitalValue previousState = entry.LastReadValue;
				DigitalValue state = input->DigitalRead();
				DigitalInputState result = DigitalReadToInputState(previousState, state);
				entry.LastReadValue = state;

				switch (result)
				{
				case DigitalInputState::WasActivated:
				{
					auto triggerCount = entry.IncrementWasActivatedCount();
					for (auto it = digitalInputListeners.begin(); it != digitalInputListeners.end();)
						if (it->shouldDelete)
							it = digitalInputListeners.erase(it);
						else
						{
							it->listener.OnActivating(input, triggerCount);
							++it;
						}
					break;
				}
				case DigitalInputState::WasInactivated:
				{
					auto triggerCount = entry.IncrementWasInactivatedCount();
					for (auto it = digitalInputListeners.begin(); it != digitalInputListeners.end();)
						if (it->shouldDelete)
							it = digitalInputListeners.erase(it);
						else
						{
							it->listener.OnInactivating(input, triggerCount);
							++it;
						}
					break;
				}
				}
			}
		}

		void IOManager::Process()
		{
			ProcessDigital();
			ProcessAnalog();
		}

#pragma region IOManagerRevertDigitalWrite
		IOManager::IOManagerRevertDigitalWrite::IOManagerRevertDigitalWrite(IOManager & manager, IDigitalOutput & output, DigitalValue toState, unsigned long millis) :
			manager(&manager),
			output(&output),
			toState(toState)
		{
			TaskScheduler->AttachOnce(millis, this);
			this->manager->AddListener(this);
		}

		IOManager::IOManagerRevertDigitalWrite::~IOManagerRevertDigitalWrite()
		{
			TaskScheduler->Detach(this);
			manager->RemoveListener(this);
		}

		void IOManager::IOManagerRevertDigitalWrite::Run()
		{
			manager->DigitalWrite(output, toState);
			delete this;
		}

		void IOManager::IOManagerRevertDigitalWrite::OnActivating(IDigitalIO* output, unsigned int count)
		{
			if (this->output != output)
				return;

			delete this;
		}

		void IOManager::IOManagerRevertDigitalWrite::OnInactivating(IDigitalIO* output, unsigned int count)
		{
			if (this->output != output)
				return;

			delete this;
		}
#pragma endregion
	}
}