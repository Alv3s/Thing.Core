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
			void SetApp(IApp* app) override;
			void SetApp(IApp& app) override;

			void AddListener(IAppListener* listener) override;
			void RemoveListener(IAppListener* listener) override;

			void AddListener(IAppListener& listener) override;
			void RemoveListener(IAppListener& listener) override;

			void Setup() override;
			void Loop() override;
		private:
			IApp* app;
			std::list<IAppListener*> _listeners;
		};
	}
}