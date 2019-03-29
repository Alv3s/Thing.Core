#pragma once

#include "IDigitalInput.h"

namespace Thing
{
	namespace Core
	{
		class GenericDigitalInput : public virtual IDigitalInput
		{
		public:
			GenericDigitalInput(int gpio, int code);
			~GenericDigitalInput();

			virtual DigitalValue DigitalRead() override;
			virtual int GetCode() const override;
			virtual void SetCode(int code) override;
		private:
			const int _gpio;
			int _code;
		};
	}
}