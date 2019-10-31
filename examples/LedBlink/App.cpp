#include "App.h"
#include "Thing.Core/IoC.h"

#define LED_GPIO 13

App::App() :
	led(LED_GPIO, Thing::Core::DigitalValue::Low)
{

}

App::~App()
{

}

void App::Setup()
{
	//Schedule a Task to run periodically each 1000 milliseconds, toggling the led.
	TaskScheduler->AttachPeriodic(1000, [this]() {
		Manager.DigitalWrite(led, Thing::Core::DigitalValue::Toggle);
	});
}

void App::Loop()
{
	Manager.Process();
}