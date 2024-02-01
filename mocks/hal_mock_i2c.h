#ifndef HAL_MOCK_I2C_H
#define HAL_MOCK_I2C_H

#include "hal_mock_general.h"

// Mocked I2C defines/macros

// Error codes from the I2C HAL
#define HAL_I2C_ERROR_NONE              0x00000000U    // No error
#define HAL_I2C_ERROR_BERR              0x00000001U    // BERR error
#define HAL_I2C_ERROR_ARLO              0x00000002U    // ARLO error
#define HAL_I2C_ERROR_AF                0x00000004U    // AF error
#define HAL_I2C_ERROR_OVR               0x00000008U    // OVR error
#define HAL_I2C_ERROR_SIZE              0x00000040U    // Size Management error

// Error codes specific to the mock implementation
#define HAL_I2C_ERROR_HAL_UNITIALIZED   0x00000010U    // HAL uninitialized error
#define HAL_I2C_ERROR_INVALID_INSTANCE  0x00000020U    // Invalid I2C instance error
#define HAL_I2C_ERROR_BUSY              0x00000080U    // I2C busy error     
#define HAL_I2C_ERROR_ALREADY_INITIALIZED 0x00000100U  // I2C already initialized error


// Defines used for mock implementation
#define HAL_I2C_MOCK_BUFFER_SIZE        256U            // Maximum buffer size for mocked I2C transfers

// I2C peripheral mock structure
typedef struct {
    uint8_t DR;                                       // Data Register: Simulates data transmission and reception
    uint32_t SR;                                      // Status Register: Optional, for detailed hardware-level statuses
    uint16_t OAR;                                     // Own Address Register: Device's own address in slave mode or target address in master mode
    uint32_t ErrorFlags;                              // Error Flags: Simulates detailed hardware-level error conditions
    struct {
        uint16_t TargetAddress;                       // Transaction Target Address: Address of the target device in master mode
        uint32_t ReadWrite;                           // Transaction Read/Write Flag: Indicates read or write operation
        uint32_t DataSize;                            // Transaction Data Size: Size of data to be transferred
    } TransactionInfo;                                // Transaction Information: Details about the ongoing transaction in master mode
} I2C_TypeDef;

// I2C initialization structure
typedef struct {
    uint32_t ClockSpeed;      // Clock speed in Hz
    uint32_t DutyCycle;       // Duty cycle for I2C clock
    uint32_t OwnAddress1;     // First device own address
    uint32_t AddressingMode;  // Addressing mode (7/10 bits)
    uint32_t DualAddressMode; // Dual address mode enable/disable
    uint32_t OwnAddress2;     // Second device own address
    uint32_t GeneralCallMode; // General call mode enable/disable
    uint32_t NoStretchMode;   // Clock stretching mode (enable/disable)
} I2C_InitTypeDef;

// I2C state enumeration
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

// I2C mode enumeration
typedef enum
{
  HAL_I2C_MODE_NONE               = 0x00U,   /*!< No I2C communication on going             */
  HAL_I2C_MODE_MASTER             = 0x10U,   /*!< I2C communication is in Master Mode       */
  HAL_I2C_MODE_SLAVE              = 0x20U,   /*!< I2C communication is in Slave Mode        */
  HAL_I2C_MODE_MEM                = 0x40U    /*!< I2C communication is in Memory Mode       */
} HAL_I2C_ModeTypeDef;

// I2C handle structure
typedef struct {
    I2C_TypeDef                 *Instance;                        // I2C registers base address
    I2C_InitTypeDef             Init;                             // I2C communication parameters
    uint8_t                     pBuff[HAL_I2C_MOCK_BUFFER_SIZE];  // I2C transfer buffer
    uint16_t                    XferSize;                         // I2C transfer size
    __IO uint16_t               XferCount;                        // I2C transfer counter
    HAL_LockTypeDef             Lock;                             // I2C locking object
    __IO HAL_I2C_StateTypeDef   State;                            // I2C communication state
    __IO HAL_I2C_ModeTypeDef    Mode;                             // I2C communication mode
    __IO uint32_t               ErrorCode;                        // I2C Error code
} I2C_HandleTypeDef;

// Functions specific to the mock implementation
HAL_StatusTypeDef Reset_I2C_Handle(I2C_HandleTypeDef *hi2c);

// Mock function declarations
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif // HAL_MOCK_I2C_H
