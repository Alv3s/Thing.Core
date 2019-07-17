#pragma once

#include "Thing.Core/IApp.h"
#include "Thing.Core/DebouncedInput.h"
#include "Thing.Core/GenericDigitalInput.h"
#include "Thing.Core/IOManager.h"

class App : public virtual Thing::Core::IApp
{
public:
	App();
	~App();

	void Setup() override;
	void Loop() override;
private:
	Thing::Core::IOManager Manager;

	Thing::Core::DebouncedInput<Thing::Core::GenericDigitalInput> button;
};