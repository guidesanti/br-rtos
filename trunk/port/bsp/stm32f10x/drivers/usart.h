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

/* USART control commands. */
#define USART_CMD_SET_BAUDRATE      (0U)
#define USART_CMD_SET_WORDLEN       (1U)
#define USART_CMD_SET_STOPBITS      (2U)
#define USART_CMD_SET_PARITY        (3U)
#define USART_CMD_SET_HW_FL         (4U)

/* Word length. */
#define USART_WORD_LEN_8            (0U)
#define USART_WORD_LEN_9            (1U)

/* Stop bits. */
#define USART_STOP_BITS_0_5         (0U)
#define USART_STOP_BITS_1           (1U)
#define USART_STOP_BITS_1_5         (2U)
#define USART_STOP_BITS_2           (3U)

/* Parity. */
#define USART_PARITY_NO             (0U)
#define USART_PARITY_EVEN           (1U)
#define USART_PARITY_ODD            (2U)

/* Hardware flow control. */
#define USART_HW_FLOW_CTRL_NONE     (0U)
#define USART_HW_FLOW_CTRL_RTS      (1U)
#define USART_HW_FLOW_CTRL_CTS      (2U)
#define USART_HW_FLOW_CTRL_RTS_CTS  (3U)


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
