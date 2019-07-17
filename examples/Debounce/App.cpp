#include "App.h"

#define DEBOUNCE_TIME_MS 50
#define BUTTON_GPIO 3

App::App() :
	button(50, BUTTON_GPIO) //DebouncedInput will use the same constructor as the 
							//GenericDigitalInput, with the only exception being it also
							//receives as first argument the debounce time
{

}

App::~App()
{

}

void App::Setup()
{
	//Add Digital Input To the IOManager
	Manager.AddDigitalInput(button);
}

void App::Loop()
{
	Manager.Process();
}