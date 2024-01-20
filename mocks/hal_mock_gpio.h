#ifndef HAL_MOCK_GPIO_H
#define HAL_MOCK_GPIO_H

#include "hal_mock_general.h" // Include for HAL_StatusTypeDef

// Mocked GPIO defines/macros
#define GPIO_PIN_MASK              0x0000FFFFU
#define IS_GPIO_PIN_ACTION(ACTION) (((ACTION) == GPIO_PIN_RESET) || ((ACTION) == GPIO_PIN_SET))
#define IS_GPIO_PIN(PIN)           (((((uint32_t)PIN) & GPIO_PIN_MASK ) != 0x00U) && ((((uint32_t)PIN) & ~GPIO_PIN_MASK) == 0x00U))

// Mocked GPIO typedefs
typedef struct
{
  uint32_t RESERVED[255];
  uint32_t IDR; // Input Data Register
  uint32_t ODR; // Output Data Register
  uint32_t BSRR; // Bit Set/Reset Register
} GPIO_TypeDef;

typedef struct
{
  uint32_t Pin;
  uint32_t Mode;
  uint32_t Pull;
  uint32_t Speed;
  uint32_t Alternate;
} GPIO_InitTypeDef;

typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
} GPIO_PinState;


// Mocked GPIO functions
HAL_StatusTypeDef HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init);
GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

#endif // HAL_MOCK_GPIO_H
