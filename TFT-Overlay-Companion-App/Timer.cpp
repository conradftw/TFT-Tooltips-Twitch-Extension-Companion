#include <Windows.h>
#include "Timer.h"

Timer::Timer(double limit) {
	Timer::limit = limit;
}

void Timer::Start() {
	QueryPerformanceFrequency(&qpc[0]);
	QueryPerformanceCounter(&qpc[1]);
}

double Timer::GetDiff() {
	QueryPerformanceCounter(&qpc[2]);
	LONGLONG diff = qpc[2].QuadPart - qpc[1].QuadPart;
	double duration = (double)diff * limit / (double)qpc[0].QuadPart;
	return duration;
}


