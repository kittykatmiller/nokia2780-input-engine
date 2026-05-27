#ifndef BACKLIGHT_H
#define BACKLIGHT_H
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

class BacklightTimer
{
public:
	BacklightTimer();
	~BacklightTimer();
	void ForceOn();
	void ForceOff();
	void Unforce();
	void Poke();
private:
	void Timer();

	std::atomic<bool> mKillFlag = false;
	std::atomic<bool> mPokeFlag = false;
	std::atomic<bool> mPowerFlag = false;

	std::atomic<std::chrono::steady_clock::time_point> mStartTime = std::chrono::steady_clock::now();

	std::thread* mTimer;
};

#endif
