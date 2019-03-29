#pragma once

#include <list>
#include <map>
#include "IAnalogInput.h"
#include "IDigitalInput.h"
#include "IAnalogIOListener.h"
#include "IDigitalIOListeners.h"
#include "DigitalIOMonitor.h"
#include "IOManagerDigitalInput.h"
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

			IDigitalIOMonitor& OnActivating(IDigitalInput* input);
			IDigitalIOMonitor& OnActivating(IDigitalInput& input);

			IDigitalIOMonitor& OnInactivating(IDigitalInput* input);
			IDigitalIOMonitor& OnInactivating(IDigitalInput& input);

			IDigitalIOMonitor& OnActivating(IDigitalOutput* output);
			IDigitalIOMonitor& OnActivating(IDigitalOutput& output);

			IDigitalIOMonitor& OnInactivating(IDigitalOutput* output);
			IDigitalIOMonitor& OnInactivating(IDigitalOutput& output);

			void Process();
			void ProcessAnalog();
			void ProcessDigital();
		private:
			IDigitalIOMonitor & On(IDigitalInput& input, DigitalInputState state);
			IDigitalIOMonitor & On(IDigitalOutput& input, DigitalInputState state);

			std::list<IOManagerAnalogInput> analogInputs;
			std::list<IAnalogInputListener*> analogInputListeners;

			std::list<IOManagerDigitalInput> digitalInputs;
			std::list<IDigitalInputListener*> digitalInputListeners;

			std::list<IDigitalOutput*> digitalOutputs;
			std::list<IDigitalOutputListener*> digitalOutputListeners;

			std::list<DigitalIOMonitor*> monitors;
		};
	}
}