#pragma once

void Cleanup();

void AwaitExit();

BYTE GetPreferedBrightness();

int GetCurrentHour();

void SetBrightness(BYTE brightness);

void Init();

void ExitHandler();
