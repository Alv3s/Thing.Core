#pragma once

#include "IAnalogIO.h"

namespace Thing
{
	namespace Core
	{
		/// <summary>
		/// An interface class to subscribe AnalogInput events.
		/// </summary>
		class IAnalogIOListener
		{
		public:
			/// <summary>
			/// This method is called when the current AnalogInput read value is higher than the previous read value.
			/// </summary>
			/// <param name="io">The AnalogIO triggering the event.</param>
			/// <param name="value">The currennt value of AnalogInput.</param>
			virtual void OnIncreasingValue(IAnalogIO* io, int value) = 0;
			/// <summary>
			/// This method is called when the current AnalogInput read value is lower than the previous read value.
			/// </summary>
			/// <param name="io">The AnalogIO triggering the event.</param>
			/// <param name="value">The currennt value of AnalogInput.</param>
			virtual void OnDecreasingValue(IAnalogIO* io, int value) = 0;
		};

		/// <summary>
		/// An interface class to subscribe AnalogInput events.
		/// </summary>
		class IAnalogInputListener : public virtual IAnalogIOListener
		{
		};
	}
}