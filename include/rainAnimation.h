#ifndef RAIN_ANIMATION_H
#define RAIN_ANIMATION_H

#include "animation.h"
#include <random>
#include <set>

class RainAnimation : public Animation {
public:
  // Initialize our random generator
  RainAnimation() : random(0.0, 1.0) { }

  virtual void animate(bool* output) {
    if (cnt++ < 10) {
      return;
    }
    cnt = 0;
   
    // Pick a point and initialize at the top (y = 3)
    int randLayer = (int) (random(generator) * 16);
    int x = randLayer / 4;
    int z = randLayer % 4;
    points.insert(48 + x * 4 + z);

    // Set on and off
    for (int i = 0; i < 64; i++) {
      if (points.find(i) != points.end()) {
        output[i] = true;
      } else {
        output[i] = false;
      }
    }

    // Update all points, have them fall down one pixel. Those that fall
    // offscreen are removed.
    tempPoints.clear();
    for (auto it = points.begin(); it != points.end(); it++) {
      int point = *it;
      int y = point / 16;
      if (y - 1 >= 0) {
        tempPoints.insert(((y - 1) * 16) + point % 16);
      }
    }
    points = tempPoints;
  }

private:
  // Set of points
  std::set<int> points;

  // Temporary set of points used when updating
  std::set<int> tempPoints;

  // Random generator used to shuffle the indexes
  std::default_random_engine generator;
  std::uniform_real_distribution<double> random;
};

#endif
