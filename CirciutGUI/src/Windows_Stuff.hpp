#pragma once

// All Windows APIs used in Project

std::string OpenFileDialog(const char* filter, int tempNumber = 0);

std::string SaveFileDialog(const char* filter, int tempNumber = 0);

int GetSystemVolumn();

tm* GetSystemDateAndTime();

void wintest();