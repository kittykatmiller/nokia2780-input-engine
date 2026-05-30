# NOKIA 2780 Input Engine!

This program enables the NOKIA 2780 flip running any xserver** on (probably*) any linux distro!

\*Currently only tested on the nokia-weeknd, edge channel build of PostMarketOS

\** Lots of features such as closing windows and switching windows rely on ratpoison commands

This program is held together by elmers glue, hopes, and dreams so please don't expect anything professional out of this. Sometimes... it just doesnt work! and I have to reboot... dont know what thats about...
but hey, i turned that stupid "SOS" button that KEEPS LOCKING ME OUT OF VIEWING MESSAGES WHILE IM ON CALL (in KaiOS—i dual booted >:3 ) into a pretty cool flashlight button ^^

## Planned features:
* Haptic feedback when certain controls are enabled
* Stability :p
* Allowing you to click enter to select a letter before timer expires

## Known Bugs:
* Sometimes doesn't launch/work on boot :p
* When coming out of mouse mode, it toggles off input-grabbing, so you have to enable that right after.
* When trying to toggle input-grabbing, you cant just click it fast—for some reason you have to hold it for a short time, but still a press, or else you'll enable mouse mode)

# HOW TO USE:
It boots into a normal t9 mode (click numbers multiple times to select a letter)

OK button = Enter/Return

D-Pad (left, right, up, down) = normal usage as up, down, left, right arrow keys on keyboard

Left softkey button (little dash at top of kbd) - nothing :3

Right softkey button - switch windows (only works with ratpoison)

Hang up button - close window (only work with ratpoison)

Answer button - Open dmenu (...needs dmenu installed)

Hamburger/menu button - 

press: toggles on/off input-grabbing (turns from normal t9 to raw, keybd input for number keys)

hold: toggles mouse mode

Vol Up/Dwn - turns volume up and down by 5% (using amixer)

SOS button - Flashlight :3 (hold for ~.5 sec)

### Mouse Mode Usage

D-pad - move mouse

OK button = Enter/Return (NOT left click)

Left softkey button = right click***

Right softkey button = left click***

Hangup/answer keys - normal behaviour

Vol Up/Dwn - scroll

Menu - Hold to switch back to normal mode (SEE BUGS FOR MORE INFO)

\***I now realise thats kind of backwards but for some reason it feels right?



Thank you for checking out my project !! :3


# Quick Tip for general Nokia 2780 Hacking
* To install this, just download the executable (not sketch i swear) and have it execute in ur .xinitrc or (preferrably, if using ratpoison) .ratpoisonrc
  
* before installing (or maybe it has to be after im not quite sure i only tested this after installing) run
  `pmbootstrap kconfig edit linux-postmarketos-qcom-msm89x7` and include the driver `Stironix ST7789V` found under `Device Drivers -> Graphics Support -> Direct Rendering Manager -> Display Panels -> Sitronix ST7789V panel` This enables the display panel driver and allows graphics to be rendered, instead of just leaving you on a blank screen

* If left at a blank screen (or frozen "powered by KaiOS" screen) run, while connected to a pc by a data cable, `telnet 172.16.42.1` then copy the `pmos_root_uuid` (or smth like that, cant remember off the top of my head) and reinsert sd and run `sudo tune2fs -U <UUID> /dev/<SDCARD>` to change your SD card's UUID to match the boot.img's expected rootfs UUID

Yor'eu welcum :3
