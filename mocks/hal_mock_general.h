// hal_mock_general.h
#ifndef HAL_MOCK_GENERAL_H
#define HAL_MOCK_GENERAL_H

// Common STD libraries to include
#include <stdint.h>
#include <unistd.h>

// Mocked general HAL defines/macros
#define     __I     volatile const       /*!< Defines 'read only' permissions */
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

#define HAL_MAX_DELAY      0xFFFFFFFFU

// Mocked general HAL typedefs
typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef enum
{
  HAL_PIN_RESET = 0,
  HAL_PIN_SET
} HAL_PinState;

typedef enum 
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U  
} HAL_LockTypeDef;

// Global variables
extern uint8_t hal_initialized;
extern uint32_t hal_current_time;

// Mocked general HAL functions
void HAL_Init(void);
void HAL_Delay(uint32_t Delay);

#endif  // HAL_MOCK_GENERAL_H
