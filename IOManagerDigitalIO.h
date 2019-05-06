#pragma once
#include "IDigitalIO.h"
#include "HardwareEnumerations.h"
#include <list>
#include "IoC.h"
#include <algorithm>

namespace Thing
{
	namespace Core
	{
		template<typename T, typename std::enable_if<std::is_base_of<IDigitalIO, T>::value>::type* = nullptr>
		class IOManagerDigitalIO
		{
			friend class IOManager;
		public:
			IOManagerDigitalIO() : 
				_triggerTimeout_ms(500), // Default value
				_lastWasActivatedMillis(0),
				_lastWasInactivatedMillis(0)
			{

			}

			~IOManagerDigitalIO()
			{

			}

			unsigned int GetWasActivatedCount()
			{
				return WasActivatedCount;
			}

			unsigned int GetWasInactivatedCount()
			{
				return WasInactivatedCount;
			}

			operator T& () { return *DigitalIO; }
			operator T* () { return DigitalIO; }

			operator IOManagerDigitalIO& () { return *DigitalIO; }
			operator IOManagerDigitalIO* () { return DigitalIO; }

		private:
			T* DigitalIO;

			template<typename Y, typename std::enable_if<std::is_base_of<IOManagerDigitalIO, Y>::value>::type* = nullptr>
			static bool Contains(std::list<Y>& list, T* element)
			{
				return std::find(list.begin(), list.end(), element) != list.end();
			}

			template<typename Y, typename std::enable_if<std::is_base_of<IOManagerDigitalIO, Y>::value>::type* = nullptr>
			static typename std::list<Y>::iterator Find(std::list<Y> & list, T* element)
			{
				return std::find(list.begin(), list.end(), element);
			}


			/// <summary>
			/// Increment the trigger count and returns updated value.
			/// Validates if timeout was reached before increment.
			/// </summary>
			/// <returns>
			/// Return the TriggerCount after validate the timeout. If timout was reached than returns 1 otherwise returns current count.
			/// </returns>
			unsigned int IncrementWasActivatedCount()
			{
				auto currentMillis = Hardware->Millis();

				if ((currentMillis - _lastWasActivatedMillis) >= _triggerTimeout_ms)
					WasActivatedCount = 0;

				_lastWasActivatedMillis = currentMillis;

				return ++WasActivatedCount;
			}

			unsigned int IncrementWasInactivatedCount()
			{
				auto currentMillis = Hardware->Millis();

				if ((currentMillis - _lastWasInactivatedMillis) >= _triggerTimeout_ms)
					WasInactivatedCount = 0;

				_lastWasInactivatedMillis = currentMillis;

				return ++WasInactivatedCount;
			}

			unsigned int WasActivatedCount;
			unsigned int WasInactivatedCount;
			unsigned int _triggerTimeout_ms;
			unsigned long _lastWasInactivatedMillis;
			unsigned long _lastWasActivatedMillis;
		};
	}
}