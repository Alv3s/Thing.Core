#pragma once

#include "HardwareMock.h"
#include "IMillisListener.h"
#include <list>

namespace Thing {
	namespace Core {
		namespace Tests {
			class TimedHardwareMock : public virtual HardwareMock
			{
			public:
				TimedHardwareMock() : _millis(0) {}
				~TimedHardwareMock() {}

				void AddListener(IMillisListener* listener)
				{
					listeners.push_back(listener);
				}

				void RemoveListener(IMillisListener* listener)
				{
					listeners.remove(listener);
				}

				virtual unsigned long Millis() override
				{
					return _millis;
				}

				virtual void Delay(unsigned long millis) override
				{
					while (millis > 0)
					{
						++_millis;
						--millis;

						for (auto l : listeners)
							l->OnMillisChange(_millis);
					}
				}

				void ResetTimer()
				{
					_millis = 0;
				}
			private:
				unsigned long _millis;
				std::list<IMillisListener*> listeners;
			};
		}
	}
}