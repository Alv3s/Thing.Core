#pragma once

#include <list>
#include <map>
#include "IAnalogInput.h"
#include "IDigitalInput.h"
#include "IAnalogIOListener.h"
#include "IDigitalIOListeners.h"
#include "DigitalIOMonitor.h"
#include "IOManagerDigitalInput.h"
#include "IOManagerDigitalOutput.h"
#include "IOManagerAnalogInput.h"

namespace Thing
{
	namespace Core
	{
		class IOManager
		{
		public:
			IOManager();
			virtual ~IOManager();

#pragma region Input Management
			void AddListener(IAnalogInputListener& listener);
			void RemoveListener(IAnalogInputListener& listener);

			void AddListener(IAnalogInputListener* listener);
			void RemoveListener(IAnalogInputListener* listener);

			void AddListener(IDigitalInputListener& listener);
			void RemoveListener(IDigitalInputListener& listener);

			void AddListener(IDigitalInputListener* listener);
			void RemoveListener(IDigitalInputListener* listener);

			void AddAnalogInput(IAnalogInput& input);
			void RemoveAnalogInput(IAnalogInput& input);

			void AddAnalogInput(IAnalogInput* input);
			void RemoveAnalogInput(IAnalogInput* input);

			void AddDigitalInput(IDigitalInput& input);
			void RemoveDigitalInput(IDigitalInput& input);

			void AddDigitalInput(IDigitalInput* input);
			void RemoveDigitalInput(IDigitalInput* input);
#pragma endregion

#pragma region Output Management
			void AddListener(IDigitalOutputListener& listener);
			void RemoveListener(IDigitalOutputListener& listener);

			void AddListener(IDigitalOutputListener* listener);
			void RemoveListener(IDigitalOutputListener* listener);

			void AddDigitalOutput(IDigitalOutput& output);
			void RemoveDigitalOutput(IDigitalOutput& output);

			void AddDigitalOutput(IDigitalOutput* output);
			void RemoveDigitalOutput(IDigitalOutput* output);
#pragma endregion

			void DigitalWrite(IDigitalOutput* output, DigitalValue state);
			void DigitalWrite(IDigitalOutput& output, DigitalValue state);

			void DigitalWrite(IDigitalOutput* output, DigitalValue state, unsigned long millis);
			void DigitalWrite(IDigitalOutput& output, DigitalValue state, unsigned long millis);

#pragma region Bindings and Configurations
			IDigitalIOMonitor& OnActivating(IDigitalInput* input);
			IDigitalIOMonitor& OnActivating(IDigitalInput& input);

			IDigitalIOMonitor& OnInactivating(IDigitalInput* input);
			IDigitalIOMonitor& OnInactivating(IDigitalInput& input);

			IDigitalIOMonitor& OnActivating(IDigitalOutput* output);
			IDigitalIOMonitor& OnActivating(IDigitalOutput& output);

			IDigitalIOMonitor& OnInactivating(IDigitalOutput* output);
			IDigitalIOMonitor& OnInactivating(IDigitalOutput& output);

			ITimedDigitalIOMonitor& OnActive(IDigitalInput* input);
			ITimedDigitalIOMonitor& OnActive(IDigitalInput& input);

			ITimedDigitalIOMonitor& OnInactive(IDigitalInput* input);
			ITimedDigitalIOMonitor& OnInactive(IDigitalInput& input);

			ITimedDigitalIOMonitor& OnActive(IDigitalOutput* output);
			ITimedDigitalIOMonitor& OnActive(IDigitalOutput& output);

			ITimedDigitalIOMonitor& OnInactive(IDigitalOutput* output);
			ITimedDigitalIOMonitor& OnInactive(IDigitalOutput& output);
#pragma endregion

			void Process();
			void ProcessAnalog();
			void ProcessDigital();
		private:
			template<typename T>
			class Listener
			{
			public:
				T& listener;
				bool shouldDelete;

				Listener(T& l) :
					shouldDelete(false),
					listener(l)
				{
				}

				Listener(T* l) :
					shouldDelete(false),
					listener(*l)
				{
				}

				operator T& () { return listener; }
				operator T* () { return &listener; }
			};

			DigitalIOMonitor& On(IDigitalInput& input, DigitalInputState state);
			DigitalIOMonitor& On(IDigitalOutput& input, DigitalInputState state);

			std::list<IOManagerAnalogInput> analogInputs;
			std::list<Listener<IAnalogInputListener> > analogInputListeners;

			std::list<IOManagerDigitalInput> digitalInputs;
			std::list<Listener<IDigitalInputListener> > digitalInputListeners;

			std::list<IOManagerDigitalOutput> digitalOutputs;
			std::list<Listener<IDigitalOutputListener> > digitalOutputListeners;

			std::list<DigitalIOMonitor*> monitors;

			class IOManagerRevertDigitalWrite : public virtual IRunnable, public virtual IDigitalOutputListener
			{
			public:
				IOManagerRevertDigitalWrite(IOManager& manager, IDigitalOutput& output, DigitalValue toState, unsigned long millis);
				~IOManagerRevertDigitalWrite();

				virtual void Run() override;

				virtual void OnActivating(int code, unsigned int count) override;
				virtual void OnInactivating(int code, unsigned int count) override;
			private:
				IOManager* manager;
				IDigitalOutput* output;
				DigitalValue toState;
			};
		};
	}
}