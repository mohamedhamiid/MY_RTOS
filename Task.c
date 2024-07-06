/*
 * Scheduler.c
 *
 *  Created on: Apr 9, 2024
 *      Author: mh_sm
 */
#include <string.h>
#include <STD_TYPES.h>
#include "Task.h"
#include "System.h"
#include "Mem_Management.h"
#include "FIFO.h"
#include "Scheduler.h"
#include "Porting_CortexM.h"
#include "MyRTOSConfig.h"
extern OS_structTask Global_structIdleTask;
extern OS_tBuffer Global_structReadyQueue;
extern OS_structTask* Global_structReadyQueueFIFO[100];


OS_enumErrorStatus OS_enumCreateTask(OS_structTask* Add_structTask){
	OS_enumErrorStatus Error = OS_OK;

	// Allocate stack
	// Check it's not exceeding the PSP Stack
	Add_structTask->_S_PSP_Task = OS_StructOS.PSP_LastEnd;
	Add_structTask->_E_PSP_Task = Add_structTask->_S_PSP_Task - Add_structTask->StackSize;
	/* Check not done yet */

	// Align 8 bytes padding between the task and others
	OS_StructOS.PSP_LastEnd = Add_structTask->_E_PSP_Task - OS_STACK_PADDING ;

	// Create Stack
	OS_enumCreateStack(Add_structTask);

	// Add task to Scheduler table (Waiting Queue)
	OS_StructOS.TaskTable[OS_StructOS.NoOfCreatedTasks++] = Add_structTask;
	// Task State Update --> Suspend
	Add_structTask->TaskState = OS_TASK_SUSPEND;

	return Error;
}


OS_enumErrorStatus OS_enumActivateTask(OS_structTask* Add_structTask){
	/* Change State from Suspend to Waiting */
	Add_structTask->TaskState = OS_TASK_WAITING ;

	/* Request activate to SVC  */
	OS_REQUEST_SERVICE(SVC_ACTIVATE);
	return OS_OK;

}
OS_enumErrorStatus OS_enumTerminateTask(OS_structTask* Add_structTask){
	/* Change State from Suspend to Waiting */
	Add_structTask->TaskState = OS_TASK_SUSPEND ;
	/* Request terminate to SVC  */
	OS_REQUEST_SERVICE(SVC_TERMINATE);
	return OS_OK;
}

OS_enumErrorStatus OS_enumDelayTask(OS_structTask* Add_structTask,u32 Copy_u32NoOfTicks){
	Add_structTask->Waiting.Blocking = OS_TASK_BLOCKING_ENABLE;
	Add_structTask->Waiting.TicksCount = Copy_u32NoOfTicks;
	OS_enumTerminateTask(Add_structTask);
	return OS_OK;
}



OS_enumErrorStatus OS_enumInit(){
	OS_enumErrorStatus Error = OS_OK;

	OS_voidHwInit();
	// Update OS mode
	OS_StructOS.OS_enumMode = OS_SUSPEND;

	// Assign Main Stack
	Error += OS_enumCreateMainStack();

	// Create Ready Queue
	if(OS_enumFifoInit(&Global_structReadyQueue, Global_structReadyQueueFIFO, 100)!=FIFO_NO_ERROR){
		Error+=FIFO_INIT_ERROR;
	}

	// Idle Task
	strcpy(Global_structIdleTask.TaskName , "IDLE");
	Global_structIdleTask.Priority = OS_LOWEST_PRIORITY;
	Global_structIdleTask.func = OS_voidIdleTask;
	Global_structIdleTask.StackSize = 300 ;
	Error += OS_enumCreateTask(&Global_structIdleTask);

	// Cr

	return Error;
}


OS_enumErrorStatus OS_enumStartOS(){
	// 1- Set OS to Running
	OS_StructOS.OS_enumMode = OS_RUNNING ;
	// 2- By default run IDLE task
	OS_StructOS.CurrentTask = &Global_structIdleTask;
	// 3- Activate task
	// It won't be activated according to the condition in OS_enumActivateTask
	OS_enumActivateTask(&Global_structIdleTask);
	// 4- Start Timer
	OS_voidStartTimer();
	// 5- Set PSP to the IDLE Task
	OS_SET_PSP(OS_StructOS.CurrentTask->CurrentPSP);
	OS_SWITCH_TO_PSP();
	OS_SWITCH_TO_NOT_PRIVELEGE();
	OS_StructOS.CurrentTask->func();
	return OS_OK;

}






