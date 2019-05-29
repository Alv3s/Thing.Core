#pragma once

#include "IThresholdedAnalogInput.h"

namespace Thing
{
	namespace Core
	{
		class ThresholdedAnalogInput : public virtual IThresholdedAnalogInput
		{
		public:
			ThresholdedAnalogInput(int _gpio, int precision);
			~ThresholdedAnalogInput();

			void SetThresholdHigherThan(int value);
			void SetThresholdLesserThan(int value);

			virtual DigitalValue DigitalRead() override;
			virtual unsigned int GetPrecision() override;
			virtual int AnalogRead() override;
		private:
			const int _gpio;
			unsigned int _precision;

			DigitalValue privateDigitalRead();

			int _threshold;
			bool _higherThreshold;
		};
	}
}