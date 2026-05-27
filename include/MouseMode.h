#ifndef MOUSE_MODE_H
#define MOUSE_MODE_H
#include <libevdev/libevdev.h>
#include <map>
#include <string>
#include <iostream>

extern std::map<int, std::string> xKeys;

namespace MouseMode
{
	void MouseInput(input_event ev, input_event volKeys);
	extern bool exitFlag;
}

#endif
