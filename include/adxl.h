#ifndef ADXL_H
#define ADXL_H

// Interface for communicating with the ADXL345 Accelerometer chip
// See here for more info on the chip:
// http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf

// Initialize the I2C connections
int adxlSetup();

// Get the most recent accelerometer readings (x, y, z)
void getAxes(int fd, float* data);

#endif
