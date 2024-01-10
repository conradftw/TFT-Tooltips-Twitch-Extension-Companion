#pragma once
#include <Windows.h>

class Timer {

public: 
	Timer(double);
	void Start();
	double GetDiff();

private:
	LARGE_INTEGER qpc[3];
	double limit;
};