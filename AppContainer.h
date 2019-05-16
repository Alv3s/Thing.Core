#pragma once

#include <list>
#include "IAppContainer.h"

namespace Thing
{
	namespace Core
	{
		class AppContainer : public virtual IAppContainer
		{
		public:
			virtual void SetApp(IApp* app) override;
			virtual void SetApp(IApp& app) override;

			virtual void AddListener(IAppListener* listener) override;
			virtual void RemoveListener(IAppListener* listener) override;

			virtual void AddListener(IAppListener& listener) override;
			virtual void RemoveListener(IAppListener& listener) override;

			virtual void Setup() override;
			virtual void Loop() override;
		private:
			IApp* app;
			std::list<IAppListener*> _listeners;
		};
	}
}