#pragma once

#include "../IPrinter.h"
#include "../ILoggerListener.h"
#include "../LogLevel.h"
#include <string>
#include <Arduino.h>

namespace Thing
{
	namespace Core
	{
		namespace Arduino
		{
			class SerialLogger : public virtual Thing::Core::ILoggerListener
			{
			private:
				Thing::Core::LogLevel _minLevel;
				Thing::Core::LogLevel _maxLevel;
				std::string _name;

				void Begin(int baudRate)
				{
					Serial.begin(baudRate);
				}

				void Println(std::string text)
				{
					Serial.println(text.c_str());
				}
			public:
				SerialLogger(std::string name, int baudRate) : _name(name)
				{
					// By default min level is Trace and max level is Fatal
					SetMinLevel(Thing::Core::LogLevel::Trace);
					SetMaxLevel(Thing::Core::LogLevel::Fatal);

					Begin(baudRate);
				}

				~SerialLogger()
				{

				}

				std::string GetLoggerName() override
				{
					return this->_name;
				}

				void SetMinLevel(Thing::Core::LogLevel minLevel) override
				{
					this->_minLevel = minLevel;
				}

				Thing::Core::LogLevel GetMinLevel() override
				{
					return this->_minLevel;
				}

				void SetMaxLevel(Thing::Core::LogLevel maxLevel) override
				{
					this->_maxLevel = maxLevel;
				}

				Thing::Core::LogLevel GetMaxLevel() override
				{
					return this->_maxLevel;
				}

				void OnTrace(std::string message) override
				{
					Serial.print("[TRACE]");
					Println(message);
				}

				void OnDebug(std::string message) override
				{
					Serial.print("[DEBUG]");
					Println(message);
				}

				void OnInfo(std::string message) override
				{
					Serial.print("[INFO]");
					Println(message);
				}

				void OnWarn(std::string message) override
				{
					Serial.print("[WARN]");
					Println(message);
				}

				void OnError(std::string message) override
				{
					Serial.print("[ERROR]");
					Println(message);
				}

				void OnFatal(std::string message) override
				{
					Serial.print("[FATAL]");
					Println(message);
				}
			};
		}
	}
}