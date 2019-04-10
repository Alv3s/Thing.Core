#pragma once

namespace Thing {
	namespace Core {
		namespace Tests {
			class IMillisListener
			{
			public:
				virtual void OnMillisChange(unsigned long millis) = 0;
			};
		}
	}
}