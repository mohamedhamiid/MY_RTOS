/*
 * Scheduler_priv.h
 *
 *  Created on: Apr 12, 2024
 *      Author: mh_sm
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#include "STD_TYPES.h"
#include "Task.h"
#include "System.h"

#define OS_STACK_PADDING 8

// OS Structure
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

// SVC IDs
extern enum{
	SVC_ACTIVATE      ,
	SVC_TERMINATE     ,
	SVC_WAITING       ,
	SVC_SUSPEND       ,
	SVC_ACQUIRE_MUTEX ,
	SVC_RELEASE_MUTEX
}OS_enumSvcID;

// OS
#define OS_REQUEST_SERVICE(SVC_ID)  __asm volatile ("SVC %[SVCid]" : : [SVCid] "i" (SVC_ID));

void OS_enumUpdateNoOfTicks();










#endif /* INC_SYSTEM_H_ */
