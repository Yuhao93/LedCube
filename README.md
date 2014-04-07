LedCube
=======

Code and explanation for a 4x4x4 Led Cube controlled by a Raspberry Pi.
An ADXL345 Accelerometer chip is also hooked up to the Raspberry Pi for certain animations.

This repository contains the code used to animate an LED cube via the Raspberry Pi.

Software
========
The code used to run the LED Cube is written in C++ and uses the WiringPi and its add-on the WiringPiI2C libraries.
The LED Cube is programmed to update its pattern every 10 milliseconds (100fps).



Hardware
========
The Raspberry Pi connects to 2 hardware components. To connect to them, the Raspberry Pi has 17 GPIO pins that we can use:
![Raspberry Pi GPIO](http://elinux.org/images/2/2a/GPIOs.png)

Accelerometer
-------------
The Accelerometer chip that is used is the [ADXL345](http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf) chip. Using I2C, we connect 4 pins, (5V, SDA, SCL, and 0V) to the Raspberry Pi.


LED Cube
--------
If we use two pins (SDA and SCL) for our Accelerometer chip, this leaves us with 15 pins to use. Our 4x4x4 LED Cube has
64 LED's, meaning we don't have nearly enough IO pins to control all of them. To handle this, we will use a technique called multiplexing or persistence of vision. The LED cube will contain 4 layers, each with 16 LED's. The GND pin for all the LED's on a layer are connected, while the VCC pin for each LED in that layer is independent but is connected to the LED's above and below it. Therefore, to display a single LED, we must connect that column (1 of 16) and that layer (1 of 4). If we go through each of the four layers, showing only the pattern for one layer at a time at a fast enough speed, it will look as if all 4 layers are shown at once. To determine which layer is shown at a given time, two bits are used to control a 2-line to 4-ine decoder.

However, this method still requires 16 + 2 pins, which is still too many. Instead, what we can do is use two 8-bit D latches. Each D-latch will connect to half of the LED's while the 8 bit input for both D-latches are the same. We can control which pattern one D-latch displays by toggling the write enable bit for that D-latch.

This means that the Raspberry Pi will only use 8(data) + 2(D-latch write enable 1 and 2) + 2(2-line to 4-line decoder) pins to control the LED cube, the rest is handled via code.

The schematic for the LED cube at a high level looks like this:

![Hardware Diagram](https://raw.githubusercontent.com/Yuhao93/LedCube/master/diagram.png)
