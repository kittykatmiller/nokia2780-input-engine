#include "../include/Backlight.h"
#include <iostream>

BacklightTimer::BacklightTimer()
{
	this->mTimer = new std::thread(&BacklightTimer::Timer, this);
}

BacklightTimer::~BacklightTimer()
{
	if (mTimer)
	{
		if (mTimer->joinable())
		{
			mKillFlag = true;
			mTimer->join();
		}
		delete mTimer;
		mTimer = nullptr;
	}
}

void BacklightTimer::Poke()
{
	system("echo 1 > /sys/class/leds/white:kbd_backlight/brightness");
	mStartTime = std::chrono::steady_clock::now();
	mPowerFlag = true;
}

void BacklightTimer::Timer()
{
	while (!mKillFlag)
	{
		if (mPowerFlag && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - mStartTime.load()).count() >= 30000)
		{
			system("echo 0 > /sys/class/leds/white:kbd_backlight/brightness");
			mPowerFlag = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	mKillFlag = false;
}

void BacklightTimer::ForceOn()
{
	if (mTimer && mTimer->joinable())
	{
		mKillFlag = true;
		mTimer->join();
		delete mTimer;
		mTimer = nullptr;
	}

	system("echo 1 > /sys/class/leds/white:kbd_backlight/brightness");
}

void BacklightTimer::ForceOff()
{
	if (mTimer && mTimer->joinable())
	{
		mKillFlag = true;
		mTimer->join();
		delete mTimer;
		mTimer = nullptr;
	}

	system("echo 0 > /sys/class/leds/white:kbd_backlight/brightness");
}

void BacklightTimer::Unforce()
{
	if (mTimer && mTimer->joinable()) return;

	mTimer = new std::thread(&BacklightTimer::Timer, this);
}
