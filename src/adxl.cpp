#include "adxl.h"

#include <wiringPiI2C.h>
#include <unistd.h>
#include <stdio.h>

// the port address for the chip
#define ADDRESS 0x1d

// PowerControl register address
#define PC 0x2d
// Value for PowerControl to measure acceleration
#define PC_MEASURE 0x08

// Format register address
#define FORMAT 0x31
// Mask that includes only the relevant bits
#define FORMAT_MASK 0x0f
// Format value that sets +- 16 g, full res, right aligned 
#define RANGE_FORMAT 0b1011;

// FIFO register address
#define FIFO 0x38

// Measurement register beginning address
#define DATA 0x32

// Scale to normalize
#define SCALE .0039F

// Get the value from the given address
int get(int fd, int address) {
  return (int) wiringPiI2CReadReg16(fd, address);
}

int adxlSetup() {
  // Call the WiringPi I2C setup function
  int fd = wiringPiI2CSetup(ADDRESS);

  // Power Control, set to start measuring data
  wiringPiI2CWriteReg8(fd, PC, PC_MEASURE);

  // build the format value for the chip (+- 16g, full resolution)
  int format = wiringPiI2CReadReg8(fd, FORMAT);
  format &= ~FORMAT_MASK;
  format |= RANGE_FORMAT;

  // Write the format
  wiringPiI2CWriteReg8(fd, FORMAT, format);

  // Disable fifo, so no queuing of data, drop data that is not read
  wiringPiI2CWriteReg8(fd, FIFO, 0);

  // return handle to be used by other functions
  return fd;
}

void getAxes(int fd, float* data) {
  // For all three axis
  for (int i = 0; i < 3; i++) {
    // get the 2 bytes from the registers on the chip
    // 0x33 0x32 X data
    // 0x35 0x33 Y data
    // 0x37 0x36 Z data
    int val = get(fd, DATA + (i * 2));

    // Each value is only 13 bits right aligned, the rest of the bits
    // are sign extended. To convert to a value we can use, we mask the least
    // significant 13 bits; however, we must also be aware of sign. If the 
    // value is negative, we must make val the twos complement of the masked
    // value.
    if (val >> 13 != 0) {
      // value should be negative, mask and then take the twos complement
      val = -1 * ((0b1111111111111 & ~val) + 1);
    } else {
      // value is positive, just mask
      val = 0b1111111111111 & val;
    }
    // Rescale and normalize the data
    data[i] = val * SCALE;
  }
}
