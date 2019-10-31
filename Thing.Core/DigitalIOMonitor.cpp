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
			callback(std::bind(&DigitalIOMonitor::Run2, this)),
			periodic(false),
			handle(NULL)
		{
			manager.AddDigitalInput(input);
		}

		DigitalIOMonitor::DigitalIOMonitor(IOManager& manager, IDigitalOutput* output, DigitalInputState action) : 
			outputMonitor(manager), 
			io(output), 
			actionType(action),
			timePressedMillis(0),
			callback(std::bind(&DigitalIOMonitor::Run2, this)),
			periodic(false),
			handle(NULL)
		{
			manager.AddDigitalOutput(output);
		}

		DigitalIOMonitor::~DigitalIOMonitor()
		{
			TaskScheduler->Detach(handle);
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
						handle = TaskScheduler->AttachPeriodic(timePressedMillis, this);
					else
						handle = TaskScheduler->AttachOnce(timePressedMillis, this);
				}
				else
					TaskScheduler->Detach(handle);
				return;
			}

			if (actionType == DigitalInputState::WasActivated)
				this->Run();
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
						handle = TaskScheduler->AttachPeriodic(timePressedMillis, this);
					else
						handle = TaskScheduler->AttachOnce(timePressedMillis, this);
				}
				else
					TaskScheduler->Detach(handle);
				return;
			}

			if (actionType == DigitalInputState::WasInactivated)
				this->Run();
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
			callback = std::function<void()>(std::bind(&IRunnable::Run, runnable));
		}

		void DigitalIOMonitor::RunMe()
		{
			callback = std::function<void()>(std::bind(&DigitalIOMonitor::Run2, this));
		}

		void DigitalIOMonitor::Run(RunnableCallback f)
		{
			callback = f;
		}

		void DigitalIOMonitor::Run()
		{
			callback();
		}

		void DigitalIOMonitor::Run2()
		{
			outputMonitor.Action();
		}
	}
}