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

			virtual unsigned int GetPrecision() override;
			virtual int AnalogRead() override;
		private:
			const int _gpio;

			unsigned int _precision;
		};
	}
}