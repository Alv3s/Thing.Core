#pragma once

#include "IAnalogInput.h"

namespace Thing
{
	namespace Core
	{
		class GenericAnalogInput : public virtual IAnalogInput
		{
		public:
			GenericAnalogInput(int gpio, int precision);
			~GenericAnalogInput();

			unsigned int GetPrecision() override;
			int AnalogRead() override;
		private:
			const int _gpio;

			unsigned int _precision;
		};
	}
}