# ArduboyRayCaster
A toy implementation of a monochrome raycast 3D-renderer, on the Arduino-based [Arduboy](https://arduboy.com/). 

![A demonstration GIF of the raycaster running on the Arduboy.](https://github.com/ulfben/ArduboyRayCaster/raw/master/ArduboyRayCaster.gif)

This device features a 128x64px black-and-white display, 2.5KB of RAM and 32KB of static Flash storage (most of which I use to store the look-up tables). The CPU is a fairly beefy ATmega32u4 running at up to 16mhz, but doesn't provide an FPU nor even a barrel shifter. Ergo: division and multiplication, even by powers-of-2, is glacial.

If you want to build- and run this on Windows I have [an SDL-based version here](https://github.com/ulfben/RayCastDemo), for Visual Studio.
