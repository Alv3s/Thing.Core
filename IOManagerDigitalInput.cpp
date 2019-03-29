#include "IOManagerDigitalInput.h"
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		// TODO: DMARTINS 2018-12-14 tornar _triggerTimeout_ms configuravel
		IOManagerDigitalInput::IOManagerDigitalInput() : 
			_triggerTimeout_ms(500), // Default value
			_lastWasActivatedMillis(0),
			_lastWasInactivatedMillis(0)
		{

		}

		// TODO: DMARTINS 2019-02-22 adicionar o metodo SetTimeout e GetTimeout

		IOManagerDigitalInput::~IOManagerDigitalInput()
		{
		}

		IDigitalInput* IOManagerDigitalInput::GetInput() 
		{
			return DigitalInput;
		}

		unsigned int IOManagerDigitalInput::GetWasActivatedCount() 
		{
			return WasActivatedCount;
		}

		unsigned int IOManagerDigitalInput::GetWasInactivatedCount()
		{
			return WasInactivatedCount;
		}

		bool IOManagerDigitalInput::Contains(std::list<IOManagerDigitalInput>& list, IDigitalInput * input)
		{
			for (auto &item : list)
			{
				if (item.DigitalInput == input)
					return true;
			}
			return false;
		}

		unsigned int IOManagerDigitalInput::IncrementWasActivatedCount()
		{
			auto currentMillis = Hardware->Millis();
			
			if ((currentMillis - _lastWasActivatedMillis) > _triggerTimeout_ms)
				WasActivatedCount = 0;

			_lastWasActivatedMillis = currentMillis;

			return ++WasActivatedCount;
		}

		unsigned int IOManagerDigitalInput::IncrementWasInactivatedCount()
		{
			auto currentMillis = Hardware->Millis();

			if ((currentMillis - _lastWasInactivatedMillis) > _triggerTimeout_ms)
				WasInactivatedCount = 0;

			_lastWasInactivatedMillis = currentMillis;

			return ++WasInactivatedCount;
		}
		int IOManagerDigitalInput::GetCode() const
		{
			return this->DigitalInput->GetCode();
		}
		void IOManagerDigitalInput::SetCode(int code)
		{
			this->DigitalInput->SetCode(code);
		}
		DigitalValue IOManagerDigitalInput::DigitalRead()
		{
			return this->DigitalInput->DigitalRead();
		}
	}
}
