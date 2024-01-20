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
  // Check if the HAL has been left uninitialized
  if(!hal_initialized) {
    // Mock implementation: Handle uninitialized HAL as needed (exit early).
    return;  // Exit early for an uninitialized HAL
  }

  // Mock implementation for HAL_Delay

  uint32_t startTime = hal_current_time;

  // Simulate the passage of time
  for(uint32_t elapsedTime=0; elapsedTime<Delay; elapsedTime++) {
    // Simulate 1 millisecond delay
    usleep(1000);
    hal_current_time++;
  }
}