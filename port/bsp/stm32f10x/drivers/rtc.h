/**
 * @file        rtc.h
 * @date        2012, August 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */


#ifndef _RTC_H_
#define _RTC_H_

/**
 * @addtogroup RTC
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include <stdint.h>


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/* RTC control commands. */
#define RTC_CMD_GET_TIME            (0U)
#define RTC_CMD_SET_TIME            (1U)
#define RTC_CMD_SET_ALARM_TIME      (2U)
#define RTC_CMD_SET_ALARM_ENABLED   (3U)
#define RTC_CMD_SET_ALARM_DISABLED  (4U)
#define RTC_CMD_SET_ALARM_CALLBACK  (5U)

typedef uint32_t BR_RtcTime_t;


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/
void __BR_RtcInit(void);
void __BR_RtcIsr(void);


/** @} */

#endif /* _RTC_H_ */
