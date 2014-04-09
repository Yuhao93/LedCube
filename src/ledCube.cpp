#include "ledCube.h"
#include <wiringPi.h>
#include <unistd.h> 

// ON and OFF values
#define ON true
#define OFF false

// How many D latches there are
#define MEM_SELECT_SIZE 2
// Indexes of the D latches
#define MEM_0 0
#define MEM_1 1

// How many pins are used to select the current layer to display
#define LAYER_SELECT_SIZE 2
// The number of layers
#define LAYER_SIZE 4

// How much data to write at once (bits)
#define DATA_SIZE 8

// Initialize the data buffer
bool subData[8] { false, false, false, false, false, false, false, false };

// IO pin numbers used
int memSelectPins[2] { 12, 3 };
int layerSelectPins[2] { 14, 13 };
int dataPins[8] { 15, 16, 1, 4, 5, 6, 10, 11 };

// Get the 8 bit data chunk to write 
void getSubData(bool* data, int ind, bool* dest) {
  int offset = ind * 8;
  for (int i = 0; i < 8; i++) {
    dest[i] = data[offset + i];
  }
}

// Set write enable for a D-latch
void setMem(int ind, bool write) {
  digitalWrite(memSelectPins[ind], write ? HIGH : LOW);
}

// Set the layer to display (convert number 0-3 into two bits) 
void setLayer(int layer) {
  digitalWrite(layerSelectPins[0], (layer & 1) == 1 ? HIGH : LOW);
  digitalWrite(layerSelectPins[1], ((layer >> 1) & 1) == 1 ? HIGH : LOW);
}

// Write the 8 bit data into the 8 output pins
void writeData(bool* data) {
  for (int i = 0; i < DATA_SIZE; i++) {
    digitalWrite(dataPins[i], data[i] ? HIGH : LOW);
  }
}

// Setup the IO pins
void ledCubeIOSetup() {
  // Call wiringPi setup
  wiringPiSetup();
  
  // Set all pins used to output mode
  for (int i = 0; i < MEM_SELECT_SIZE; i++) {
    pinMode(memSelectPins[i], OUTPUT);
  }
  
  for (int i = 0; i < LAYER_SELECT_SIZE; i++) {
    pinMode(layerSelectPins[i], OUTPUT);
  }

  for (int i = 0; i < DATA_SIZE; i++) {
    pinMode(dataPins[i], OUTPUT);
  }
}

void ledCubeDisplay(bool* data) {
  // Initially, disable writing for both d-latches
  setMem(MEM_0, OFF);
  setMem(MEM_1, OFF);

  // For each layer
  for (int i = 0; i < LAYER_SIZE; i++) {
    // Set to the current layer
    setLayer(i);
   
    // Prepare the first 8 bit chunk to write and write
    getSubData(data, i * 2, subData);
    writeData(subData);

    // Write into D-latch 0, then disable write for D-latch 0
    setMem(MEM_0, ON);
    setMem(MEM_0, OFF);

    // Prepare the second 8 bit chunk to write and write
    getSubData(data, i * 2 + 1, subData);
    writeData(subData);

    // Write into D-latch 1, then disable write for D-latch 1
    setMem(MEM_1, ON);
    setMem(MEM_1, OFF);
    
    // Wait for 1/4 the total time of one frame
    usleep(TIMESTEP);
  }
}

