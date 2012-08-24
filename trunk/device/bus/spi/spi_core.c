/**
 * @file        spi_core.c
 * @date        2012, August 23
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup SPICore SPI Core
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOS.h"
#include "spi_core.h"
#include "port.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/


/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

/**
 * @brief Register a SPI bus within the kernel.
 * @param [in] bus A pointer to the SPI bus structure to be registered.
 * @param [in] name A string with the name to be used for the SPI bus object.
 * @return Error code.
 * @retval E_OK If the registration is successful.
 * @retval E_INVAL If the parameters check is enabled and any of the parameters is invalid.
 */
BR_Err_t BR_SpiBusRegister(BR_SpiBus_t* bus, char* name)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != bus);
  __BR_ASSERT(NULL != name);
#if (__BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != bus) && (NULL != name))
  {
#endif
    __BR_ENTER_CRITICAL();

    /* Initialize the SPI bus structure. */
    bus->owner = NULL;
    BR_IpcMutexInit(&(bus->lock), name);
    /* Register the SPI bus. */
    ret = BR_DeviceRegister(name, &(bus->parent));

    __BR_EXIT_CRITICAL();
#if (__BR_CHECK_FUNC_PARAMETERS)
  }
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief Register a SPI device within the kernel.
 * @param [in] bus A pointer to the SPI device structure to be registered.
 * @param [in] name A string with the name to be used for the SPI device object.
 * @return Error code.
 * @retval E_OK If the registration is successful.
 * @retval E_INVAL If the parameters check is enabled and any of the parameters is invalid.
 */
BR_Err_t BR_SpiDeviceRegister(BR_SpiDevice_t* device, char* name)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
  __BR_ASSERT(NULL != name);
#if (__BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != device) && (NULL != name))
  {
#endif
    __BR_ENTER_CRITICAL();

    /* Initialize the SPI device structure. */
    device->bus = NULL;
    /* Register the SPI device. */
    ret = BR_DeviceRegister(name, &(device->parent));

    __BR_EXIT_CRITICAL();
#if (__BR_CHECK_FUNC_PARAMETERS)
  }
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief Attach a SPI device to a SPI bus.
 * @param [in] device The SPI device.
 * @param [in] busName The of the SPI bus.
 * @return Error code.
 * @retval E_OK If the device is successful attached to the bus.
 * @retval E_ERROR If an error occurred during the attach.
 * @retval E_INVAL If the parameters check is enabled and any of the parameters is invalid.
 */
BR_Err_t BR_SpiAttach(BR_SpiDevice_t* device, char* busName)
{
  BR_Err_t ret = E_OK;
  BR_SpiBus_t* bus = NULL;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
  __BR_ASSERT(NULL != busName);
#if (__BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != device) && (NULL != busName))
  {
#endif
    __BR_ENTER_CRITICAL();

    /* Look for the SPI bus. */
    bus = (BR_SpiBus_t*)BR_DeviceFind(busName);
    if (NULL != bus)
    {
      /* Attach the device to bus. */
      device->bus = bus;
    }
    else
    {
      ret = E_ERROR;
    }

    __BR_EXIT_CRITICAL();
#if (__BR_CHECK_FUNC_PARAMETERS)
  }
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief TODO
 */
BR_Err_t BR_SpiAcquire(BR_SpiDevice_t* device)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
#if (__BR_CHECK_FUNC_PARAMETERS)
  if (NULL != device)
  {
#endif
    /* Acquire the SPI bus lock. */
    if (E_OK == BR_IpcMutexAcquire(&(device->bus->lock), BR_IPC_WAIT_FOREVER))
    {
      /* Check if the device is the current owner of the bus. */
      if (device->bus->owner != device)
      {
        /* Re-configure the SPI bus. */
        if (E_OK == device->bus->configure(device->bus, &(device->config)))
        {
          device->bus->owner = device;
        }
        else
        {
          BR_IpcMutexRelease(&(device->bus->lock));
          ret = E_ERROR;
        }
      }
    }
    else
    {
      ret = E_ERROR;
    }
#if (__BR_CHECK_FUNC_PARAMETERS)
  }
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief TODO
 */
BR_Err_t BR_SpiRelease(BR_SpiDevice_t* device)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
#if (__BR_CHECK_FUNC_PARAMETERS)
  if (NULL != device)
  {
#endif
    BR_IpcMutexRelease(&(device->bus->lock));
#if (__BR_CHECK_FUNC_PARAMETERS)
  }
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief TODO
 */
BR_Err_t BR_SpiTransfer(BR_SpiDevice_t* device, void* txBuffer, void* rxBuffer, uint32_t length)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
  __BR_ASSERT((NULL != txBuffer) || (NULL != rxBuffer));
  __BR_ASSERT(length > 0U);
#if (__BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != device) &&
      ((NULL != txBuffer) || (NULL != rxBuffer)) &&
      (length > 0U))
  {
#endif
    /* Acquire the SPI bus lock. */
    if (E_OK == BR_IpcMutexAcquire(&(device->bus->lock), BR_IPC_WAIT_FOREVER))
    {
      /* Check if the device is the current owner of the bus. */
      if (device->bus->owner != device)
      {
        /* Re-configure the SPI bus. */
        if (E_OK == device->bus->configure(device->bus, &(device->config)))
        {
          device->bus->owner = device;
        }
        else
        {
          ret = E_ERROR;
          goto __EXIT;
        }
      }

      /* Execute the SPI transfer. */
      ret = device->bus->transfer(device->bus, txBuffer, rxBuffer, length);
    }
    else
    {
      ret = E_ERROR;
    }
#if (__BR_CHECK_FUNC_PARAMETERS)
  }
  else
  {
    ret = E_INVAL;
  }
#endif

__EXIT:
  BR_IpcMutexRelease(&(device->bus->lock));
  return ret;
}

/**
 * @brief TODO
 */
BR_Err_t BR_SpiSend(BR_SpiDevice_t* device, void* txBuffer, uint32_t length)
{
  return BR_SpiTransfer(device, txBuffer, NULL, length);
}

/**
 * @brief TODO
 */
BR_Err_t BR_SpiReceive(BR_SpiDevice_t* device, void* rxBuffer, uint32_t length)
{
  return BR_SpiTransfer(device, NULL, rxBuffer, length);
}


/** @} */

