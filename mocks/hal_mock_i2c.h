#ifndef HAL_MOCK_I2C_H
#define HAL_MOCK_I2C_H

#include "hal_mock_general.h"

// Mocked I2C defines/macros

#define HAL_I2C_ERROR_NONE                0U      // No error
#define HAL_I2C_ERROR_HAL_UNINITIALIZED   10U     // HAL uninitialized error

#define HAL_I2C_ERROR_NULL_PARAM          100U    // I2C null parameter error
#define HAL_I2C_ERROR_UNINITIALIZED       101U    // I2C uninitialized error
#define HAL_I2C_ERROR_BUSY                102U    // I2C busy error
#define HAL_I2C_ERROR_FAILSTATE           103U    // I2C failstate error
#define HAL_I2C_ERROR_BAD_SLAVE           104U    // I2C bad slave error

// I2C state enumeration
typedef enum
{
  HAL_I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
  HAL_I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
  HAL_I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
  HAL_I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */
} HAL_I2C_StateTypeDef;

// I2C handle structure
typedef struct {
    uint16_t                    XferAddress;      // I2C target device address
    uint8_t                     *XferBuffPtr;     // I2C transfer buffer pointer
    uint16_t                    XferSize;         // I2C transfer size
    __IO HAL_I2C_StateTypeDef   State;            // I2C communication state
    __IO uint32_t               ErrorCode;        // I2C Error code
} I2C_HandleTypeDef;

// Functions specific to the mock implementation
HAL_StatusTypeDef Set_I2C_Mock_Slave(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

// Mock function declarations
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif // HAL_MOCK_I2C_H
