/**
 * @file        object.h
 * @date        2012, July 26
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: add brief description
 *
 * TODO: add detailed description
 */


#ifndef _OBJECT_H_
#define _OBJECT_H_

/**
 * @addtogroup ObjectCtrl Object Control
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOSDefs.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * @name Constants, Macros and Type definitions
 * @{
 */

/** @} */


/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/
void __BR_ObjectInit(void);
BR_Object_t* __BR_ObjectCreate(char* name, BR_ObjectType_t type, void* child);
BR_Object_t* __BR_ObjectFind(uint16_t id);
BR_Object_t* __BR_ObjectFindByName(char* name);

/** @} */


#endif /* _OBJECT_H_ */
