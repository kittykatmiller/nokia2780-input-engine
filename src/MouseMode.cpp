#include "../include/MouseMode.h"
#include <iostream>
#include <string>
#include <chrono>

std::map<int, std::string> xKeys = {
	{KEY_UP, "Up"},
	{KEY_DOWN, "Down"},
	{KEY_LEFT, "Left"},
	{KEY_RIGHT, "Right"},
	{KEY_ENTER, "Return"},
	{KEY_BACKSPACE, "BackSpace"},
	{KEY_PICKUP_PHONE, "XF86Phone"},
	{KEY_KBD_LCD_MENU1, "N/A"},
	{KEY_KBD_LCD_MENU2, "N/A"},
	{KEY_MENU, "N/A"},
};

bool MouseMode::exitFlag = false;

void MouseMode::MouseInput(input_event ev, input_event volKeys) {
	if (ev.value == 1 || ev.value == 2) {
		switch (ev.code)
		{
		case KEY_UP: { system("DISPLAY=:0 xdotool mousemove_relative -- 00 -10"); break; }
		case KEY_DOWN: { system("DISPLAY=:0 xdotool mousemove_relative -- 00 10"); break; }
		case KEY_LEFT: { system("DISPLAY=:0 xdotool mousemove_relative -- -10 00"); break; }
		case KEY_RIGHT: { system("DISPLAY=:0 xdotool mousemove_relative -- 10 00"); break; }
		case KEY_ENTER: { system("DISPLAY=:0 xdotool key Return"); break; }
		case KEY_BACKSPACE: { system("DISPLAY=:0 xdotool key BackSpace"); break; }
		case KEY_KBD_LCD_MENU1: { system("DISPLAY=:0 xdotool click 3"); break; }
		case KEY_KBD_LCD_MENU2: { system("DISPLAY=:0 xdotool click 1"); break; }
		case KEY_PICKUP_PHONE: { system("DISPLAY=:0 dmenu_run &"); break; }
		}
	}

	if (ev.code == KEY_MENU)
	{
		static auto heldSince = std::chrono::steady_clock::time_point();
		if (ev.value == 1)
		{
			heldSince = std::chrono::steady_clock::now();
		}
		else if (ev.value == 2)
		{
			if (heldSince == std::chrono::steady_clock::time_point()) return;

			auto now = std::chrono::steady_clock::now();

			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - heldSince).count() >= 500)
			{
				exitFlag = true;
				heldSince = std::chrono::steady_clock::time_point();
			}
		}
		else if (ev.value == 0) heldSince = std::chrono::steady_clock::time_point();
	}

	if (volKeys.value == 1 || volKeys.value == 2)
	{
		switch (volKeys.code)
		{
		case KEY_VOLUMEUP: { system("DISPLAY=:0 xdotool click 4"); break; }
		case KEY_VOLUMEDOWN: { system("DISPLAY=:0 xdotool click 5"); break; }
		}
	}
}
