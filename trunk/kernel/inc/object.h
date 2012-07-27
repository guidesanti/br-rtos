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
 * @defgroup ModuleName The Module Name
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/

/**
 * @defgroup Includes Header File Include
 * @{
 */

#include "BR-RTOSConfig.h"
#include "list.h"
#include <stdint.h>

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

typedef enum
{
  BR_OBJ_TYPE_TASK = 0U,
  BR_OBJ_TYPE_DEVICE,
  BR_OBJ_TYPE_TIMER,
  BR_N_OBJ_TYPES,
} BR_ObjectType_t;

typedef struct
{
    uint16_t        id;
    BR_ObjectType_t type;
    void*           child;
    BR_ListNode_t   node;
    char            name[__BR_MAX_OBJ_NAME_LEN];
} BR_Object_t;

/**@}*/

/******************************************************************************/
/* P U B L I C  V A R I A B L E S                                             */
/******************************************************************************/

/**
 * @defgroup PublicVar Public Variables
 * @{
 */

/**@}*/


/******************************************************************************/
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

/**
 * @defgroup PublicFunc Public Functions
 * @{
 */

void __BR_ObjectInit(void);
BR_Object_t* __BR_ObjectCreate(char* name, BR_ObjectType_t type, void* child);
BR_Object_t* __BR_ObjectFind(uint16_t id);
BR_Object_t* __BR_ObjectFindByName(char* name);

/**@}*/

/**@}*/


#endif /* _OBJECT_H_ */
