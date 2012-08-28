/**
 * @file        AT25DB161D.c
 * @date        2012, August 28
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup AT25DB161D_Device_Driver
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "AT25DB161D.h"

/* From BR-RTOS. */
#include "spi_core.h"

/* From ST standard peripheral library. */
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * The flash memory SPI bus name.
 */
#define AT25DB161D_SPI_BUS_NAME "spi1"

/**
 * The flash device name.
 */
#define AT25DB161D_DEV_NAME     "flash1"

/* Flash memory pin definitions. */
#define AT25DB161D_CS_PORT      (GPIOA)
#define AT25DB161D_CS_PIN       (GPIO_Pin_4)

/* Chip select assert and deassert operations. */
#define AT25DB161D_CS_ASSERT()    (GPIO_ResetBits(AT25DB161D_CS_PORT, AT25DB161D_CS_PIN))
#define AT25DB161D_CS_DEASSERT()  (GPIO_SetBits(AT25DB161D_CS_PORT, AT25DB161D_CS_PIN))

/**
 * The dummy byte.
 */
#define AT25DB161D_DUMMY_BYTE   (0xFFU)

/* ---------------------------------------------------------------------------------------------------------------------------------- */
/* TABLE OF COMMANDS                                                                                                                  */
/* ---------------------------------------------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------------------------------------------------- */
/* 1. Read Commands                                                                                                                   */
/* ---------------------------------------------------------------------------------------------------------------------------------- */
#define AT25DB161D_CMD_READ_PAGE          ((uint8_t)0xD2U)  /**< Main Memory Page Read D2H                                            */
#define AT25DB161D_CMD_READ_ARRAY_1       ((uint8_t)0xE8U)  /**< Continuous Array Read (Legacy Command)                               */
#define AT25DB161D_CMD_READ_ARRAY_2       ((uint8_t)0x03U)  /**< Continuous Array Read (Low Frequency)                                */
#define AT25DB161D_CMD_READ_ARRAY_3       ((uint8_t)0x0BU)  /**< Continuous Array Read (High Frequency)                               */
#define AT25DB161D_CMD_READ_BUFFER_1_LOW  ((uint8_t)0xD1U)  /**< Buffer 1 Read (Low Frequency)                                        */
#define AT25DB161D_CMD_READ_BUFFER_2_LOW  ((uint8_t)0xD3U)  /**< Buffer 2 Read (Low Frequency)                                        */
#define AT25DB161D_CMD_READ_BUFFER_1      ((uint8_t)0xD4U)  /**< Buffer 1 Read                                                        */
#define AT25DB161D_CMD_READ_BUFFER_2      ((uint8_t)0xD6U)  /**< Buffer 2 Read                                                        */

/* ---------------------------------------------------------------------------------------------------------------------------------- */
/* 2. Program and Erase Commands                                                                                                      */
/* ---------------------------------------------------------------------------------------------------------------------------------- */
#define AT25DB161D_CMD_WRITE_BUFFER_1       ((uint8_t)0x84U)        /**< Buffer 1 Write                                               */
#define AT25DB161D_CMD_WRITE_BUFFER_2       ((uint8_t)0x87U)        /**< Buffer 2 Write                                               */
#define AT25DB161D_CMD_B1_TO_MEM_ERASE      ((uint8_t)0x83U)        /**< Buffer 1 to Main Memory Page Program with Built-in Erase     */
#define AT25DB161D_CMD_B2_TO_MEM_ERASE      ((uint8_t)0x86U)        /**< Buffer 2 to Main Memory Page Program with Built-in Erase     */
#define AT25DB161D_CMD_B1_TO_MEM_NO_ERASE   ((uint8_t)0x88U)        /**< Buffer 1 to Main Memory Page Program without Built-in Erase  */
#define AT25DB161D_CMD_B2_TO_MEM_NO_ERASE   ((uint8_t)0x89U)        /**< Buffer 2 to Main Memory Page Program without Built-in Erase  */
#define AT25DB161D_CMD_PAGE_ERASE           ((uint8_t)0x81U)        /**< Page Erase                                                   */
#define AT25DB161D_CMD_BLOCK_ERASE          ((uint8_t)0x50U)        /**< Block Erase                                                  */
#define AT25DB161D_CMD_SECTOR_ERASE         ((uint8_t)0x7CU)        /**< Sector Erase                                                 */
#define AT25DB161D_CMD_CHIP_ERASE           ((uint32_t)0xC794809AU) /**< Chip Erase                                                   */
#define AT25DB161D_CMD_PROGRAM_THROUGH_B1   ((uint8_t)0x82U)        /**< Main Memory Page Program Through Buffer 1                    */
#define AT25DB161D_CMD_PROGRAM_THROUGH_B2   ((uint8_t)0x85U)        /**< Main Memory Page Program Through Buffer 2                    */

