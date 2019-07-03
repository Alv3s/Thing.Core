#ifdef ESP8266
#include "ArduinoHardware.h"
#include <Arduino.h>

Thing::Core::IRunnable* ArduinoHardware::interrupts[16];

void ArduinoHardware::ConfigurePin(int gpio, Thing::Core::PinMode mode)
{
    switch(mode)
    {
        case Thing::Core::PinMode::Input:
            pinMode(gpio, INPUT);
            break;
        case Thing::Core::PinMode::Output:
            pinMode(gpio, OUTPUT);
    }
}

void ArduinoHardware::DigitalWrite(int gpio, Thing::Core::DigitalValue value)
{
    switch(value)
    {
		case Thing::Core::DigitalValue::Low:
            digitalWrite(gpio, LOW);
            break;
        case Thing::Core::DigitalValue::High:
            digitalWrite(gpio, HIGH);
    }
}

Thing::Core::DigitalValue ArduinoHardware::DigitalRead(int gpio)
{
    bool value = digitalRead(gpio);
    return value ? Thing::Core::DigitalValue::High : Thing::Core::DigitalValue::Low;
}

int ArduinoHardware::AnalogRead(int gpio)
{
    return analogRead(gpio);
}

unsigned long ArduinoHardware::Millis()
{
	return millis();
}

unsigned long ArduinoHardware::Micros()
{
	return micros();
}

void ArduinoHardware::Delay(unsigned long millis)
{
	delay(millis);
}


void ArduinoHardware::AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable * runnable)
{
	int aMode = RISING;
	switch (mode)
	{
	case Thing::Core::InterruptMode::WasActivated: aMode = RISING;  break;
	case Thing::Core::InterruptMode::WasInactivated: aMode = FALLING;  break;
	case Thing::Core::InterruptMode::OnChange: aMode = CHANGE;  break;
	}

	int interrupt = digitalPinToInterrupt(gpio);
	interrupts[interrupt] = runnable;

	switch (interrupt)
	{
	case 0: attachInterrupt(interrupt, interrupt0, aMode); break;
	case 1: attachInterrupt(interrupt, interrupt1, aMode); break;
	case 2: attachInterrupt(interrupt, interrupt2, aMode); break;
	case 3: attachInterrupt(interrupt, interrupt3, aMode); break;
	case 4: attachInterrupt(interrupt, interrupt4, aMode); break;
	case 5: attachInterrupt(interrupt, interrupt5, aMode); break;
	case 6: attachInterrupt(interrupt, interrupt6, aMode); break;
	case 7: attachInterrupt(interrupt, interrupt7, aMode); break;
	case 8: attachInterrupt(interrupt, interrupt8, aMode); break;
	case 9: attachInterrupt(interrupt, interrupt9, aMode); break;
	case 10: attachInterrupt(interrupt, interrupt10, aMode); break;
	case 11: attachInterrupt(interrupt, interrupt11, aMode); break;
	case 12: attachInterrupt(interrupt, interrupt12, aMode); break;
	case 13: attachInterrupt(interrupt, interrupt13, aMode); break;
	case 14: attachInterrupt(interrupt, interrupt14, aMode); break;
	case 15: attachInterrupt(interrupt, interrupt15, aMode); break;
	}
	
}

void ArduinoHardware::AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable & runnable)
{
	AttachInterrupt(gpio, mode, &runnable);
}

void ArduinoHardware::DetachInterrupt(int gpio)
{
	int interrupt = digitalPinToInterrupt(gpio);
	detachInterrupt(interrupt);
}
#endif