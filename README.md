# Thing.Core

Thing.Core is an IoT framework created for productivity over performance. With the rise of faster microcontrollers, such as ESP, code can be made in such way that losing a bit of performance over simplicity might be a good approach. Thing.Core was built to abstract completly from the machine you are programming, and you could even use it to run on Windows or even Linux, enabling you to mock your microcontroller and even debug your code in a Windows/Linux environment.

###### How does it work?
Thing.Core, by itself isn't capable on anything extraordinary, so it should be used on top of another framework such as Arduino. The tutorials ahead focus on the use of the arduino, but they can also be used with any other framework as long as you tell Thing.Core how to deal with your target framework.

Thing.Core relies on the following things:
  - Hardware: In order for Thing.Core to talk with GPIOs, we will need to provide an implementation of how it should talk with the hardware.
  - TaskScheduler: In order to schedule tasks, we will need to provide and implementation on how we can schedule tasks to run later. Thing.Core already has a built-in implementation for this, allowing us to tell it to run a function after a few seconds or each few seconds.
  - LoggerManager: The logger manager allow us to send logs to whatever we want. It's a good feature that allow us to keep track (and maybe even debug) on what is happening on our device. LoggerManager can be used for us to log things over Serial or any other protocol such as MQTT or CoAP.
  - WiFi: In order for our Thing being truly part of the IoT, it should communicate over WiFi. We should provide Thing.Core on How to do such Thing.
  
###### Why should I use it?
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
