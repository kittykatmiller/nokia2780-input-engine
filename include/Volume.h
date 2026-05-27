#ifndef VOLUME_H
#define VOLUME_H
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include "Backlight.h"

class VolumeController
{
public:
	VolumeController(BacklightTimer* bt);
	input_event Tick();
	void ToggleMouseMode() { mMouseMode = !mMouseMode; }
private:
	libevdev* evdev1 = nullptr;
	libevdev* evdev2 = nullptr;

	bool mMouseMode = false;
	BacklightTimer* bt;
};

#endif
