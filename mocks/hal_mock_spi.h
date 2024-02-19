// hal_mock_spi.h
#ifndef HAL_MOCK_SPI_H
#define HAL_MOCK_SPI_H

#include "hal_mock_general.h"

#define HAL_SPI_ERROR_NONE              0U    // No error
#define HAL_SPI_ERROR_HAL_UNITIALIZED   10U    // HAL uninitialized error

#define HAL_SPI_ERROR_NULL_PARAM        100U    // SPI null parameter error
#define HAL_SPI_ERROR_UNITIALIZED       101U    // SPI uninitialized error
#define HAL_SPI_ERROR_BUSY              102U    // SPI busy error
#define HAL_SPI_ERROR_FAILSTATE         103U    // SPI failstate error
#define HAL_SPI_ERROR_BAD_SLAVE         104U    // SPI bad slave error

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
  uint8_t                    *pTxBuffPtr;               /*!< Pointer to SPI Tx transfer Buffer        */
  uint16_t                   TxXferSize;                /*!< SPI Tx Transfer size                     */
  uint8_t                    *pRxBuffPtr;               /*!< Pointer to SPI Rx transfer Buffer        */
  uint16_t                   RxXferSize;                /*!< SPI Rx Transfer size                     */
  __IO HAL_SPI_StateTypeDef  State;                     /*!< SPI communication state                  */
  __IO uint32_t              ErrorCode;                 /*!< SPI Error code                           */
} SPI_HandleTypeDef;

// Functions specific to the mock implementation
HAL_StatusTypeDef Set_SPI_Transmit_Mock_Slave(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef Set_SPI_Receive_Mock_Slave(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);

// Mocked SPI functions
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_DeInit(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);

#endif  // HAL_MOCK_SPI_H
