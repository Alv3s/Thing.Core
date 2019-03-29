#pragma once
#include "IDigitalInput.h"
#include "HardwareEnumerations.h"
#include <list>

namespace Thing
{
	namespace Core
	{
		class IOManagerDigitalInput : public IDigitalInput
		{
			friend class IOManager;
		public:
			IOManagerDigitalInput();
			~IOManagerDigitalInput();

			IDigitalInput* GetInput();
			unsigned int GetWasActivatedCount();
			unsigned int GetWasInactivatedCount();

			IDigitalInput* DigitalInput;

			// Inherited via IDigitalInput
			virtual int GetCode() const override;
			virtual void SetCode(int code) override;
			virtual DigitalValue DigitalRead() override;
		private:
			/// <summary>
			/// Validates if a Digital Input exists in a list of IOManagerDigitalInputs.
			/// </summary>
			/// <param name="list">The list.</param>
			/// <param name="input">The digital input to validate.</param>
			/// <returns><c>True</c> if the input is contained in the list, otherwise <c>False</c></returns>
			static bool Contains(std::list<IOManagerDigitalInput> &list, IDigitalInput* input);
			/// <summary>
			/// Increment the trigger count and returns updated value.
			/// Validates if timeout was reached before increment.
			/// </summary>
			/// <returns>
			/// Return the TriggerCount after validate the timeout. If timout was reached than returns 1 otherwise returns current count.
			/// </returns>
			unsigned int IncrementWasActivatedCount();
			unsigned int IncrementWasInactivatedCount();

			
			DigitalValue LastReadValue;
			unsigned int WasActivatedCount;
			unsigned int WasInactivatedCount;
			unsigned int _triggerTimeout_ms;
			unsigned long _lastWasInactivatedMillis;
			unsigned long _lastWasActivatedMillis;
		};
	}
}