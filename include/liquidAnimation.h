#ifndef LIQUID_ANIMATION_H
#define LIQUID_ANIMATION_H

#include "animation.h"
#include "adxl.h"
#include <algorithm>
#include <utility>
#include <vector>

// Comapre two given tuples of data based on the score (float)
bool compare(std::pair<int, float> p1, std::pair<int, float> p2) {
  return p1.second < p2.second;
}

// Reads the data from the accelerometer to determine the tilt of the cube
// and only light up the "bottom" most leds so that the light appears to be
// affected by gravity.
class LiquidAnimation : public Animation {
public:
  // Initialize the ADXL345 chip
  LiquidAnimation() : fd(adxlSetup()) { }

  virtual void animate(bool* data) {
    // Read data from accelerometer, we can consider the vector as opposite
    // the direction of gravity
    float* arr = new float[3];
    getAxes(fd, arr);     

    // Create a buffer of tuples to write data into, to be sorted later
    std::pair<int, float>* buffer = new std::pair<int, float>[64];
    
    // For each led
    for (int x = 0; x < 4; x++) {
      for (int y = 0; y < 4; y++) {
        for (int z = 0; z < 4; z++) {
          // Calculate the index and initially assume that the light is off
          int ind = y * 16 + x * 4 + z;
          data[ind] = false;

          // Consider this point as a vector and project it (dot product) onto
          // our gravity vector. This results in a scalar value that determines
          // where along our gravity direction this point lies.
          // Larger values means it's "higher up" and lower values means it's
          // "farther down" Store this information in a tuple to be sorted
          // later
          float factor = -y * arr[0] + z * arr[1] + -x * arr[2];
          buffer[ind] = std::pair<int, float>(ind, factor);
        }
      }
    }
  
    // Copy into a vector to be sorted
    std::vector<std::pair<int, float>> vect(buffer, buffer + 64);
    // Sort based on the score of each point (smallest first)
    std::sort(vect.begin(), vect.end(), compare);

    // Get the n lowest points to display, note, n can change to any number,
    // but for the animation to look good while sitting flat, n should be
    // either 16, 32, or 48.
    auto it = vect.begin();
    for (int i = 0; i < 32; i++) {
      data[it->first] = true;
      it++;
    }

    delete[] arr;
    delete[] buffer;
  }

private:
  int fd;
};

#endif
