#include "ledCube.h"

#include "animation.h"
#include "liquidAnimation.h"
#include "waveAnimation.h"
#include "clearAnimation.h"

#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <wiringPi.h>

// If there is one argument that is "wave", "liquid", or "clear", sets to
// that animation. If none are specified or an unrecognized argument is given,
// defaults to liquid.
//
// "wave" =>
//    Displays a wave effect
//
// "liquid" =>
//    Makes use of the attached ADXL345 accelerometer to display an effect
//    where lights shift around based on the orientation or tilt of the cube.
//    This gives it a liquid effect like water in a cup.
//
// "clear" =>
//    All leds are set to off and exits immediately
int main(int argc, char** argv) {
  // initialize parameters
  bool* buffer = new bool[64];
  int count = 0;
  int max = 1;
  Animation* animation = NULL;

  // Check to see if an animation is specified
  if (argc == 2) {
    if (strcmp(argv[1], "wave") == 0) {
      // 10 led cube frames for each animation frame
      // => 10 animation frames / second
      max = 10;
      animation = new WaveAnimation();
    } else if (strcmp(argv[1], "liquid") == 0) {
      animation = new LiquidAnimation();
    } else if (strcmp(argv[1], "clear") == 0) {
      animation = new ClearAnimation();
    }
  }

  // if none are specified, default to liquid
  if (animation == NULL) {
    animation = new LiquidAnimation();
  }

  // setup the IO ports
  ledCubeIOSetup();

  // Animate initially
  animation->animate(buffer);

  // If we want to exit immediately, do so
  if (strcmp(argv[1], "clear") == 0) {
    ledCubeDisplay(buffer);
    return 0;
  }

  // Loop forever
  while (true) {
    // Wait some number of LedCube frames (100fps)
    // before updating the animation
    if (++count == max) {
      count = 0;
      animation->animate(buffer);
    }

    // update the led cube
    ledCubeDisplay(buffer);
  }

  return 0;
}
