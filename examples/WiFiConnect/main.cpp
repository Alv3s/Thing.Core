#include "Thing.Core/Arduino/Main.h"
#include "App.h"

Thing::Core::IApp* InitializeApp()
{
	return new App();
}