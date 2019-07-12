#include "App.h"

#define BUTTON_GPIO 3
#define LED_GPIO 13

App::App() :
	button(BUTTON_GPIO),
	led(LED_GPIO, Thing::Core::DigitalValue::Low)
{

}

App::~App()
{

}

void App::Setup()
{
	Manager.OnActivating(button).Toggle(led);
}

void App::Loop()
{
	Manager.Process();
}