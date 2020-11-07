#pragma once

void Cleanup();

void AwaitExit();

BYTE GetPreferedBrightness();

int GetCurrentHour();

void SetBrightness(BYTE brightness);

void Init();

tsc::TaskScheduler* SetupSchedule();

void UpdateBrightness();

void MainLoop(tsc::TaskScheduler* scheduler);

BOOL WINAPI ConsoleHandler(DWORD signal);

void ExitHandler();
