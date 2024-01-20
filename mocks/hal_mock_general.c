// hal_mock_general.c
#include "hal_mock_general.h"

// Simulate the initialization status of HAL as a global variable
uint8_t hal_initialized = 0;
// Simulate the passage of time as a global variable
uint32_t hal_current_time = 0;

void HAL_Init(void) {
  // Mock implementation for HAL_Init
  hal_initialized = 1;
}

void HAL_Delay(uint32_t Delay) {
  // Mock implementation for HAL_Delay
  // Simulate the passage of time
  uint32_t startTime = hal_current_time;
  uint32_t elapsedTime = 0;

  while (elapsedTime < Delay) {
    // Adjust the simulated time resolution based on your requirements
    // Here, we simulate 1 millisecond intervals
    elapsedTime = hal_current_time - startTime;
    usleep(1000);  // Simulate 1 millisecond delay
    hal_current_time++;
  }
}