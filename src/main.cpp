// When this project started only God and I knew what was happening...
// Now only God knows

#include <iostream>
#include <map>
#include <chrono>
#include <cstdlib>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <libevdev/libevdev.h>
#include "../include/MouseMode.h"
#include "../include/Volume.h"
#include "../include/Backlight.h"

using namespace std;
using namespace std::chrono;

map<int, string> keys = {
	{KEY_1, "1`:\"!?/\\.,+-=_@$%^&;'<>(){}[]"},
	{KEY_2, "abc2"},
	{KEY_3, "def3"},
	{KEY_4, "ghi4"},
	{KEY_5, "jkl5"},
	{KEY_6, "mno6"},
	{KEY_7, "pqrs7"},
	{KEY_8, "tuv8"},
	{KEY_9, "wxyz9"},
	{KEY_NUMERIC_STAR, "*"},
	{KEY_0, " 0"},
	{KEY_NUMERIC_POUND, "#"},
};

map<int, string> keysCap = {
	{KEY_1, "1`:\"!?/\\.,+-=_@$%^&;'<>(){}[]"},
	{KEY_2, "ABC2"},
	{KEY_3, "DEF3"},
	{KEY_4, "GHI4"},
	{KEY_5, "JKL5"},
	{KEY_6, "MNO6"},
	{KEY_7, "PQRS7"},
	{KEY_8, "TUV8"},
	{KEY_9, "WXYZ9"},
	{KEY_NUMERIC_STAR, "*"},
	{KEY_0, " 0"},
	{KEY_NUMERIC_POUND, "#"},
};

int main() {
	const char* dev = "/dev/input/event3";

	int fd = open(dev, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		cerr << "Failed to open input device\n";
		return -1;
	}

	int fd_pwr = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
	if (fd_pwr < 0) {
		cerr << "Failed to open input device (event0, power btn)\n";
		return -1;
	}

	libevdev* evdev = nullptr;
	libevdev_new_from_fd(fd, &evdev);
	ioctl(fd, EVIOCGRAB, 1);

	libevdev* evdev_pwr = nullptr;
	libevdev_new_from_fd(fd_pwr, &evdev_pwr);
	ioctl(fd_pwr, EVIOCGRAB, 1);

	int last_key = -1;
	int index = 0;
	bool replacing = false;
	bool grabbed = true;
	bool mouseMode = false;
	bool capsLock = false;

	auto last_press = steady_clock::now();

	BacklightTimer backlightTimer;
	VolumeController volCon(&backlightTimer);

	while (true) {
		if (!mouseMode) volCon.Tick();

		input_event ev;

		int rc = libevdev_next_event(
				evdev,
				LIBEVDEV_READ_FLAG_NORMAL,
				&ev
			);

		if (rc == LIBEVDEV_READ_STATUS_SUCCESS && ev.type == EV_KEY)
		{
			backlightTimer.Poke();

			auto now = steady_clock::now();

			if (keys.count(ev.code) && grabbed && ev.value == 1) {
				if (ev.code == last_key && duration_cast<milliseconds>(now - last_press).count() < 1000) {
					index++;
					replacing = true;
				} else {
					index = 0;
				}

				string chars;
				if (capsLock)
				{
					chars = keysCap[ev.code];
				}
				else
				{
					chars = keys[ev.code];
				}

				char out = chars[index % chars.size()];

				string s(1, out);

				if (out == '\"') s = "\\\"";
				if (out == '\\') s = "\\\\";
				if (out == '$') s = "\\$";
				if (out == '`') s = "\\`";

				if (replacing)
				{
					system("DISPLAY=:0 xdotool key BackSpace");
					replacing = false;
				}

				string cmd = "DISPLAY=:0 xdotool type -- \"" + s + "\"";
				system(cmd.c_str());

				last_key = ev.code;
				last_press = now;
			} else if (xKeys.count(ev.code) && !mouseMode) {
				if (ev.code == KEY_PICKUP_PHONE && ev.value == 1)
				{
					system("DISPLAY=:0 dmenu_run &");
					continue;
				} else if (ev.code == KEY_KBD_LCD_MENU2 && ev.value == 1) {
					system("DISPLAY=:0 ratpoison -c next");
					continue;
				} else if (ev.code == KEY_KBD_LCD_MENU1 && ev.value == 1) {
					capsLock = !capsLock;
					continue;
				} else if (ev.code == KEY_MENU) {
					static auto timeHeld = steady_clock::time_point();
					if (ev.value == 1)
					{
						timeHeld = steady_clock::now();
					} else if (ev.value == 2) {
						if (timeHeld == steady_clock::time_point()) continue;
						auto now = steady_clock::now();
						if (duration_cast<milliseconds>(now - timeHeld).count() >= 500)
						{
							mouseMode = true;
							volCon.ToggleMouseMode();
							timeHeld = steady_clock::time_point();
						}
					} else if (ev.value == 0) {
						timeHeld = steady_clock::time_point();
						if (grabbed && !mouseMode) { ioctl(fd, EVIOCGRAB, 0); grabbed = false; } else if (!grabbed && !mouseMode) { ioctl(fd, EVIOCGRAB, 1); grabbed = true; }
					}
				}

				if (ev.value == 1 || ev.value == 2) {
					string cmd = "DISPLAY=:0 xdotool key " + xKeys[ev.code];
					system(cmd.c_str());
				}
			} 
		}

		if (MouseMode::exitFlag)
		{
			mouseMode = false;
			volCon.ToggleMouseMode();
			MouseMode::exitFlag = false;
		}

		if (mouseMode)
		{
			MouseMode::MouseInput(ev, volCon.Tick());
		}

		rc = libevdev_next_event(
				evdev_pwr,
				LIBEVDEV_READ_FLAG_NORMAL,
				&ev
			);

		if (rc == LIBEVDEV_READ_STATUS_SUCCESS && ev.type == EV_KEY && ev.value == 1) {
			backlightTimer.Poke();

			if (ev.code == KEY_HANGUP_PHONE)
			{
				system("DISPLAY=:0 ratpoison -c delete");
			}
		}

		usleep(10000);
	}
}
