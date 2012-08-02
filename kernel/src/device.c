/**
 * @file        device.c
 * @date        2012, August 02
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup DevCtrl Device Control
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOSConfig.h"
#include "BR-RTOS.h"
#include "port.h"
#include "device.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * @name Constants, Macro and Type Definitions
 * @{
 */

/** @} */


/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/


/******************************************************************************/
/* K E R N E L  I N T E R N A L  V A R I A B L E S                            */
/******************************************************************************/

/**
 * @name Kernel Internal Variables
 * @{
 */

/** @} */

/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/

/**
 * @name Kernel Internal Functions
 * @{
 */

/**
 * @brief Initialize the device control sub-system.
 */
void __BR_DeviceInit(void)
{
  // TODO
}

/** @} */

/******************************************************************************/
/* P U B L I C  A P I  F U N C T I O N S                                      */
/******************************************************************************/

/**
 * @name Public API Functions
 * @{
 */

/**
 * @brief Register a new device driver into the kernel.
 * @param [in] name The device driver name.
 * @param [in] device A pointer to the device driver.
 * @return Error code.
 * @retval E_OK If the device has been registered with successful.
 * @retval E_ERROR If the kernel is unable to register the device.
 * @retval E_INVAL If the name == NULL or device == NULL and the check parameters
 *                 feature of the BR-RTOS is enabled.
 */
BR_Err_t BR_DeviceRegister(const char* name, BR_Device_t* device)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != name);
  __BR_ASSERT(NULL != device);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != name) && (NULL != device))
#endif
  {
#error "TODO"
    /* Allocate the object memory for the device. */
    // TODO
    /* Insert the device within the kernel device list. */
    // TODO
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
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
BR_Err_t BR_DeviceInit(BR_Device_t* device)
{
#error "TODO"
  // TODO
}

/**
 * @brief Execute the open operation over a device.
 * @param [in] device A pointer to device.
 * @param [in] flags The open operation flags.
 * @return Error code.
 * @retval E_OK If the device is successfully opened.
 * @retval E_ENOSYS If the open operation is not implemented for the device.
 * @retval E_INVAL If device == NULL and the check parameters feature is enabled.
 */
BR_Err_t BR_DeviceOpen(BR_Device_t* device, uint8_t flags)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != device)
#endif
  {
    if (NULL != device->open)
    {
      ret = device->open(device, flags);
    }
    else
    {
      ret = E_ENOSYS;
    }
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief Close a previously opened device.
 * @param [in] device A pointer to device.
 * @return Error code.
 * @retval E_OK If the device is successfully closed.
 * @retval E_ENOSYS If the close operation is not implemented for the device.
 * @retval E_INVAL If device == NULL and the check parameters feature is enabled.
 */
BR_Err_t BR_DeviceClose(BR_Device_t* device)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != device)
#endif
  {
    if (NULL != device->close)
    {
      ret = device->close(device);
    }
    else
    {
      ret = E_ENOSYS;
    }
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief Read data from device.
 * @param [in] device A pointer to device.
 * @param [in] address The address to read data.
 * @param [in] buffer A pointer to the buffer to store data.
 * @param [in] nBytes The number of bytes to read from the device.
 * @return The number of bytes actually read from the device and written to the buffer.
 *
 * Execute a read operation over the device pointed by device parameter.
 * If the read operation is not implemented by the device 0 is returned.
 * If any I/O error occurs during the read operation 0 is returned.
 * If device is NULL or buffer is NULL and the check parameters feature is
 * enable 0 is returned.
 * Otherwise the actually number of bytes read from device is returned.
 */
uint32_t BR_DeviceRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes)
{
  uint32_t ret = 0U;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
  __BR_ASSERT(NULL != buffer);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != device) && (NULL != buffer))
#endif
  {
    if (NULL != device->read)
    {
      ret = device->read(device, address, buffer, nBytes);
    }
    else
    {
      ret = 0U;
    }
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = 0U;
  }
#endif

  return ret;
}

/**
 * @brief Write data to device.
 * @param [in] device A pointer to device.
 * @param [in] address The address to write data.
 * @param [in] buffer A pointer to the buffer that contains the data to write.
 * @param [in] nBytes The number of bytes to write to device.
 * @return The number of bytes actually written to device.
 *
 * Execute a write operation over the device pointed by device parameter.
 * If the write operation is not implemented by the device 0 is returned.
 * If any I/O error occurs during the write operation 0 is returned.
 * If device is NULL or buffer is NULL and the check parameters feature is
 * enable 0 is returned.
 * Otherwise the actually number of bytes written to device is returned.
 */
uint32_t BR_DeviceWrite(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes)
{
  uint32_t ret = 0U;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
  __BR_ASSERT(NULL != buffer);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != device) && (NULL != buffer))
#endif
  {
    if (NULL != device->write)
    {
      ret = device->write(device, address, buffer, nBytes);
    }
    else
    {
      ret = 0U;
    }
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = 0U;
  }
#endif

  return ret;
}

/**
 * @brief Execute a control command on a device.
 * @param [in] device A pointer to device.
 * @param [in] cmd The control command to execute.
 * @param [in] param Pointer to the command parameters.
 * @return Error code.
 * @retval E_OK If the command was successfully executed.
 * @retval E_ENOSYS If the control operation is not implemented for the device.
 * @retval E_INVAL If device is NULL and the check parameters feature is enabled.
 */
BR_Err_t BR_DeviceControl(BR_Device_t* device, uint8_t cmd, void* param)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != device)
#endif
  {
    if (NULL != device->control)
    {
      ret = device->control(device, cmd, param);
    }
    else
    {
      ret = E_ENOSYS;
    }
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/** @} */

/** @} */

