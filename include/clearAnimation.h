#ifndef CLEAR_ANIMATION_H
#define CLEAR_ANIMATION_H

// Animation used to just turn off all the lights
class ClearAnimation : public Animation {
public:
  virtual void animate(bool* data) {
    for (int i = 0; i < 64; i++) {
      data[i] = false;
    }
  }
};

#endif
