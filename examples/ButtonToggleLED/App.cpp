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
	//Here we tell the IOManager each time the button changes state to HIGH, it should toggle the led.
	// This line will automatically call Manager.AddDigitalInput(button) and Manager.AddDigitalOutput(led)
	Manager.OnActivating(button).Toggle(led);
}

void App::Loop()
{
	Manager.Process();
}