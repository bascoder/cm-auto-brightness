#include <iostream>
#include <ctime>
#include <Windows.h>
#include <synchapi.h>
#include "SDKDLL.h"
#include "TaskScheduler.hpp"
#include "main.h"

using namespace std;
using namespace literals;

const int DAWN = 8;
const int DUSK = 17;
const int BRIGHTNESS_DARK = 50;
const int BRIGHTNESS_BRIGHT = 200;

int main() {
	cout << "Cooler Master controller" << endl << endl;

	try
	{
		Init();
		UpdateBrightness();

		auto scheduler = SetupSchedule();
		MainLoop(scheduler);
	}
	catch (const exception& e)
	{
		cerr << e.what() << endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void Init()
{
	atexit(ExitHandler);
	if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
		cerr << "ERROR: Could not set control handler" << endl;

		exit(EXIT_FAILURE);
	}

	SetControlDevice(DEV_MKeys_L_White);

	if (!EnableLedControl(true)) {
		cerr << "Failed to take over LED control.";
	}
}

tsc::TaskScheduler* SetupSchedule()
{
	static tsc::TaskScheduler* scheduler = new tsc::TaskScheduler();
	scheduler->Schedule(30min, [](tsc::TaskContext context)
	{
		UpdateBrightness();

		context.Repeat();
	});

	return scheduler;
}

void UpdateBrightness()
{
	cout << "Updating brightness" << endl;

	auto brightness = GetPreferedBrightness();
	SetBrightness(brightness);
}

void MainLoop(tsc::TaskScheduler* scheduler)
{
	while (true)
	{
		scheduler->Update();
		Sleep(5000);
	}
}

BYTE GetPreferedBrightness()
{
	int hour = GetCurrentHour();

	if (hour >= DUSK || hour < DAWN) {
		return BRIGHTNESS_DARK;
	}
	else {
		return BRIGHTNESS_BRIGHT;
	}
}

int GetCurrentHour()
{
	time_t ttime = time(0);
	struct tm localTime;
	errno_t err = localtime_s(&localTime, &ttime);

	if (err) {
		cerr << "Could not obtain local time" << endl;
		throw new exception("Failed to obtain localTime", err);
	}

	return localTime.tm_hour;
}

void SetBrightness(BYTE brightness)
{
	if (!SetFullLedColor(brightness, brightness, brightness)) {
		cerr << "Failed to set LED brightness";
	}
}

BOOL WINAPI ConsoleHandler(DWORD signal)
{
	switch (signal)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		cout << "Ctrl-C pressed! Starting gracefull exit..." << endl;
		exit(EXIT_SUCCESS);
		return TRUE;
	default:
		return FALSE;
	}
}

void Cleanup()
{
	if (!EnableLedControl(false)) {
		cerr << "Failed to give back LED control!" << endl;
	}
}

void ExitHandler() {
	cout << "Handling exit" << endl;

	Cleanup();
}
