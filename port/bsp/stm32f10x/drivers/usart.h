/**
 * @file        usart.h
 * @date        2012, August 03
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */


#ifndef _USART_H_
#define _USART_H_

/**
 * @addtogroup BSPstm32f10x BSP STM32F10x
 * @{
 */

/**
 * @addtogroup USART
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

#define USART_CMD_SET_BAUDRATE  (0U)
#define USART_CMD_SET_WORDLEN   (1U)
#define USART_CMD_SET_STOPBITS  (2U)
#define USART_CMD_SET_PARITY    (3U)
#define USART_CMD_SET_HW_FL     (4U)
#define USART_CMD_SET_MODE      (5U)


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/
void __BR_UsartInit(void);
void __BR_UsartIsr1(void);
void __BR_UsartIsr2(void);
void __BR_UsartIsr3(void);


/** @} */

/** @} */

#endif /* _USART_H_ */
