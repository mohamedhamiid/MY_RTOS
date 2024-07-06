/*
 * Scheduler.h
 *
 *  Created on: Apr 9, 2024
 *      Author: mh_sm
 */

#ifndef INC_TASK_H_
#define INC_TASK_H_

#include "STD_TYPES.h"

typedef enum {
	noAutoStart,
	AutoStart
}OS_enumTaskAutoStart;



// Task Structure
typedef struct{
	/* Entered by user */
	u8 Priority;
	u8 TaskName[30] ;
	u16 StackSize;
	void (*func)(void); // Pointer to C function of the task
	OS_enumTaskAutoStart AutoStart ;
	struct{
		enum{
			OS_TASK_BLOCKING_DISABLE,
			OS_TASK_BLOCKING_ENABLE
		}Blocking;
		u32 TicksCount;
	}Waiting;
	/* Not entered by user */
	u32 _S_PSP_Task ; // Start of task stack
	u32 _E_PSP_Task ; // End   of task stack
	pu32 CurrentPSP ;
	enum{
		OS_TASK_SUSPEND,
		OS_TASK_WAITING,
		OS_TASK_READY,
		OS_TASK_RUNNING
	}TaskState;
}OS_structTask;

typedef enum {
	OS_OK  ,
	OS_EXCEED_AVAILABLE_STACK,
	FIFO_INIT_ERROR,
	TASK_CREATION_ERROR

}OS_enumErrorStatus;


/* API */
OS_enumErrorStatus OS_enumInit();
OS_enumErrorStatus OS_enumCreateTask(OS_structTask* Add_structTask);
OS_enumErrorStatus OS_enumActivateTask(OS_structTask* Add_structTask);
OS_enumErrorStatus OS_enumTerminateTask(OS_structTask* Add_structTask);
OS_enumErrorStatus OS_enumDelayTask(OS_structTask* Add_structTask,u32 Copy_u32NoOfTicks);
OS_enumErrorStatus OS_enumStartOS();


#endif /* INC_TASK_H_ */
