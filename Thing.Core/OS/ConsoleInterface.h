#pragma once

#include "../IApp.h"
#include "../ILoggerListener.h"
#include "CLI11.h"
#include <list>
#include "FakeHardware.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include "../IoC.h"
#include <conio.h>

namespace Thing
{
	namespace Core
	{
		namespace OS
		{
			class ConsoleInterface : public virtual ILoggerListener, public virtual IAppListener, public virtual IGPIOListener
			{
			public:
				ConsoleInterface() : _name("Console Logger"), 
									app("Thing.Core Simulation")
				{
					// By default min level is Trace and max level is Fatal
					SetMinLevel(Thing::Core::LogLevel::Trace);
					SetMaxLevel(Thing::Core::LogLevel::Fatal);

					app.require_subcommand(); // 1 or more

					app.add_subcommand("exit", "Exits simulator.");
					CLI::App* gpio = app.add_subcommand("gpio", "Select a GPIO to perform operation.");
					gpio->require_option(1);
					gpio->require_subcommand(1);
					auto gpioOption = gpio->add_option("number", "The number of the gpio");

					CLI::App* write = gpio->add_subcommand("write", "Select 'Write' operation into the selected GPIO.");
					write->require_subcommand(1);
					write->add_subcommand("toggle", "Toggles the state of the selected GPIO.");
					write->add_subcommand("high", "Sets to high the state of the selected GPIO.");
					write->add_subcommand("low", "Sets to high the state of the selected GPIO.");

					CLI::App* read = gpio->add_subcommand("read", "Select 'Read' operation into the selected GPIO.");
					CLI::App* watch = gpio->add_subcommand("watch", "Adds a watch on the specified GPIO to be notified immediatly when changing state.");
				}

				~ConsoleInterface()
				{

				}

				std::string GetLoggerName() override
				{
					return this->_name;
				}

				void SetMinLevel(LogLevel minLevel) override
				{
					this->_minLevel = minLevel;
				}

				LogLevel GetMinLevel() override
				{
					return this->_minLevel;
				}

				void SetMaxLevel(LogLevel maxLevel) override
				{
					this->_maxLevel = maxLevel;
				}

				LogLevel GetMaxLevel() override
				{
					return this->_maxLevel;
				}

				void OnTrace(std::string message) override
				{
					printTime();
					std::cout << "[TRACE] " << message << std::endl;
					printInput();
				}

				void OnDebug(std::string message) override
				{
					printTime();
					std::cout << "[DEBUG] " << message << std::endl;
				}

				void OnInfo(std::string message) override
				{
					printTime();
					std::cout << "[INFO] " << message << std::endl;
				}

				void OnWarn(std::string message) override
				{
					printTime();
					std::cout << "[WARN] " << message << std::endl;
				}

				void OnError(std::string message) override
				{
					printTime();
					std::cout << "[ERROR] " << message << std::endl;
				}

				void OnFatal(std::string message) override
				{
					printTime();
					std::cout << "[FATAL] " << message << std::endl;
				}

				bool OnLoop() override
				{
					clearLine();
					if (commandResponse.size() > 0)
					{
						std::cout << commandResponse;
						commandResponse = "";
					}
					printInput();
					return true;
				}

				void ProcessInput()
				{
					::AppContainer->AddListener(this);
					for (;;)
					{
						char c = _getch();
						if (c == '\n' || c == '\r')
						{
							if (inputCommand.size() > 0)
								break;
							continue;
						}
						if (c == 8)
						{
							inputCommand = inputCommand.substr(0, inputCommand.size() - 1);
						}
						else if (c == (char)0xe0)
						{
							c = _getch();
							if (c == (char)0x48) //Key Up
							{
								++historyCommandIndex;
								if (historyCommandIndex >= commandHistory.size())
									historyCommandIndex = commandHistory.size() - 1;
								if (commandHistory.size() > 0)
									inputCommand = *(std::next(commandHistory.begin(), historyCommandIndex));
							}
							else if (c == (char)0x50) //Key Down
							{
								--historyCommandIndex;
								if (historyCommandIndex < 0)
								{
									historyCommandIndex = 0;
									inputCommand = "";
								}
								else if (commandHistory.size() > 0)
									inputCommand = *(std::next(commandHistory.begin(), historyCommandIndex));
							}
						}
						else
							inputCommand += c;
						::AppContainer->AddListener(this);
					}
					std::string command = inputCommand;
					commandResponse = "> " + inputCommand + "\n";

					commandHistory.push_front(inputCommand);
					historyCommandIndex = -1;
					inputCommand = "";
					try {
						app.parse(command);
						auto gpio = app.get_subcommand("gpio");
						auto exit = app.get_subcommand("exit");
						if (gpio->parsed())
						{
							int number = gpio->get_option("number")->as<int>();
							if (gpio->got_subcommand("read"))
							{
								DigitalValue value = Hardware->DigitalRead(number);
								commandResponse += "GPIO " + std::to_string(number) + " was read with state " + convertToString(value) + "\n";
							}
							else if (gpio->got_subcommand("write"))
							{
								auto write = gpio->get_subcommand("write");
								DigitalValue value;
								if (write->got_subcommand("high"))
									value = DigitalValue::High;
								else if (write->got_subcommand("low"))
									value = DigitalValue::Low;
								else if (write->got_subcommand("toggle"))
									value = DigitalValue::Toggle;

								FakeHardware* hardware = dynamic_cast<FakeHardware*>(Hardware);
								hardware->MockInputState(number, value);
								commandResponse += "GPIO " + std::to_string(number) + " was set to state " + convertToString(hardware->DigitalRead(number)) + "\n";
							}
							else if (gpio->got_subcommand("watch"))
							{
								FakeHardware* hardware = dynamic_cast<FakeHardware*>(Hardware);
								hardware->AddListener(this, number);
								commandResponse += "GPIO " + std::to_string(number) + " is now being watched\n";
							}
						}
						else if (exit->parsed())
						{
							std::exit(0);
						}
						else
							commandResponse += "Command Processed Sucessfully!\n";
					}
					catch (const CLI::ParseError& e)
					{
						commandResponse += app.help() + "\n";
					}
					::AppContainer->AddListener(this);
				}

				void OnActivating(int gpio) override
				{
					commandResponse += "GPIO " + std::to_string(gpio) + " is now HIGH\n";
					::AppContainer->AddListener(this);
				}

				void OnInactivating(int gpio) override
				{
					commandResponse += "GPIO " + std::to_string(gpio) + " is now LOW\n";
					::AppContainer->AddListener(this);
				}
			private:
				std::list<std::string> commandHistory;
				int historyCommandIndex;

				CLI::App app;
				bool debug;

				std::string inputCommand;
				std::string commandResponse;

				Thing::Core::LogLevel _minLevel;
				Thing::Core::LogLevel _maxLevel;
				std::string _name;
				
				std::string Now(const char* format = "%c")
				{
					std::time_t t = std::time(0);
					char cstr[128];
					std::strftime(cstr, sizeof(cstr), format, std::localtime(&t));
					return cstr;
				}

				void clearLine()
				{
					printf("%c[2K\r", 27);
				}

				void printTime()
				{
					auto now = std::chrono::system_clock::now();
					auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
					auto epoch = now_ms.time_since_epoch();

					clearLine();
					std::cout << Now("%H:%M:%S.") << epoch.count() << " ";
				}

				void printInput()
				{
					std::cout << "> " << inputCommand;
				}
			};
		}
	}
}