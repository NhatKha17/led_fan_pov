/**
 * @file       sys_manage.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       01/04/2024
 * @author     Kha Nguyen
 *
 * @brief      system manage to handle flow activity project
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_SYSTEM_SYS_MANAGE_H_
#define INC_SYSTEM_SYS_MANAGE_H_
/* Includes ----------------------------------------------------------- */
#include "sys_display.h"
#include "sys_rtc.h"
#include "sys_uart.h"
/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
	SYS_MANAGE_OK = 0U,
	SYS_MANGAE_ERROR,
} sys_manage_status_t;
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief   System management loop function.
 *
 * @details This function is responsible for managing system tasks and operations in a loop.
 *          It typically handles tasks such as scheduling, monitoring, and responding to events
 *          or conditions within the system. It may involve coordinating various subsystems,
 *          managing resources, handling interrupts, and ensuring the overall stability and
 *          functionality of the system.
 *
 * @note    This function is expected to run continuously to maintain system functionality.
 *          It may interact with other modules or functions within the system to perform
 *          specific tasks and respond to system events.
 *
 * @return  None
 */
void sys_manage_loop(void);
#endif /* INC_SYSTEM_SYS_MANAGE_H_ */
	   /* End of file -------------------------------------------------------- */