/* ---------------------------------------------------------------------------------------------------------------------------------- */
/* 3. Protection and Security Commands                                                                                                */
/* ---------------------------------------------------------------------------------------------------------------------------------- */
#define AT25DB161D_CMD_SECTOR_PROTECTION_ENABLE   ((uint32_t)0x3D2A7FA9U) /**< Enable Sector Protection                               */
#define AT25DB161D_CMD_SECTOR_PROTECTION_DISABLE  ((uint32_t)0x3D2A7F9AU) /**< Disable Sector Protection                              */
#define AT25DB161D_CMD_SEC_PROT_REG_ERASE         ((uint32_t)0x3D2A7FCFU) /**< Erase Sector Protection Register                       */
#define AT25DB161D_CMD_SEC_PROT_REG_PROGRAM       ((uint32_t)0x3D2A7FFCU) /**< Program Sector Protection Register                     */
#define AT25DB161D_CMD_SEC_PROT_REG_READ          ((uint8_t)0x32U)        /**< Read Sector Protection Register                        */
#define AT25DB161D_CMD_SECTOR_LOCKDOWN            ((uint32_t)0x3D2A7F30U) /**< Sector Lockdown                                        */
#define AT25DB161D_CMD_SECTOR_LOCKDOWN_REG_READ   ((uint8_t)0x35U)        /**< Read Sector Lockdown Register                          */
#define AT25DB161D_CMD_SECURITY_REGISTER_PROGRAM  ((uint32_t)0x9B000000U) /**< Program Security Register                              */
#define AT25DB161D_CMD_SECURITY_REGISTER_READ     ((uint8_t)0x77U)        /**< Read Security Register                                 */

/* ---------------------------------------------------------------------------------------------------------------------------------- */
/* 4. Additional Commands                                                                                                             */
/* ---------------------------------------------------------------------------------------------------------------------------------- */
#define AT25DB161D_CMD_MEM_TO_BUFFER_1_TXFER  ((uint8_t)0x53U)  /**< Main Memory Page to Buffer 1 Transfer                            */
#define AT25DB161D_CMD_MEM_TO_BUFFER_2_TXFER  ((uint8_t)0x55U)  /**< Main Memory Page to Buffer 2 Transfer                            */
#define AT25DB161D_CMD_MEM_COMP_BUFFER_1      ((uint8_t)0x60U)  /**< Main Memory Page to Buffer 1 Compare                             */
#define AT25DB161D_CMD_MEM_COMP_BUFFER_2      ((uint8_t)0x61U)  /**< Main Memory Page to Buffer 2 Compare                             */
#define AT25DB161D_CMD_AUTO_PAGE_REWRITE_B1   ((uint8_t)0x58U)  /**< Auto Page Rewrite through Buffer 1                               */
#define AT25DB161D_CMD_AUTO_PAGE_REWRITE_B2   ((uint8_t)0x59U)  /**< Auto Page Rewrite through Buffer 2                               */
#define AT25DB161D_CMD_DEEP_POWER_DOWN_ENTER  ((uint8_t)0xB9U)  /**< Deep Power-down                                                  */
#define AT25DB161D_CMD_DEEP_POWER_DOWN_EXIT   ((uint8_t)0xABU)  /**< Resume from Deep Power-down                                      */
#define AT25DB161D_CMD_STATUS_REG_READ        ((uint8_t)0xD7U)  /**< Status Register Read                                             */
#define AT25DB161D_CMD_MANUFACTURER_ID_READ   ((uint8_t)0x9FU)  /**< Manufacturer and Device ID Read                                  */

typedef BR_Err_t (*AT25DB161DIoctlHandler)(BR_Device_t* device, void* param);


