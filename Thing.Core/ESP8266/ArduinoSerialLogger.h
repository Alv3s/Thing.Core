#pragma once

#include "../IPrinter.h"
#include "../ILoggerListener.h"
#include "../LogLevel.h"
#include <string>

class ArduinoSerialLogger : public virtual Thing::Core::ILoggerListener
{
private:
	Thing::Core::LogLevel _minLevel;
	Thing::Core::LogLevel _maxLevel;
	std::string _name;

	void Begin(int baudRate);

	void Println(std::string text);
public:
	ArduinoSerialLogger(std::string name, int baudRate);
	virtual ~ArduinoSerialLogger();

	virtual std::string GetLoggerName() override;
	virtual void SetMinLevel(Thing::Core::LogLevel minLevel) override;
	virtual Thing::Core::LogLevel GetMinLevel() override;
	virtual void SetMaxLevel(Thing::Core::LogLevel maxLevel) override;
	virtual Thing::Core::LogLevel GetMaxLevel() override;

	virtual void OnTrace(std::string message) override;
	virtual void OnDebug(std::string message) override;
	virtual void OnInfo(std::string message) override;
	virtual void OnWarn(std::string message) override;
	virtual void OnError(std::string message) override;
	virtual void OnFatal(std::string message) override;
};