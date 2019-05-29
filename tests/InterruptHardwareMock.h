#pragma once

#include "TimedHardwareMock.h"
#include <map>
#include "../Thing.Core/IDigitalInput.h"
#include "../Thing.Core/InputHelpers.h"

namespace Thing {
	namespace Core {
		namespace Tests {
			class InterruptHardwareMock : public TimedHardwareMock, public virtual IMillisListener
			{
			private:
				struct Interrupt
				{
					Thing::Core::IRunnable* interruptListener;
					Thing::Core::InterruptMode mode;
				};
			public:
				InterruptHardwareMock() : TimedHardwareMock()
				{
					this->AddListener(this);
				}

				virtual void OnMillisChange(unsigned long millis) override
				{
					for (auto& e : simulators)
					{
						int gpio = e.first;
						Thing::Core::IDigitalInput* simulator = e.second;

						Thing::Core::DigitalValue previousState = states[simulator];
						Thing::Core::DigitalValue state = simulator->DigitalRead();

						Thing::Core::DigitalInputState result = Thing::Core::DigitalReadToInputState(previousState, state);
						Interrupt parameter = interrups[gpio];

						switch (result)
						{
						case Thing::Core::DigitalInputState::WasActivated:
							if (parameter.mode == Thing::Core::InterruptMode::OnChange || parameter.mode == Thing::Core::InterruptMode::WasActivated)
								parameter.interruptListener->Run();
							break;
						case Thing::Core::DigitalInputState::WasInactivated:
							if (parameter.mode == Thing::Core::InterruptMode::OnChange || parameter.mode == Thing::Core::InterruptMode::WasInactivated)
								parameter.interruptListener->Run();
							break;
						}
					}
				}

				void AddGpioSimulator(int gpio, Thing::Core::IDigitalInput& input)
				{
					simulators.erase(gpio);
					simulators[gpio] = &input;
					states[&input] = input.DigitalRead();
				}

				virtual void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable* runnable) override
				{
					DetachInterrupt(gpio);

					Interrupt parameter;
					parameter.interruptListener = runnable;
					parameter.mode = mode;
					interrups[gpio] = parameter;
				}

				virtual void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable& runnable) override
				{
					AttachInterrupt(gpio, mode, &runnable);
				}

				virtual void DetachInterrupt(int gpio) override
				{
					interrups.erase(gpio);
				}

			protected:
				std::map<int, Thing::Core::IDigitalInput*> simulators;
				std::map<Thing::Core::IDigitalInput*, DigitalValue> states;
				std::map<int, Interrupt> interrups;
			};
		}
	}
}