#ifndef WAVE_ANIMATION_H
#define WAVE_ANIMATION_H

#include "animation.h"
#include <cstdlib>

// Animation that displays a wave tha moves up and down
// 
// Frame 1:   Frame 2:   Frame 3:
// +----+     +----+     +----+
// |   x|     |    |     |    |
// |  x |     |   x|     |x   |
// | x  |     |x x |     | x x|
// |x   |     | x  |     |  x |
// +----+     +----+     +----+
//
// Frame 4:   Frame 5:   Frame 6:
// +----+     +----+     +----+
// |x   |     | x  |     |  x |
// | x  |     |x x |     | x x|
// |  x |     |   x|     |x   |
// |   x|     |    |     |    |
// +----+     +----+     +----+
//

class WaveAnimation : public Animation {
public:
  WaveAnimation() { cnt = 0; }

  virtual void animate(bool* output) {
    // For each led
    for (int x = 0; x < 4; x++) {
      for (int y = 0; y < 4; y++) {
        bool res = false;
        // If this is the correct level to show the wave, set on
        if (abs(((cnt + x) % 6) - 3) == y) {
          res = true;
        }
        // Repeat across z
        for (int z = 0; z < 4; z++) {
          output[y * 16 + x * 4 + z] = res; 
        }
      }
    }
    // increment cnt through the 6 unique animation frames
    if (++cnt == 6) {
      cnt = 0;
    }
  }
};

#endif
