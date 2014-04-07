#ifndef LED_CUBE_H
#define LED_CUBE_H

// Inteface for displaying a pattern on the 4x4x4 LED cube
// Some things to note:
//   This uses the Raspberry Pi R2 to communicate with the cube. The rPi has 17
//   GPIO pins, 2 of which are used by the accelerometer chip. This leaves us
//   with 15 pins to use for our cube.

//   The LED cube was built for a 4-layer multiplex/persistence-of-vision
//   scheme. Persistence of vision is the concept that if multiple images are 
//   flashed in quick succesion, our brain will join those images together.
//   This is how we perceive motion and animations in general.
//   We don't have the 64 pins necessary to control each led on the cube at 
//   the same time, instead, we will only show one layer (16 leds) at a time.
//   We will show all 4 layers in quick succession. If done fast enough, it
//   will appear as if we have shown all 4 layers at the same time.

//   However, we still don't have enough pins to show 16 leds at a time, 
//   especially since we need at least 2 pins to select which layer to 
//   show. Therefore, as a workaround, we will use two 8-bit D-latches. This 
//   way, we will write half a layer to one D-latch, then the other half layer 
//   to the other D-latch. Although we are only writing half a layer at a time, 
//   because we are writing immediately after each other, it seems as if an 
//   entire layer is being displayed at once.
//
//   So in total, we use 2 pins to control the write enable for each D-latch,
//   8 pins to write to the D-latches (Each D-latch takes 8 bits, but they
//   share the same 8 pins, the pattern of the D-latches can be different from
//   the other because we can enable/disable writing to a D-latch.) Lastly, we
//   use 2 pins that feed into a 2-line to 4-line decoder to control which
//   layer we are displaying.
//   All in all, we are using 2 + 8 + 2 = 12 pins to control our LED cube



// Intialize the IO pins to communicate with the LED cube
void ledCubeIOSetup();

// Draw given sequence of ones and zeros onto the cube
// the mapping of position to index is (y * 16 + x * 4 + z)
void ledCubeDisplay(bool* data);

#endif
