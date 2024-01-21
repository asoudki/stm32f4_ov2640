#include "hal_mock_gpio.h"

HAL_StatusTypeDef HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init)
{
    // Check if the HAL has been left uninitialized
    if(!hal_initialized) {
      // Mock implementation: Handle uninitialized HAL as needed (return error value).
      return HAL_ERROR;  // Return error value for an uninitialized HAL
    }

    // Mock implementation of HAL_GPIO_Init.

    // Simulate the initial values of IDR, ODR and BSRR (Set to 0)
    GPIOx->IDR = 0;
    GPIOx->ODR = 0;
    GPIOx->BSRR = 0;

    return HAL_OK;
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    // Check if the HAL has been left uninitialized
    if(!hal_initialized) {
      // Mock implementation: Handle uninitialized HAL as needed (return a default value).
      return GPIO_PIN_RESET;  // Return a default value for an uninitialized HAL.
    }

    // Check if a valid GPIO_Pin has been given.
    if(!IS_GPIO_PIN(GPIO_Pin)) {
      // Mock implementation: Handle invalid GPIO_Pin as needed (return a default value).
      return GPIO_PIN_RESET;  // Return a default value for an invalid GPIO_PIN.
    }

    // Mock implementation of HAL_GPIO_ReadPin.

    // Simulate reading from IDR.
    GPIO_PinState bitstatus;
    if((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET)
    {
      bitstatus = GPIO_PIN_SET;
    }
    else
    {
      bitstatus = GPIO_PIN_RESET;
    }

    return bitstatus;
}

void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    // Check if the HAL has been left uninitialized
    if(!hal_initialized) {
      // Mock implementation: Handle uninitialized HAL as needed (exit early).
      return;  // Exit early for an uninitialized HAL.
    }

    // Check if a valid GPIO_Pin has been given.
    if(!IS_GPIO_PIN(GPIO_Pin)) {
      // Mock implementation: Handle invalid GPIO_Pin as needed (exit early).
      return;  // Exit early for an invalid GPIO_Pin.
    }

    // Mock implementation of HAL_GPIO_WritePin.

    // Simulate writing to ODR
    if (PinState != GPIO_PIN_RESET)
    {
      // Set the corresponding bits in ODR to simulate pin set.
      GPIOx->ODR |= GPIO_Pin;
      // Set BSRR to simulate pin set.
      GPIOx->BSRR = GPIO_Pin;
      // Set IDR to match ODR after some delay (skipped for brevity).
      GPIOx->IDR |= GPIO_Pin;
    }
    else
    {
      // Reset the corresponding bits in ODR to simulate pin reset.
      GPIOx->ODR &= ~GPIO_Pin;
      // Set BSRR to simulate pin reset.
      GPIOx->BSRR = (uint32_t)GPIO_Pin << 16U;
      // Set IDR to match ODR after some delay (skipped for brevity).
      GPIOx->IDR &= ~GPIO_Pin;
    }
}