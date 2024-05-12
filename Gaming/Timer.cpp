#include "Timer.h"
#include <thread>
#include <windows.h>

void Timer::StartTimer(unsigned long timeRequiredMilliseconds, OnTimeElapsed onTimeElapse)
{
	const std::unique_ptr<std::thread> thread = std::make_unique<std::thread>([timeRequiredMilliseconds,onTimeElapse]() {

		Sleep(timeRequiredMilliseconds);
		onTimeElapse();
	});

	thread->detach();
}

void Timer::StartLoopTimer(unsigned long timeRequiredMilliseconds, OnTimeElapsedWithLoop onTimeElapse)
{
	const std::unique_ptr<std::thread> thread = std::make_unique<std::thread>([timeRequiredMilliseconds, onTimeElapse]() {

		bool continueLoop { true};
		while (continueLoop)
		{
			Sleep(timeRequiredMilliseconds);
			continueLoop = onTimeElapse();
		}
		
	});

	thread->detach();
}
