#pragma once

#include "../IHardware.h"

#define INTERRUPT_METHOD(n) static void interrupt ## n() { interrupts[n]->Run();}

class ArduinoHardware : public virtual Thing::Core::IHardware
{
public:
    virtual void ConfigurePin(int gpio, Thing::Core::PinMode mode) override;

    virtual void DigitalWrite(int gpio, Thing::Core::DigitalValue value) override;
    virtual Thing::Core::DigitalValue DigitalRead(int gpio) override;

    virtual int AnalogRead(int gpio) override;

	virtual unsigned long Millis() override;
	virtual unsigned long Micros() override;
	virtual void Delay(unsigned long millis) override;

	virtual void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable* runnable) override;
	virtual void AttachInterrupt(int gpio, Thing::Core::InterruptMode mode, Thing::Core::IRunnable& runnable) override;
	virtual void DetachInterrupt(int gpio) override;

	virtual void Reset() override;
private:
	static Thing::Core::IRunnable* interrupts[16];

	INTERRUPT_METHOD(0);
	INTERRUPT_METHOD(1);
	INTERRUPT_METHOD(2);
	INTERRUPT_METHOD(3);
	INTERRUPT_METHOD(4);
	INTERRUPT_METHOD(5);
	INTERRUPT_METHOD(6);
	INTERRUPT_METHOD(7);
	INTERRUPT_METHOD(8);
	INTERRUPT_METHOD(9);
	INTERRUPT_METHOD(10);
	INTERRUPT_METHOD(11);
	INTERRUPT_METHOD(12);
	INTERRUPT_METHOD(13);
	INTERRUPT_METHOD(14);
	INTERRUPT_METHOD(15);
};

#undef INTERRUPT_METHOD(n)