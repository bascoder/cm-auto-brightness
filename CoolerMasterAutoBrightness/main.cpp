#include <iostream>
#include <cstdio>
#include <csignal>
#include "SDKDLL.h"
#include "main.h"

using namespace std;

void ExitHandler() {
	cout << "Handling exit" << endl;

	Cleanup();
}

int main() {
	cout << "Cooler Master controller" << endl << endl;


	try
	{
		Init();
		SetBrightness(5);

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

void SetBrightness(BYTE brightness)
{
	if (!SetFullLedColor(brightness, brightness, brightness)) {
		cerr << "Failed to set LED brightness";
	}
}

void AwaitExit()
{
	cout << "Press Any Key To Exit..." << endl;
	getchar();
}

void Cleanup()
{
	if (!EnableLedControl(false)) {
		cerr << "Failed to give back LED control!";
	}
}
