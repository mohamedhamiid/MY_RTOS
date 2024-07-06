/**********************************************************/
/* Author  : Mohamed Abdel Hamid                          */
/* Date    : 7 / 6 / 2024                             	  */
/* Version : V01                                          */
/* Email   : mohamedhamiid20@gmail.com                    */
/* Brief   : Handling system structures                   */
/**********************************************************/
#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "STD_TYPES.h"
#include "Task.h"
#include "System.h"

// Macro: OS_STACK_PADDING
// Description: Defines the padding size (in bytes) used between the end of one stack allocation
//              and the start of another. Ensures alignment between different stack regions or tasks.
#define OS_STACK_PADDING 8

// Macro: OS_REQUEST_SERVICE(SVC_ID)
// Description: Initiates a request for a specific service call (SVC) identified by SVC_ID.
//              Uses inline assembly to generate a Supervisor Call (SVC) instruction with the
//              provided SVC_ID as an immediate operand. This triggers an SVC exception,
//              allowing the operating system to handle the requested service.
#define OS_REQUEST_SERVICE(SVC_ID)  __asm volatile ("SVC %[SVCid]" : : [SVCid] "i" (SVC_ID));

/**
 * @brief Structure defining the operating system (OS) attributes.
 */
extern struct{
	u8  NoOfCreatedTasks;
	u32 _S_MSP_Task ;               // Start of main(OS) stack
	u32 _E_MSP_Task ; 	            // End   of main(OS) stack
	u32 PSP_LastEnd ;               // End of Last PSP allocated
	enum{
		OS_SUSPEND,
		OS_RUNNING
	}OS_enumMode;
	OS_structTask* CurrentTask;
	OS_structTask* NextTask;
	OS_structTask* TaskTable[100] ; // Table of all tasks of System

}OS_StructOS;

/**
 * @brief Enumeration defining the Service Call (SVC) IDs for the operating system.
 */
extern enum{
	SVC_ACTIVATE      ,
	SVC_TERMINATE     ,
	SVC_WAITING       ,
	SVC_SUSPEND       ,
	SVC_ACQUIRE_MUTEX ,
	SVC_RELEASE_MUTEX
}OS_enumSvcID;

void OS_enumUpdateNoOfTicks();










#endif /* INC_SYSTEM_H_ */
