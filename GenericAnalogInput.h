#pragma once

#include "IAnalogInput.h"

namespace Thing
{
	namespace Core
	{
		class GenericAnalogInput : public virtual IAnalogInput
		{
		public:
			GenericAnalogInput(int gpio, int code, int precision);
			~GenericAnalogInput();

			virtual unsigned int GetPrecision() override;
			virtual int AnalogRead() override;
			virtual int GetCode() const override;
			virtual void SetCode(int code) override;
		private:
			const int _gpio;
			int _code;

			unsigned int _precision;
		};
	}
}