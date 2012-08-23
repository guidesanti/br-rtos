/**
 * @file        spi_core.h
 * @date        2012, August 22
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */


#ifndef _SPI_CORE_H_
#define _SPI_CORE_H_

/**
 * @addtogroup SPICore SPI Core
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

typedef struct
{
  uint32_t  freqHz;
  uint8_t   mode;
  uint8_t   dataSize;
} BR_SpiConfig_t;

typedef struct
{
  BR_Device_t*  parent;
  BR_Device_t*  owner;
  BR_Mutex_t    lock;
  void          (*configure)(BR_SpiBus_t* bus, BR_SpiConfig_t* config);
  void          (*transfer)(BR_SpiBus_t* bus, void* txBuffer, void* rxBuffer, uint32_t nBytes);
} BR_SpiBus_t;

typedef struct
{
  BR_Device_t*    parent;
  BR_SpiBus_t*    bus;
  BR_SpiConfig_t  config;
} BR_SpiDevice_t;


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/
BR_Err_t BR_SpiBusRegister(BR_SpiBus_t* bus, char* name);
BR_Err_t BR_SpiBusAcquire(BR_SpiDevice_t* device);
BR_Err_t BR_SpiBusRelease(BR_SpiDevice_t* device);
BR_Err_t BR_SpiAttach(BR_SpiBus_t* bus, BR_Device_t* device);
BR_Err_t BR_SpiTransfer(BR_SpiDevice_t* device, void* txBuffer, void* rxBuffer, uint32_t length);
BR_Err_t BR_SpiSend(BR_SpiDevice_t* device, void* txBuffer, uint32_t length);
BR_Err_t BR_SpiReceive(BR_SpiDevice_t* device, void* rxBuffer, uint32_t length);


/** @} */

#endif /* _SPI_CORE_H_ */
