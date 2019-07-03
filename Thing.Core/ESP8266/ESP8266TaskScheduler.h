#pragma once

#include "Ticker.h"
#include "../ITaskScheduler.h"
#include <map>

class ESP8266TaskScheduler : public virtual Thing::Core::ITaskScheduler
{
public:
	ESP8266TaskScheduler();
	~ESP8266TaskScheduler();

	virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable* runnable) override;
	virtual void AttachOnce(unsigned long milli, Thing::Core::IRunnable& runnable) override;

	virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable* runnable) override;
	virtual void AttachPeriodic(unsigned long milli, Thing::Core::IRunnable& runnable) override;

	virtual void Detach(Thing::Core::IRunnable* runnable) override;
	virtual void Detach(Thing::Core::IRunnable& runnable) override;
private:
	struct ScheduledTask
	{
		Thing::Core::IRunnable* runnable;
		Ticker* task;
	};
	static void taskDelegate(ScheduledTask* task);
	ScheduledTask* createScheduledTask(Thing::Core::IRunnable* runnable);
	void deleteScheduledTask(ScheduledTask* task);

	std::map<Thing::Core::IRunnable*, ScheduledTask*> periodicTasks;
};