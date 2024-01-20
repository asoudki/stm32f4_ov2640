#ifndef HAL_MOCK_I2C_H
#define HAL_MOCK_I2C_H

#include "hal_mock_general.h"
#include "hal_stub_dma.h"

// Mocked I2C typedefs
typedef struct
{
  __IO uint32_t CR1;        /*!< I2C Control register 1,     Address offset: 0x00 */
  __IO uint32_t CR2;        /*!< I2C Control register 2,     Address offset: 0x04 */
  __IO uint32_t OAR1;       /*!< I2C Own address register 1, Address offset: 0x08 */
  __IO uint32_t OAR2;       /*!< I2C Own address register 2, Address offset: 0x0C */
  __IO uint32_t DR;         /*!< I2C Data register,          Address offset: 0x10 */
  __IO uint32_t SR1;        /*!< I2C Status register 1,      Address offset: 0x14 */
  __IO uint32_t SR2;        /*!< I2C Status register 2,      Address offset: 0x18 */
  __IO uint32_t CCR;        /*!< I2C Clock control register, Address offset: 0x1C */
  __IO uint32_t TRISE;      /*!< I2C TRISE register,         Address offset: 0x20 */
  __IO uint32_t FLTR;       /*!< I2C FLTR register,          Address offset: 0x24 */
} I2C_TypeDef;

typedef struct {
    uint32_t ClockSpeed;
    uint32_t DutyCycle;
    uint32_t OwnAddress1;
    uint32_t AddressingMode;
    uint32_t DualAddressMode;
    uint32_t OwnAddress2;
    uint32_t GeneralCallMode;
    uint32_t NoStretchMode;
} I2C_InitTypeDef;

typedef enum
{
  HAL_I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
  HAL_I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
  HAL_I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
  HAL_I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
  HAL_I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
  HAL_I2C_STATE_LISTEN            = 0x28U,   /*!< Address Listen Mode is ongoing            */
  HAL_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< Address Listen Mode and Data Transmission
                                                 process is ongoing                         */
  HAL_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< Address Listen Mode and Data Reception
                                                 process is ongoing                         */
  HAL_I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */
  HAL_I2C_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
  HAL_I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */

} HAL_I2C_StateTypeDef;

typedef enum
{
  HAL_I2C_MODE_NONE               = 0x00U,   /*!< No I2C communication on going             */
  HAL_I2C_MODE_MASTER             = 0x10U,   /*!< I2C communication is in Master Mode       */
  HAL_I2C_MODE_SLAVE              = 0x20U,   /*!< I2C communication is in Slave Mode        */
  HAL_I2C_MODE_MEM                = 0x40U    /*!< I2C communication is in Memory Mode       */
} HAL_I2C_ModeTypeDef;

typedef struct {
  I2C_TypeDef                *Instance;      /*!< I2C registers base address                                            */
  I2C_InitTypeDef            Init;           /*!< I2C communication parameters                                          */
  uint8_t                    *pBuffPtr;      /*!< Pointer to I2C transfer buffer                                        */
  uint16_t                   XferSize;       /*!< I2C transfer size                                                     */
  __IO uint16_t              XferCount;      /*!< I2C transfer counter                                                  */
  __IO uint32_t              XferOptions;    /*!< I2C transfer options                                                  */
  __IO uint32_t              PreviousState;  /*!< I2C communication Previous state and mode context for internal usage  */
  DMA_HandleTypeDef          *hdmatx;        /*!< I2C Tx DMA handle parameters                                          */
  DMA_HandleTypeDef          *hdmarx;        /*!< I2C Rx DMA handle parameters                                          */
  HAL_LockTypeDef            Lock;           /*!< I2C locking object                                                    */
  __IO HAL_I2C_StateTypeDef  State;          /*!< I2C communication state                                               */
  __IO HAL_I2C_ModeTypeDef   Mode;           /*!< I2C communication mode                                                */
  __IO uint32_t              ErrorCode;      /*!< I2C Error code                                                        */
  __IO uint32_t              Devaddress;     /*!< I2C Target device address                                             */
  __IO uint32_t              Memaddress;     /*!< I2C Target memory address                                             */
  __IO uint32_t              MemaddSize;     /*!< I2C Target memory address  size                                       */
  __IO uint32_t              EventCount;     /*!< I2C Event counter                                                     */
} I2C_HandleTypeDef;

// Mocked I2C functions
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif  // HAL_MOCK_I2C_H
