//This file is only compiled when running Thing.Core on Windows.
#ifdef _WIN32
#include "Thing.Core/IApp.h"
#include "App.h"

Thing::Core::IApp* InitializeApp(int argc, char* argv[])
{
	return new App();
}
#endif