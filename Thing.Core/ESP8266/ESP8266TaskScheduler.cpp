#ifdef ESP8266
#include "ESP8266TaskScheduler.h"

using namespace Thing::Core;

void ESP8266TaskScheduler::taskDelegate(ScheduledTask* task)
{
	task->runnable->Run();
}

ESP8266TaskScheduler::ScheduledTask* ESP8266TaskScheduler::createScheduledTask(IRunnable* runnable)
{
	ScheduledTask* task = new ScheduledTask;
	task->runnable = runnable;
	task->task = new Ticker;
	periodicTasks.insert(std::pair<IRunnable*, ScheduledTask*>(runnable, task));
	return task;
}

void ESP8266TaskScheduler::deleteScheduledTask(ScheduledTask* task)
{
	task->task->detach();
	delete task->task;
	delete task;
}



ESP8266TaskScheduler::ESP8266TaskScheduler()
{
}

ESP8266TaskScheduler::~ESP8266TaskScheduler()
{
	for (std::map<IRunnable*, ScheduledTask*>::iterator it = periodicTasks.begin(); it != periodicTasks.end();)
	{
		deleteScheduledTask(it->second);
		it = periodicTasks.erase(it);
	}
}



void ESP8266TaskScheduler::AttachOnce(unsigned long milli, IRunnable* runnable)
{
	Detach(runnable);

	ScheduledTask* scheduled = createScheduledTask(runnable);
	scheduled->task->attach(milli, ESP8266TaskScheduler::taskDelegate, scheduled);
}

void ESP8266TaskScheduler::AttachOnce(unsigned long milli, IRunnable& runnable)
{
	AttachPeriodic(milli, &runnable);
}



void ESP8266TaskScheduler::AttachPeriodic(unsigned long milli, IRunnable* runnable)
{
	Detach(runnable);

	ScheduledTask* scheduled = createScheduledTask(runnable);
	scheduled->task->attach_ms(milli, ESP8266TaskScheduler::taskDelegate, scheduled);
}

void ESP8266TaskScheduler::AttachPeriodic(unsigned long milli, IRunnable& runnable)
{
	AttachPeriodic(milli, &runnable);
}

void ESP8266TaskScheduler::Detach(IRunnable* runnable)
{
	std::map<IRunnable*, ScheduledTask*>::iterator it = periodicTasks.find(runnable);
	if (it != periodicTasks.end())
		deleteScheduledTask(it->second);
}

void ESP8266TaskScheduler::Detach(IRunnable& runnable)
{
	Detach(&runnable);
}
#endif