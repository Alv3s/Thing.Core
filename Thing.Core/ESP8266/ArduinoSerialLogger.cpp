#ifdef ESP8266
#include "ArduinoSerialLogger.h"
#include "../LogLevel.h"
#include <Arduino.h>

ArduinoSerialLogger::ArduinoSerialLogger(std::string name, int baudRate)
{
	this->_name = name;

	// By default min level is Trace and max level is Fatal
	SetMinLevel(Thing::Core::LogLevel::Trace);
	SetMaxLevel(Thing::Core::LogLevel::Fatal); 

	Begin(baudRate);
}

ArduinoSerialLogger::~ArduinoSerialLogger()
{
}

void ArduinoSerialLogger::SetMinLevel(Thing::Core::LogLevel minLevel)
{
	this->_minLevel = minLevel;
}

Thing::Core::LogLevel ArduinoSerialLogger::GetMinLevel()
{
	return this->_minLevel;
}

void ArduinoSerialLogger::SetMaxLevel(Thing::Core::LogLevel maxLevel)
{
	this->_maxLevel = maxLevel;
}

Thing::Core::LogLevel ArduinoSerialLogger::GetMaxLevel()
{
	return this->_maxLevel;
}

std::string ArduinoSerialLogger::GetLoggerName()
{
	return this->_name;
}

void ArduinoSerialLogger::OnTrace(std::string message)
{
	Serial.print("[TRACE]");
	Println(message);
}

void ArduinoSerialLogger::OnDebug(std::string message)
{
	Serial.print("[DEBUG]");
	Println(message);
}

void ArduinoSerialLogger::OnInfo(std::string message)
{
	Serial.print("[INFO]");
	Println(message);
}

void ArduinoSerialLogger::OnWarn(std::string message)
{
	Serial.print("[WARN]");
	Println(message);
}

void ArduinoSerialLogger::OnError(std::string message)
{
	Serial.print("[ERROR]");
	Println(message);
}

void ArduinoSerialLogger::OnFatal(std::string message)
{
	Serial.print("[FATAL]");
	Println(message);
}

void ArduinoSerialLogger::Begin(int baudRate)
{
	Serial.begin(baudRate);
}

void ArduinoSerialLogger::Println(std::string text)
{
	Serial.println(text.c_str());
}
#endif