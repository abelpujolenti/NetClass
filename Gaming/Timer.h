#pragma once
#include <mutex>
#include <functional>

static class Timer
{
public:
	typedef std::function<void()> OnTimeElapsed;
	typedef std::function<bool()> OnTimeElapsedWithLoop;

	static void StartTimer(unsigned long timeRequiredMilliseconds, OnTimeElapsed onTimeElapse);
	static void StartLoopTimer(unsigned long timeRequiredMilliseconds, OnTimeElapsedWithLoop onTimeElapse);
};

