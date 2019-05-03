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
				managerInput.DigitalInput = input;
				managerInput.LastReadValue = input->DigitalRead();
				digitalInputs.push_back(managerInput);

				Logger->Debug("IOManager.AddDigitalInput -> code = %d", input->GetCode());
			}
		}

		void IOManager::RemoveDigitalInput(IDigitalInput * input)
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
			if (!Utils::Contains(digitalOutputs, output))
			{
				Logger->Debug("IOManager.AddDigitalOutput -> code = %d", output->GetCode());
				digitalOutputs.push_back(output);
			}
		}

		void IOManager::RemoveDigitalOutput(IDigitalOutput * output)
		{
			Logger->Debug("IOManager.RemoveDigitalOutput -> code = %d", output->GetCode());

			digitalOutputs.remove(output);
		}
#pragma endregion


		void IOManager::DigitalWrite(IDigitalOutput * output, DigitalValue writeState)
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
				for (auto it = digitalOutputListeners.begin(); it != digitalOutputListeners.end();)
					if (it->shouldDelete)
						it = digitalOutputListeners.erase(it);
					else
					{
						it->listener.OnActivating(code);
						++it;
					}
				break;
			case DigitalInputState::WasInactivated:
				for (auto it = digitalOutputListeners.begin(); it != digitalOutputListeners.end();)
					if (it->shouldDelete)
						it = digitalOutputListeners.erase(it);
					else
					{
						it->listener.OnInactivating(code);
						++it;
					}
				break;
			}
		}

		void IOManager::DigitalWrite(IDigitalOutput & output, DigitalValue state)
		{
			DigitalWrite(&output, state);
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

			DigitalIOMonitor* monitor = new DigitalIOMonitor(*this, &input, state);
			monitors.push_back(monitor);
			AddListener((IDigitalInputListener*)monitor);
			return *monitor;
		}

		DigitalIOMonitor& IOManager::On(IDigitalOutput & output, DigitalInputState state)
		{
			AddDigitalOutput(output);

			DigitalIOMonitor* monitor = new DigitalIOMonitor(*this, &output, state);
			monitors.push_back(monitor);
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
					for (auto it = analogInputListeners.begin(); it != analogInputListeners.end();)
						if (it->shouldDelete)
							it = analogInputListeners.erase(it);
						else
						{
							it->listener.OnIncreasingValue(code, currentAnalogValue);
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
							it->listener.OnDecreasingValue(code, currentAnalogValue);
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
					for (auto it = digitalInputListeners.begin(); it != digitalInputListeners.end();)
						if (it->shouldDelete)
							it = digitalInputListeners.erase(it);
						else
						{
							it->listener.OnActivating(code, triggerCount);
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
							it->listener.OnInactivating(code, triggerCount);
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

		void IOManager::IOManagerRevertDigitalWrite::OnActivating(int code)
		{
			if (output->GetCode() != code)
				return;

			delete this;
		}

		void IOManager::IOManagerRevertDigitalWrite::OnActivating(int code, unsigned int count)
		{
			if (output->GetCode() != code)
				return;

			delete this;
		}

		void IOManager::IOManagerRevertDigitalWrite::OnInactivating(int code)
		{
			if (output->GetCode() != code)
				return;

			delete this;
		}

		void IOManager::IOManagerRevertDigitalWrite::OnInactivating(int code, unsigned int count)
		{
			if (output->GetCode() != code)
				return;

			delete this;
		}
#pragma endregion
	}
}