/**
 * @file        AT25DB161D.h
 * @date        2012, August 28
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */


#ifndef _AT25DB161D_H_
#define _AT25DB161D_H_

/**
 * @addtogroup AT25DB161D_Device_Driver
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/*The AT25DB161D device control commands. */
#define AT25DB161D_IOCTL_PAGE_ERASE         (0U)  /**< Page erase. */
#define AT25DB161D_IOCTL_BLOCK_ERASE        (1U)  /**< Block erase. */
#define AT25DB161D_IOCTL_SEGMENT_ERASE      (2U)  /**< Segment erase. */
#define AT25DB161D_IOCTL_CHIP_ERASE         (3U)  /**< Chip erase. */
#define AT25DB161D_IOCTL_DEEP_POWER_DOWN    (4U)  /**< Enter deep power down mode. */
#define AT25DB161D_IOCTL_WAKE_UP            (5U)  /**< Wake up from deep power down mode. */
#define AT25DB161D_IOCTL_GET_MANUFACTURE_ID (6U)  /**< Get the manufacture ID. */


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/
void AT25DB161DInit(void);


/** @} */

#endif /* _AT25DB161D_H_ */
