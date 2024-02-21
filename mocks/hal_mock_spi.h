// hal_mock_spi.h
#ifndef HAL_MOCK_SPI_H
#define HAL_MOCK_SPI_H

#include "hal_mock_general.h"

#define HAL_SPI_ERROR_NONE                0U    // No error
#define HAL_SPI_ERROR_HAL_UNINITIALIZED   10U    // HAL uninitialized error

#define HAL_SPI_ERROR_NULL_PARAM          100U    // SPI null parameter error
#define HAL_SPI_ERROR_UNINITIALIZED       101U    // SPI uninitialized error
#define HAL_SPI_ERROR_BUSY                102U    // SPI busy error
#define HAL_SPI_ERROR_FAILSTATE           103U    // SPI failstate error
#define HAL_SPI_ERROR_MSG_TOO_BIG         104U    // SPI message too big error

#define MOCK_SPI_MAX_MSG_SIZE             256U    // Maximum I2C transfer size

// Mocked SPI typedefs
typedef enum
{
  HAL_SPI_STATE_RESET      = 0U,    /*!< Peripheral not Initialized                         */
  HAL_SPI_STATE_READY      = 1U,    /*!< Peripheral Initialized and ready for use           */
  HAL_SPI_STATE_BUSY       = 2U,    /*!< an internal process is ongoing                     */
  HAL_SPI_STATE_ERROR      = 3U,    /*!< SPI error state                                    */
} HAL_SPI_StateTypeDef;

typedef struct __SPI_HandleTypeDef
{
  uint8_t                    TxMsgBuff[MOCK_SPI_MAX_MSG_SIZE];     // SPI TX transfer message buffer
  uint16_t                   TxMsgSize;                            // SPI TX transfer message size
  uint8_t                    RxMsgBuff[MOCK_SPI_MAX_MSG_SIZE];     // SPI RX transfer message buffer
  uint16_t                   RxMsgSize;                            // SPI RX transfer message size
  __IO HAL_SPI_StateTypeDef  State;                                // SPI communication state
  __IO uint32_t              ErrorCode;                            // SPI Error code
} SPI_HandleTypeDef;

// Mocked SPI functions
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);

#endif  // HAL_MOCK_SPI_H
