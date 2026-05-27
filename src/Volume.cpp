#include "../include/Volume.h"
#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;

VolumeController::VolumeController(BacklightTimer* bt)
	: bt(bt)
{
// events 1 and 2 (1 = vol dwn) (2 = vol up and sos)	
	int fd1 = open("/dev/input/event1", O_RDONLY | O_NONBLOCK);
	if (fd1 < 0)
	{
		std::cerr << "FAILED TO INITIALIZE EVENT1 (VOL DWN)" << std::endl;
		std::exit(-1);
	}

	libevdev_new_from_fd(fd1, &this->evdev1);
	ioctl(fd1, EVIOCGRAB, 1);

	int fd2 = open("/dev/input/event2", O_RDONLY | O_NONBLOCK);
	if (fd2 < 0)
	{
		std::cerr << "FAILED TO INITIALIZE EVENT2 (VOL DWN, SOS, SW_LID)" << std::endl;
		std::exit(-1);
	}

	libevdev_new_from_fd(fd2, &this->evdev2);
	ioctl(fd2, EVIOCGRAB, 1);
}

input_event VolumeController::Tick()
{
	input_event ev;

	int rc = libevdev_next_event(
			evdev1,
			LIBEVDEV_READ_FLAG_NORMAL,
			&ev
	);

	if (rc >= 0 && ev.type == EV_KEY)
	{
		if (!this->mMouseMode && (ev.value == 1 || ev.value == 2))
		{
			system("amixer set Master 5%-");
		}

		return ev;
	}

	rc = libevdev_next_event(
			evdev2,
			LIBEVDEV_READ_FLAG_NORMAL,
			&ev
	);

	static auto heldSince = std::chrono::steady_clock::time_point();
	static bool poll = false;
	if (rc == LIBEVDEV_READ_STATUS_SUCCESS)
	{
		if (ev.type == EV_KEY && !this->mMouseMode)
		{
			if (ev.code == KEY_VOLUMEUP && ev.value != 0) system("amixer set Master 5%+");
			if (ev.code == KEY_SOS)
			{
				if (ev.value == 1)
				{
					heldSince = std::chrono::steady_clock::now();
					poll = true;
				}
				else if (ev.value == 0)
				{
					heldSince = std::chrono::steady_clock::time_point();
					poll = false;
				}
			}
		}

		if (ev.type == EV_SW && ev.code == SW_LID)
		{
			if (ev.value == 1)
			{
				bt->ForceOff();
				system("DISPLAY=:0 xset dpms force off");
			}
			else if (ev.value == 0)
			{
				bt->ForceOn();
				bt->Unforce();
				system("DISPLAY=:0 xset dpms force on");
			}
		}

		return ev;
	}
	if (poll)
	{
		if (heldSince == std::chrono::steady_clock::time_point())
		{
			poll = false;
			return ev;
		}

		auto now = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - heldSince).count() >= 500)
		{
			static bool flashPower = false;
			flashPower = !flashPower;
			std::string cmd = "echo " + std::to_string(flashPower) + " > /sys/class/leds/white:flash/brightness";
			system(cmd.c_str());
			heldSince = std::chrono::steady_clock::time_point();
			poll = false;
		}
	}

	return {};
}