/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/
static uint32_t __AT25DB161DRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
static uint32_t __AT25DB161DWrite(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
static BR_Err_t __AT25DB161DControl(BR_Device_t* device, uint8_t cmd, void* param);
static BR_Err_t __AT25DB161DPageErase(BR_Device_t* device, void* param);
static BR_Err_t __AT25DB161DBlockErase(BR_Device_t* device, void* param);
static BR_Err_t __AT25DB161DSegmentErase(BR_Device_t* device, void* param);
static BR_Err_t __AT25DB161DChipErase(BR_Device_t* device, void* param);
static BR_Err_t __AT25DB161DDeepPowerDown(BR_Device_t* device, void* param);
static BR_Err_t __AT25DB161DWakeUp(BR_Device_t* device, void* param);
static BR_Err_t __AT25DB161DGetManufactureID(BR_Device_t* device, void* param);


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/

static uint8_t buffer[5U];

static const AT25DB161DIoctlHandler ioctlHandler[7U] =
{
    /* Control Command                          Function Handler */
    /* AT25DB161D_IOCTL_PAGE_ERASE          */  __AT25DB161DPageErase,
    /* AT25DB161D_IOCTL_BLOCK_ERASE         */  __AT25DB161DBlockErase,
    /* AT25DB161D_IOCTL_SEGMENT_ERASE       */  __AT25DB161DSegmentErase,
    /* AT25DB161D_IOCTL_CHIP_ERASE          */  __AT25DB161DChipErase,
    /* AT25DB161D_IOCTL_DEEP_POWER_DOWN     */  __AT25DB161DDeepPowerDown,
    /* AT25DB161D_IOCTL_WAKE_UP             */  __AT25DB161DWakeUp,
    /* AT25DB161D_IOCTL_GET_MANUFACTURE_ID  */  __AT25DB161DGetManufactureID,
};

static BR_SpiDevice_t at25db161dSpiDevice =
{
    .parent.init      = NULL,
    .parent.open      = NULL,
    .parent.close     = NULL,
    .parent.read      = __AT25DB161DRead,
    .parent.write     = __AT25DB161DWrite,
    .parent.control   = __AT25DB161DControl,
    .config.maxFreqHz = 3000000U,
    .config.mode      = BR_SPI_MODE_0,
    .config.dataSize  = BR_SPI_DATA_SIZE_8_BITS,
};


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

static uint32_t __AT25DB161DRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes)
{
  /* Check the parameters. */
  BR_ASSERT(NULL != device);

  if (NULL != device)
  {
    /* Assert the chip select pin. */
    AT25DB161D_CS_ASSERT();
    /* Send the opcode, the address and the dummy byte. */
    buffer[0U] = AT25DB161D_CMD_READ_ARRAY_3;
    buffer[1U] = U32_BYTE_2(address);
    buffer[2U] = U32_BYTE_1(address);
    buffer[3U] = U32_BYTE_0(address);
    buffer[4U] = AT25DB161D_DUMMY_BYTE;
    BR_SpiSend((BR_SpiDevice_t*)device, buffer, 5U);
    /* Receive the data bytes. */
    BR_SpiReceive((BR_SpiDevice_t*)device, buffer, nBytes);
    /* Deassert the chip select pin. */
    AT25DB161D_CS_DEASSERT();
    return nBytes;
  }

  return 0U;
}

static uint32_t __AT25DB161DWrite(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes)
{
  // TODO
  return 0U;
}

static BR_Err_t __AT25DB161DControl(BR_Device_t* device, uint8_t cmd, void* param)
{
  /* Check the parameters. */
  BR_ASSERT(NULL != device);
  BR_ASSERT(cmd <= AT25DB161D_IOCTL_GET_MANUFACTURE_ID);

  if ((NULL != device) && (cmd <= AT25DB161D_IOCTL_GET_MANUFACTURE_ID))
  {
    return ioctlHandler[cmd](device, param);
  }

  return E_INVAL;
}

static BR_Err_t __AT25DB161DPageErase(BR_Device_t* device, void* param)
{
  // TODO
  return E_ENOSYS;
}

static BR_Err_t __AT25DB161DBlockErase(BR_Device_t* device, void* param)
{
  // TODO
  return E_ENOSYS;
}

static BR_Err_t __AT25DB161DSegmentErase(BR_Device_t* device, void* param)
{
  // TODO
  return E_ENOSYS;
}

static BR_Err_t __AT25DB161DChipErase(BR_Device_t* device, void* param)
{
  // TODO
  return E_ENOSYS;
}

static BR_Err_t __AT25DB161DDeepPowerDown(BR_Device_t* device, void* param)
{
  // TODO
  return E_ENOSYS;
}

static BR_Err_t __AT25DB161DWakeUp(BR_Device_t* device, void* param)
{
  // TODO
  return E_ENOSYS;
}

static BR_Err_t __AT25DB161DGetManufactureID(BR_Device_t* device, void* param)
{
  /* Check the parameters. */
  BR_ASSERT(NULL != param);

  if (NULL != param)
  {
    /* Assert the chip select pin. */
    AT25DB161D_CS_ASSERT();
    /* Send the opcode. */
    buffer[0U] = AT25DB161D_CMD_MANUFACTURER_ID_READ;
    BR_SpiSend((BR_SpiDevice_t*)device, buffer, 1U);
    /* Receive the data bytes. */
    BR_SpiReceive((BR_SpiDevice_t*)device, param, 4U);
    /* Deassert the chip select pin. */
    AT25DB161D_CS_DEASSERT();
    return E_OK;
  }

  return E_INVAL;
}

/**
 * @brief Initialize and register the AT25DB161D device driver.
 */
void AT25DB161DInit(void)
{
  /* RCC Configuration. */
  {
    //RCC_APB2PeriphClockCmd();
    // TODO
  }

  /* GPIO Configuration. */
  {
    GPIO_InitTypeDef gpioInit;

    /* Chip select pin. */
    gpioInit.GPIO_Pin = AT25DB161D_CS_PIN;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(AT25DB161D_CS_PORT, &gpioInit);
  }

  /* Register the device driver within the BR-RTOS kernel. */
  BR_SpiDeviceRegister(&at25db161dSpiDevice, AT25DB161D_DEV_NAME);
  /* Attach the flash device to the SPI bus device. */
  BR_SpiAttach(&at25db161dSpiDevice, AT25DB161D_SPI_BUS_NAME);
}


/** @} */

