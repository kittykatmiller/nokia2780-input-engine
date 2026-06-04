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
			mFlags |= mKillFlag; // Raise kill flag (7)
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
	mFlags |= mPowerFlag; // Raise power flag
}

void BacklightTimer::Timer()
{
	while (!(mFlags & mKillFlag))
	{
		if ((mFlags & mPowerFlag) && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - mStartTime.load()).count() >= 30000)
		{
			system("echo 0 > /sys/class/leds/white:kbd_backlight/brightness");
			mFlags &= static_cast<unsigned char>(~mPowerFlag); // Lower power flag (0)
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	mFlags &= static_cast<unsigned char>(~mKillFlag); // Lower kill flag (7)
}

void BacklightTimer::ForceOn()
{
	if (mTimer && mTimer->joinable())
	{
		mFlags |= mKillFlag; // Raise kill flag
		mTimer->join();
		delete mTimer;
		mTimer = nullptr;
	}

	Poke();
}

void BacklightTimer::ForceOff()
{
	if (mTimer && mTimer->joinable())
	{
		mFlags |= mKillFlag; // Raise kill flag
		mTimer->join();
		delete mTimer;
		mTimer = nullptr;
	}

	system("echo 0 > /sys/class/leds/white:kbd_backlight/brightness");
	mFlags &= static_cast<unsigned char>(~mPowerFlag); // Lower power flag
}

void BacklightTimer::Unforce()
{
	if (mTimer && mTimer->joinable()) return;

	mTimer = new std::thread(&BacklightTimer::Timer, this);
}
