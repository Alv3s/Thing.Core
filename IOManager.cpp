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
			for (auto m : monitors)
				delete m;
		}


#pragma region Inputs Management
#pragma region Analogic
		void IOManager::AddListener(IAnalogInputListener& listener)
		{
			AddListener(&listener);
		}

		void IOManager::RemoveListener(IAnalogInputListener& listener)
		{
			RemoveListener(&listener);
		}

		void IOManager::AddListener(IAnalogInputListener* listener)
		{
			if (!Utils::Contains(analogInputListeners, listener))
				analogInputListeners.push_back(listener);
		}

		void IOManager::RemoveListener(IAnalogInputListener* listener)
		{
			analogInputListeners.remove(listener);
		}

		void IOManager::AddAnalogInput(IAnalogInput * input)
		{
			if (!IOManagerAnalogInput::Contains(analogInputs, input))
			{
				IOManagerAnalogInput managerAnalog;
				managerAnalog.AnalogInput = input;
				managerAnalog.LastReadValue = input->AnalogRead();
				analogInputs.push_back(managerAnalog);

				Logger->Debug("IOManager.AddAnalogInput -> code = %d", input->GetCode());
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
					Logger->Debug("IOManager.RemoveAnalogInput -> code = %d", input->GetCode());
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
		void IOManager::AddListener(IDigitalInputListener& listener)
		{
			AddListener(&listener);
		}

		void IOManager::RemoveListener(IDigitalInputListener& listener)
		{
			RemoveListener(&listener);
		}

		void IOManager::AddListener(IDigitalInputListener* listener)
		{
			if (!Utils::Contains(digitalInputListeners, listener))
				digitalInputListeners.push_back(listener);
		}

		void IOManager::RemoveListener(IDigitalInputListener* listener)
		{
			digitalInputListeners.remove(listener);
		}

		void IOManager::AddDigitalInput(IDigitalInput& input)
		{
			AddDigitalInput(&input);
		}

		void IOManager::RemoveDigitalInput(IDigitalInput& input)
		{
			RemoveDigitalInput(&input);
		}

		void IOManager::AddDigitalInput(IDigitalInput* input)
		{
			if (!IOManagerDigitalInput::Contains(digitalInputs, input))
			{
				IOManagerDigitalInput managerInput;
				managerInput.DigitalInput = input;
				managerInput.LastReadValue = input->DigitalRead();
				digitalInputs.push_back(managerInput);

				Logger->Debug("IOManager.AddDigitalInput -> code = %d", input->GetCode());
			}
		}

		void IOManager::RemoveDigitalInput(IDigitalInput* input)
		{
			for (auto item = digitalInputs.begin(); item != digitalInputs.end(); ++item)
			{
				if (item->DigitalInput == input)
				{
					digitalInputs.erase(item);
					Logger->Debug("IOManager.RemoveDigitalInput -> code = %d", input->GetCode());
					return;
				}
			}
		}
#pragma endregion
#pragma endregion

#pragma region Outputs Management
		void IOManager::AddListener(IDigitalOutputListener& listener)
		{
			AddListener(&listener);
		}

		void IOManager::RemoveListener(IDigitalOutputListener& listener)
		{
			RemoveListener(&listener);
		}

		void IOManager::AddListener(IDigitalOutputListener* listener)
		{
			if (!Utils::Contains(digitalOutputListeners, listener))
				digitalOutputListeners.push_back(listener);
		}

		void IOManager::RemoveListener(IDigitalOutputListener* listener)
		{
			digitalOutputListeners.remove(listener);
		}



		void IOManager::AddDigitalOutput(IDigitalOutput& output)
		{
			AddDigitalOutput(&output);
		}

		void IOManager::RemoveDigitalOutput(IDigitalOutput& output)
		{
			RemoveDigitalOutput(&output);
		}

		void IOManager::AddDigitalOutput(IDigitalOutput* output)
		{
			if (!Utils::Contains(digitalOutputs, output))
			{
				Logger->Debug("IOManager.AddDigitalOutput -> code = %d", output->GetCode());
				digitalOutputs.push_back(output);
			}	
		}

		void IOManager::RemoveDigitalOutput(IDigitalOutput* output)
		{
			Logger->Debug("IOManager.RemoveDigitalOutput -> code = %d", output->GetCode());

			digitalOutputs.remove(output);
		}
#pragma endregion


		void IOManager::DigitalWrite(IDigitalOutput* output, DigitalValue writeState)
		{
			AddDigitalOutput(output);

			DigitalValue previousValue = output->GetState();
			output->DigitalWrite(writeState);
			DigitalValue currentValue = output->GetState();

			DigitalInputState state = DigitalReadToInputState(previousValue, currentValue);
			const int code = output->GetCode();

			switch (state)
			{
			case DigitalInputState::WasActivated:
				for (IDigitalOutputListener* l : digitalOutputListeners)
					l->OnActivating(code);
				break;
			case DigitalInputState::WasInactivated:
				for (IDigitalOutputListener* l : digitalOutputListeners)
					l->OnInactivating(code);
				break;
			}
		}

		void IOManager::DigitalWrite(IDigitalOutput& output, DigitalValue state)
		{
			DigitalWrite(&output, state);
		}

		IDigitalIOMonitor& IOManager::OnActivating(IDigitalInput& input)
		{
			return On(input, DigitalInputState::WasActivated);
		}

		IDigitalIOMonitor& IOManager::OnActivating(IDigitalInput* input)
		{
			return OnActivating(*input);
		}

		IDigitalIOMonitor& IOManager::OnInactivating(IDigitalInput& input)
		{
			return On(input, DigitalInputState::WasInactivated);
		}

		IDigitalIOMonitor& IOManager::OnInactivating(IDigitalInput* input)
		{
			return OnInactivating(*input);
		}


		IDigitalIOMonitor& IOManager::OnActivating(IDigitalOutput& output)
		{
			return On(output, DigitalInputState::WasActivated);
		}

		IDigitalIOMonitor& IOManager::OnActivating(IDigitalOutput* output)
		{
			return OnActivating(*output);
		}

		IDigitalIOMonitor& IOManager::OnInactivating(IDigitalOutput& output)
		{
			return On(output, DigitalInputState::WasInactivated);
		}

		IDigitalIOMonitor& IOManager::OnInactivating(IDigitalOutput* output)
		{
			return OnInactivating(*output);
		}

		DigitalIOMonitor& IOManager::On(IDigitalInput& input, DigitalInputState state)
		{
			AddDigitalInput(input);

			DigitalIOMonitor* monitor = new DigitalIOMonitor(*this, &input, state);
			monitors.push_back(monitor);
			AddListener((IDigitalInputListener*)monitor);
			return *monitor;
		}

		DigitalIOMonitor& IOManager::On(IDigitalOutput& output, DigitalInputState state)
		{
			AddDigitalOutput(output);

			DigitalIOMonitor* monitor = new DigitalIOMonitor(*this, &output, state);
			monitors.push_back(monitor);
			AddListener((IDigitalOutputListener*)monitor);
			return *monitor;
		}

		ITimedDigitalIOMonitor& IOManager::OnActive(IDigitalInput* input)
		{
			return OnActive(*input);
		}

		ITimedDigitalIOMonitor& IOManager::OnActive(IDigitalInput& input)
		{
			return On(input, DigitalInputState::WasActivated);
		}

		ITimedDigitalIOMonitor& IOManager::OnInactive(IDigitalInput* input)
		{
			return OnInactive(*input);
		}

		ITimedDigitalIOMonitor& IOManager::OnInactive(IDigitalInput& input)
		{
			return On(input, DigitalInputState::WasInactivated);
		}

		ITimedDigitalIOMonitor& IOManager::OnActive(IDigitalOutput* output)
		{
			return OnActive(*output);
		}

		ITimedDigitalIOMonitor& IOManager::OnActive(IDigitalOutput& output)
		{
			return On(output, DigitalInputState::WasActivated);
		}

		ITimedDigitalIOMonitor& IOManager::OnInactive(IDigitalOutput* output)
		{
			return OnInactive(*output);
		}
		
		ITimedDigitalIOMonitor& IOManager::OnInactive(IDigitalOutput& output)
		{
			return On(output, DigitalInputState::WasInactivated);
		}


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
				const int code = input->GetCode();

				if (currentAnalogValue == previousAnalogValue)
					continue;

				if (precision == 0U)
				{
					Logger->Warn("The AnalogInput was ignored because precision = 0 is invalid -> code = %d", input->GetCode());
					continue;
				}

				if (currentAnalogValue - precision >= previousAnalogValue)
				{
					for (IAnalogInputListener* l : analogInputListeners)
						l->OnIncreasingValue(code, currentAnalogValue);
					entry.LastReadValue = currentAnalogValue;
				}
				else if (currentAnalogValue + precision <= previousAnalogValue)
				{
					for (IAnalogInputListener* l : analogInputListeners)
						l->OnDecreasingValue(code, currentAnalogValue);
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
				IDigitalInput* input = entry.DigitalInput;
				DigitalValue previousState = entry.LastReadValue;
				DigitalValue state = input->DigitalRead();
				DigitalInputState result = DigitalReadToInputState(previousState, state);
				entry.LastReadValue = state;

				const int code = input->GetCode();
				switch (result)
				{
				case DigitalInputState::WasActivated:
				{
					auto triggerCount = entry.IncrementWasActivatedCount();
					for (IDigitalInputListener* l : digitalInputListeners)
						l->OnActivating(code, triggerCount);
					break;
				}
				case DigitalInputState::WasInactivated:
				{
					auto triggerCount = entry.IncrementWasInactivatedCount();
					for (IDigitalInputListener* l : digitalInputListeners)
						l->OnInactivating(code, triggerCount);
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
	}
}