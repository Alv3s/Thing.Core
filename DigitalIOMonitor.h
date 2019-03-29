#pragma once

#include "IDigitalIOListeners.h"
#include "IDigitalInput.h"
#include "IDigitalIOMonitor.h"

namespace Thing
{
	namespace Core
	{
		class DigitalIOMonitor : public virtual IDigitalIOMonitor, private virtual IDigitalInputListener, private virtual IDigitalOutputListener
		{
			friend class IOManager;
		public:
			virtual ~DigitalIOMonitor();

			void Toggle(IDigitalOutput& output) override;
			void Toggle(IDigitalOutput* output) override;

			IDigitalOutputMonitor& SetHigh(IDigitalOutput& output) override;
			IDigitalOutputMonitor& SetHigh(IDigitalOutput* output) override;

			IDigitalOutputMonitor& SetLow(IDigitalOutput& output) override;
			IDigitalOutputMonitor& SetLow(IDigitalOutput* output) override;
		private:
			DigitalIOMonitor(class IOManager& manager, IDigitalInput* input, DigitalInputState action);
			DigitalIOMonitor(class IOManager& manager, IDigitalOutput* output, DigitalInputState action);

			DigitalOutputMonitor outputMonitor;
			const IDigitalIO* io;
			const DigitalInputState actionType;

			virtual void OnActivating(int code) override;
			virtual void OnActivating(int code, unsigned int count) override;
			virtual void OnInactivating(int code) override;
			virtual void OnInactivating(int code, unsigned int count) override;
		};
	}
}