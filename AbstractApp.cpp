#include "AbstractApp.h"

namespace Thing
{
	namespace Core
	{
		void AbstractApp::AddListener(IAppListener* listener)
		{
			_listeners.push_back(listener);
		}

		void AbstractApp::RemoveListener(IAppListener* listener)
		{
			_listeners.remove(listener);
		}



		void AbstractApp::AddListener(IAppListener& listener)
		{
			AddListener(&listener);
		}

		void AbstractApp::RemoveListener(IAppListener& listener)
		{
			RemoveListener(&listener);
		}



		void AbstractApp::Setup()
		{
			this->OnSetup();
		}

		void AbstractApp::Loop()
		{
			this->OnLoop();
			for (std::list<IAppListener*>::iterator it = _listeners.begin(); it != _listeners.end(); )
			{
				if ((*it)->OnLoop())
					it = _listeners.erase(it);
				else
					it++;
			}
		}
	}
}