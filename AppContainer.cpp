#include "AppContainer.h"

namespace Thing
{
	namespace Core
	{
		void AppContainer::SetApp(IApp* app)
		{
			this->app = app;
		}

		void AppContainer::SetApp(IApp& app)
		{
			SetApp(&app);
		}

		void AppContainer::AddListener(IAppListener* listener)
		{
			_listeners.push_back(listener);
		}

		void AppContainer::RemoveListener(IAppListener* listener)
		{
			_listeners.remove(listener);
		}



		void AppContainer::AddListener(IAppListener& listener)
		{
			AddListener(&listener);
		}

		void AppContainer::RemoveListener(IAppListener& listener)
		{
			RemoveListener(&listener);
		}



		void AppContainer::Setup()
		{
			app->Setup();
		}

		void AppContainer::Loop()
		{
			app->Loop();
			for (std::list<IAppListener*>::iterator it = _listeners.begin(); it != _listeners.end(); )
			{
				if ((*it)->OnLoop())
					it = _listeners.erase(it);
				else
					++it;
			}
		}
	}
}