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

	static constexpr unsigned char mKillFlag = (1u << 7); // 10000000
	static constexpr unsigned char mPowerFlag = (1u << 0); // 00000001

	std::atomic<unsigned char> mFlags = 0;

	std::atomic<std::chrono::steady_clock::time_point> mStartTime = std::chrono::steady_clock::now();

	std::thread* mTimer;
};

#endif
