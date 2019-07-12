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
	TaskScheduler->AttachPeriodic(1000, [this](void* obj) {
		Manager.DigitalWrite(led, Thing::Core::DigitalValue::Toggle);
	});
}

void App::Loop()
{
	Manager.Process();
}

Thing::Core::IApp* InitializeApp(int argc, char* argv[])
{
	return new App();
}