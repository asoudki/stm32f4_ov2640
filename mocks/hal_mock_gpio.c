#include "hal_mock_gpio.h"

HAL_StatusTypeDef HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init)
{
    // Check if the HAL has been left uninitialized
    if(!hal_initialized) {
      // Mock implementation: Handle uninitialized HAL as needed (return error value).
      printf("Mock: HAL_GPIO_Init called, but HAL is uninitialized");
      return HAL_ERROR;  // Return error value for an uninitialized HAL
    }

    // Mock implementation of HAL_GPIO_Init.

    // Print a message indicating that HAL_GPIO_Init was called.
    printf("Mock: HAL_GPIO_Init called\n");

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
      printf("Mock: HAL_GPIO_ReadPin called, but HAL is uninitialized");
      return GPIO_PIN_RESET;  // Return a default value for an uninitialized HAL.
    }

    // Check if a valid GPIO_Pin has been given.
    if(!IS_GPIO_PIN(GPIO_Pin)) {
      // Mock implementation: Handle invalid GPIO_Pin as needed (return a default value).
      printf("Mock: HAL_GPIO_ReadPin called, but given GPIO_Pin is invalid");
      return GPIO_PIN_RESET;  // Return a default value for an invalid GPIO_PIN.
    }

    // Mock implementation of HAL_GPIO_ReadPin.
    printf("Mock: HAL_GPIO_ReadPin called\n");

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
      // Mock implementation: Handle uninitialized HAL as needed (print a message and exit).
      printf("Mock: HAL_GPIO_WritePin called, but HAL is uninitialized");
      return;  // Print a message and exit.
    }

    // Check if a valid GPIO_Pin has been given.
    if(!IS_GPIO_PIN(GPIO_Pin)) {
      // Mock implementation: Handle invalid GPIO_Pin as needed (print a message and exit).
      printf("Mock: HAL_GPIO_WritePin called, but given GPIO_Pin is invalid");
      return;  // Print a message and exit.
    }

    // Mock implementation of HAL_GPIO_WritePin.
    printf("Mock: HAL_GPIO_WritePin called\n");

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
      GPIOx->IDR &= GPIO_Pin;
    }
}