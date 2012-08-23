/**
 * @file        object.c
 * @date        2012, July 26
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: Add brief description
 *
 * TODO: Add detailed description
 */


/**
 * @addtogroup ObjectCtrl Object Control
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOS.h"
#include "object.h"
#include "list.h"
#include "port.h"
#include <string.h>


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

static BR_ListNode_t objList[BR_N_OBJ_TYPES];

/** @} */

/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/

/**
 * @name Kernel Internal Functions
 * @{
 */

/** @} */

/******************************************************************************/
/* P U B L I C  A P I  F U N C T I O N S                                      */
/******************************************************************************/

/**
 * @name Public API Functions
 * @{
 */

void __BR_ObjectStartUpInit(void)
{
  uint8_t index = 0U;

  for (index = 0U; index < BR_N_OBJ_TYPES; index++)
  {
    __BR_ListInit(&(objList[index]));
  }
}

/**
 * @brief Initialize a kernel object.
 * @param [in] object A pointer to the object.
 */
void __BR_ObjectInit(BR_Object_t* object, BR_ObjectType_t type, const char* name)
{
  __BR_ASSERT(NULL != object);
  __BR_ASSERT(type < BR_N_OBJ_TYPES);

  /* Set the object type. */
  object->type = type;
  /* Initialize the object list node. */
  __BR_ListInit(&(object->node));
  /* Set the object name. */
  strncpy(object->name, name, __BR_MAX_OBJ_NAME_LEN);
  object->name[__BR_MAX_OBJ_NAME_LEN - 1U] = '\0';

  /* Insert the object within the object list. */
  __BR_ENTER_CRITICAL();
  __BR_ListInsertBefore(&(objList[type]), &(object->node));
  __BR_EXIT_CRITICAL();
}

BR_Object_t* __BR_ObjectFind(const char* name)
{
  BR_Object_t* obj = NULL;
  BR_ListNode_t* node = NULL;
  uint8_t index = 0U;

  for (index = 0U; index < BR_N_OBJ_TYPES; index++)
  {
    node = objList[index].next;
    while (node != &(objList[index]))
    {
      obj = __BR_LIST_ENTRY(node, BR_Object_t, node);
      if (0U == strncmp(name, obj->name, __BR_MAX_OBJ_NAME_LEN))
      {
        return obj;
      }
      node = node->next;
    }
  }

  return NULL;
}

/** @} */

/** @} */
