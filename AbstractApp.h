#pragma once

#include <list>
#include "IApp.h"

namespace Thing
{
	namespace Core
	{
		class AbstractApp : public virtual IApp
		{
		public:
			virtual void AddListener(IAppListener* listener) override;
			virtual void RemoveListener(IAppListener* listener) override;

			virtual void AddListener(IAppListener& listener) override;
			virtual void RemoveListener(IAppListener& listener) override;

			virtual void Setup() override;
			virtual void Loop() override;

			virtual void OnSetup() = 0;
			virtual void OnLoop() = 0;
		private:
			std::list<IAppListener*> _listeners;
		};
	}
}