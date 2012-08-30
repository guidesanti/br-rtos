/**
 * @file        timer.h
 * @date        2012, July 27
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: add brief description
 *
 * TODO: add detailed description
 */


#ifndef _TIMER_H_
#define _TIMER_H_


/**
 * @addtogroup TimerCtrl Timer Control
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/
extern BR_Task_t* timerTask;

/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/
void __BR_TimerStartUpInit(void);
void __BR_TimerDecrement(void);
void __BR_TimerTask(void);

/** @} */

#endif /* _TIMER_H_ */
