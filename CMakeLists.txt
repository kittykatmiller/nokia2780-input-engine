cmake_minimum_required(VERSION 3.10)

project(nokia2780_t9_engine)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
	include
	/usr/include/libevdev-1.0
)

add_executable(nokia2780_t9_engine
	src/main.cpp
	src/MouseMode.cpp
	src/Volume.cpp
	src/Backlight.cpp
)

target_link_libraries(nokia2780_t9_engine
	evdev
)
