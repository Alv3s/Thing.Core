#pragma once

#include "IDigitalInput.h"
#include <type_traits>
#include <cstdint>
#include "IoC.h"

namespace Thing
{
	namespace Core
	{
		template<typename T, typename std::enable_if<std::is_base_of<IDigitalInput, T>::value>::type* = nullptr>
		class DebouncedInput : public T, public virtual IDigitalInput
		{
		public:
			template<class... TARGS>
			DebouncedInput(unsigned long debounceTime, TARGS... a) :
				T(a...),
				debouncingTime(debounceTime),
				timer(NULL)
			{
				previousState = T::DigitalRead();
			}

			DigitalValue DigitalRead() override
			{
				DigitalValue newState = T::DigitalRead();
				if (timer != NULL)
				{
					if(Hardware->Millis() - *timer < debouncingTime)
						return previousState;
					else
					{
						delete timer;
						timer = NULL;
					}
				}
				if (newState != previousState)
					timer = new unsigned long(Hardware->Millis());

				previousState = newState;
				return newState;
			}
		private:
			DigitalValue previousState;
			const unsigned long debouncingTime;
			unsigned long* timer;
		};
	}
}