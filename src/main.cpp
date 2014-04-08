#include "ledCube.h"

#include "animation.h"
#include "liquidAnimation.h"
#include "waveAnimation.h"
#include "clearAnimation.h"

#include <cstring>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <wiringPi.h>

// Mutex to control access to buffer
std::mutex bufferMutex;
// Run flag to control execution of the flags
bool run = true;
// Temporary buffer that data is copied into for
// displaying the LedCube
bool* tBuffer = new bool[64];
// Buffer that animation writes into and display copies from
bool* buffer = new bool[64];

// Function that updates the LedCube
void display_function(bool* buffer) {
  while (run) {
    bufferMutex.lock();
    for (int i = 0; i < 64; i++) {
      tBuffer[i] = buffer[i]; 
    }
    bufferMutex.unlock();
    ledCubeDisplay(tBuffer);
  }
}

// Function that updates the animation
void animation_function(Animation* animation, int waitTime) {
  while (run) {
    bufferMutex.lock();
    animation->animate(buffer);
    bufferMutex.unlock();
    usleep(waitTime);
  }
}


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
  // Buffer must be cleared
  for (int i = 0; i < 64; i++) {
    buffer[i] = false;
  }
 
  // Initialize parameters
  int step = 1;
  Animation* animation = NULL;

  // Check to see if an animation is specified
  if (argc == 2) {
    if (strcmp(argv[1], "wave") == 0) {
      // 10 led cube frames for each animation frame
      // => 10 animation frames / second
      step = 10;
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

  // Exit immediately if just clearing the led cube
  if (argc == 2 && strcmp(argv[1], "clear") == 0) {
    animation->animate(buffer);
    ledCubeDisplay(buffer);
    return 0;
  }

  // Start our separate display and animation threads
  std::thread displayThread(display_function, buffer);
  std::thread animationThread(animation_function, animation,
      4 * TIMESTEP * step);

  // Read in commands until exit is typed
  std::string command = "";
  std::string exit = "exit";
  while (command.compare(exit) != 0) {
    printf("Type exit to exit: ");
    getline(std::cin, command);
  }

  // Signal threads to stop
  run = false;
  displayThread.join();
  animationThread.join();

  delete animation;

  // Clear cube
  animation = new ClearAnimation();
  animation->animate(buffer);
  ledCubeDisplay(buffer);
  return 0;
}
