#ifndef ANIMATION_H
#define ANIMATION_H

// Base class for an animation
class Animation {
public:
  // The animation frame should progress each time animate is called
  virtual void animate(bool* output) = 0;

  // Reset should reset the animation to its original state if applicable,
  // if an animation is sequential, i.e. is a function of time, then it should
  // use cnt to keep track of time.
  virtual void reset() {
    cnt = 0; 
  }
protected:
  int cnt;
};

#endif
