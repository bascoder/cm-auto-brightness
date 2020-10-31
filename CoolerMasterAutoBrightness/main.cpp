#include <iostream>
#include <ctime>
#include "SDKDLL.h"
#include "main.h"

using namespace std;

const int DAWN = 8;
const int DUSK = 17;
const int BRIGHTNESS_DARK = 50;
const int BRIGHTNESS_BRIGHT = 200;

int main() {
	cout << "Cooler Master controller" << endl << endl;

	try
	{
		Init();

		auto brightness = GetPreferedBrightness();
		SetBrightness(brightness);

		AwaitExit();
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

	SetControlDevice(DEV_MKeys_L_White);

	if (!EnableLedControl(true)) {
		cerr << "Failed to take over LED control.";
	}
}

BYTE GetPreferedBrightness()
{
	int hour = GetCurrentHour();

	if (hour > DUSK && hour < DAWN) {
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

void AwaitExit()
{
	cout << "Press Any Key To Exit..." << endl;
	cin.get();
}

void Cleanup()
{
	if (!EnableLedControl(false)) {
		cerr << "Failed to give back LED control!";
	}
}

void ExitHandler() {
	cout << "Handling exit" << endl;

	Cleanup();
}
