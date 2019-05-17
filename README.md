# Thing.Core

Thing.Core is an IoT framework created for productivity over performance. With the rise of faster microcontrollers, such as ESP, code can be made in such way that losing a bit of performance over simplicity might be a good approach. Thing.Core was built to abstract completly from the machine you are programming, and you could even use it to run on Windows or even Linux, enabling you to mock your microcontroller and even debug your code in a Windows/Linux environment.

## How does it work?
Thing.Core, by itself isn't capable on anything extraordinary, so it should be used on top of another framework such as Arduino. The tutorials ahead focus on the use of the arduino, but they can also be used with any other framework as long as you tell Thing.Core how to deal with your target framework.

Thing.Core relies on the following things:
  - Hardware: In order for Thing.Core to talk with GPIOs, we will need to provide an implementation of how it should talk with the hardware.
  - TaskScheduler: In order to schedule tasks, we will need to provide and implementation on how we can schedule tasks to run later. Thing.Core already has a built-in implementation for this, allowing us to tell it to run a function after a few seconds or each few seconds.
  - LoggerManager: The logger manager allow us to send logs to whatever we want. It's a good feature that allow us to keep track (and maybe even debug) on what is happening on our device. LoggerManager can be used for us to log things over Serial or any other protocol such as MQTT or CoAP.
  - WiFi: In order for our Thing being truly part of the IoT, it should communicate over WiFi. We should provide Thing.Core on How to do such Thing.
  
## Why should I use it?
Although Thing.Core is something new and may be probably missing a few features, it allow us to easily program stuff. A few things Thing.Core can do are:
  - You can program with a single line to toogle a led every time you press a button:
  ```
  Manager.OnActivating(button).Toggle(led);
  ```
  - You can schedule a function to run when a button is pressed:
  ```
  Manager.OnActivating(button).Run([](void* obj){
    //Code here will run automatically when the button is pressed.
  });
  ```
  - You can schedule a function to run when an output turns inactive:
  ```
  Manager.OnInactivating(output).Run([](void* obj){
    //Code here will run automatically when output changes from High to Low.
  });
  ```
  - You can turn off a relay when another relay is turned on (useful for blinds):
  ```
  Manager.OnActivating(someRelay).SetLow(anotherRelay);
  ```
  - You can even program in a single line to turn a LED on only after 5 seconds since a button has been pressed:
  ```
  Manager.OnActive(button).For(5000).SetHigh(led);
  ```
  - You can schedule things:
  ```
  TaskScheduler.AttachOnce(5000, [](void* obj){
    //Code here will be run after 5 seconds
  });
  ```
  
These are only a few things you can do with a single line of code.

## Getting Started
We recommend to use Thing.Core with Platform IO. For those who don't know it, Platform IO is an IDE for microcontrollers and it can be used with Arduino and many other frameworks. Instructions on how to install it [here](https://docs.platformio.org/en/latest/ide/vscode.html#installation).

After the installation create a new Platform IO and copy this repository onto lib folder. Since we will be using ESP8266, we will also need to clone Thing.Core implementation for ESP8266 available [here](https://github.com/Alv3s/Thing.Core.ESP8266). By now, your new project should contain a ```Thing.Core``` and ```Thing.Core.ESP8266``` folders inside ```lib``` folder. We are now ready to use Thing.Core framework!

Thing.Core will handle most of our setup and loop for us, but we will need to tell Thing.Core what is the application it should run. For this, we will create into ```src``` folder two files:
  - ```App.h```
  - ```App.cpp```

Paste the following code inside ```App.h```:

```
#pragma once

#include "IApp.h"
#include "GenericDigitalOutput.h"
#include "GenericDigitalInput.h"
#include "IOManager.h"

class App : public virtual Thing::Core::IApp
{
public:
    App();
    ~App();

    void Setup() override;
    void Loop() override;
private:
    Thing::Core::IOManager Manager;
    
    Thing::Core::GenericDigitalInput button;
    Thing::Core::GenericDigitalOutput led;
};
```

In the above code we declared an App that implements Thing.Core ```IApp```. In Thing.Core, every Application should implement ```Thing::Core::IApp``` in order for Thing.Core knowing how to deal with it. By implementing ```IApp```, we should then override two method:
- ```void Setup() override```: Thing.Core will use this method to setup your Application. This method will be run once and only once by Thing.Core.
- ```void Loop() override```: Thing.Core will use this method to run your Application. This method will be run on an infinite loop automatically by Thing.Core.

Also, we have declared the following:
- ```Thing::Core::IOManager Manager;```: we have declared an instance of one of the most powerful classes Thing.Core has. You will see what it can do below.
- ```Thing::Core::GenericDigitalInput button;``` we have declared a General Purpose Input that will represent our button.
- ```Thing::Core::GenericDigitalOutput led;``` we have declared a General Purpose Output that will represent our led.

Paste the following code inside ```App.cpp```:
```
#include "App.h"

App::App() :
    button(3),
    led(13, Thing::Core::DigitalValue::Low)
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
```

In ```App.cpp``` we have included the actual code. As you can see, in the App Constructor we have two initializations:
- ```button(3)```: Here we are configuring our General Purpose Input ```button``` to use our GPIO number 3.
- ```led(13, Thing::Core::DigitalValue::Low)```: Here we configured our General Purpose Output ```output``` to use our GPIO number 13 and be initialized with the value ```Low```. If we wanted, we could change this value to ```High```in order for it to start at 'On' state.
- 
In the function ```void App::Setup()``` we will find the following lines:
- ```Manager.OnActivating(button).Toggle(led);```: Here we say that, every time the button changes its state from ```Low``` to ```High```, then it should toggle the state of our ```led```.

In the function ```void App::Loop()``` we will only find the line ```Manager.Process();```. This line will do everything for you. The IOManager works in a non-blocking way, meaning that you can add lines afterwards or bellow and still be sure that ```Manager.Process()``` will not be waiting for something to happen.

One last thing, you should change your ```main.cpp``` to the following:
```
#include "ArduinoMain.h"
#include "App.h"

Thing::Core::IApp* InitializeApp()
{
  return new App();
}
```
In order for Thing.Core running your app, you have to tell it where your app is and how to create one. Thing.Core will find for a the function above ```Thing::Core::IApp* InitializeApp()``` and will run initialize your app when its ready.

After compiling and uploading to ESP, you should see that your led toggles automatically whenever you press the button!
