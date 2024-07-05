/*
 * Scheduler.c
 *
 *  Created on: Jul 5, 2024
 *      Author: mh_sm
 */
#include <string.h>
#include "STD_TYPES.h"
#include "Task.h"
#include "System.h"
#include "FIFO.h"
#include "Porting_CortexM.h"
#include "Scheduler.h"
OS_tBuffer Global_structReadyQueue;
OS_structTask* Global_structReadyQueueFIFO[100];
OS_structTask Global_structIdleTask;
void OS_voidSortSchedulerTable(){

	for(u8 i = 0 ; i < OS_StructOS.NoOfCreatedTasks-1 ; i++)
		for(u8 j = 0 ; j < OS_StructOS.NoOfCreatedTasks - i - 1 ; j++)
			if(OS_StructOS.TaskTable[j]->Priority > OS_StructOS.TaskTable[j+1]->Priority){
				OS_structTask * temp ;
				temp = OS_StructOS.TaskTable[j] ;
				OS_StructOS.TaskTable[j] =  OS_StructOS.TaskTable[j+1];
				OS_StructOS.TaskTable[j+1] = temp ;
			}

}

void OS_voidUpdateReadyQueue(){

	OS_structTask* temp = NULL ;
    // 1- Free ready queue
    while(OS_enumFifoDequeue(&Global_structReadyQueue, &temp) != FIFO_EMPTY);

    // 2- Update ready queue
    for(u8 i = 0; i < OS_StructOS.NoOfCreatedTasks; i++){
        // Check if the task is not suspended
        if(OS_StructOS.TaskTable[i]->TaskState != OS_TASK_SUSPEND){
            // Enqueue the task into the ready queue
            OS_enumFifoEnqueue(&Global_structReadyQueue, OS_StructOS.TaskTable[i]);
            // Update task state to ready
            OS_StructOS.TaskTable[i]->TaskState = OS_TASK_READY;
            // Break if it's the last task or if the next task has a different priority
            if((i == OS_StructOS.NoOfCreatedTasks - 1) || (OS_StructOS.TaskTable[i]->Priority != OS_StructOS.TaskTable[i + 1]->Priority))
                break;
        }

    }
}

/* In this function we decide the nextTask and make it running
 * And make the current task ready */
void OS_voidDecideNext(){
	/* 1- Check if ready queue is not empty and current task running CPU
	 * is not suspend then maintain current task running */
	if(Global_structReadyQueue.counter == 0 && OS_StructOS.CurrentTask->TaskState != OS_TASK_SUSPEND){
		// Mark the current task running
		OS_StructOS.CurrentTask->TaskState = OS_TASK_RUNNING;
		// Enqueue the current task
		OS_enumFifoEnqueue(&Global_structReadyQueue, OS_StructOS.CurrentTask);
		// Make the next
		OS_StructOS.NextTask->TaskState = OS_StructOS.CurrentTask->TaskState;
	}
	else{
		// Get the next task from queue
		OS_enumFifoDequeue(&Global_structReadyQueue, &OS_StructOS.NextTask);
		// make it's state running
		OS_StructOS.NextTask->TaskState = OS_TASK_RUNNING ;
		/* Before enqueue the current task make sure that it has the same priority as next task
		    to maintain round robin */
		if((OS_StructOS.NextTask->Priority == OS_StructOS.CurrentTask->Priority)&&OS_StructOS.CurrentTask->TaskState!=OS_TASK_SUSPEND){
			// enqueue the current task to ready queue
			OS_enumFifoEnqueue(&Global_structReadyQueue, OS_StructOS.CurrentTask);
			// make it's state ready
			OS_StructOS.CurrentTask->TaskState = OS_TASK_READY ;
		}
	}
}
/* Called by SVC handler (Handler Mode) to execute
 * what should be done in each service*/
void OS_voidSvcServices(pu32 Add_u32StackFrame){
	u8 SVC_ID = *((u8*)(((u8*)Add_u32StackFrame[6])-2));
	switch(SVC_ID){
		case SVC_ACTIVATE:/* Activate Task */
		case SVC_TERMINATE:/* Terminate Task */
			/* 1- Update waiting queue (scheduler table) */
			OS_voidSortSchedulerTable();
			/* 2- Update ready queue */
			OS_voidUpdateReadyQueue();

			if(OS_StructOS.OS_enumMode == OS_RUNNING){
				if(strcmp(OS_StructOS.CurrentTask->TaskName,"IDLE")!=0){
					/* 3- What next? */
					OS_voidDecideNext();
					/* 4- Trigger PendSV */
					OS_TRIGGER_PENDSV();

				}
			}
		break;
		case SVC_WAITING:/* Suspend Task */
			/* 1- Update waiting queue (scheduler table) */
			OS_voidSortSchedulerTable();
			/* 2- Update ready queue */
			OS_voidUpdateReadyQueue();


		break;
		case SVC_SUSPEND:/* Suspend Task */
		break;
	}

}

void OS_enumUpdateNoOfTicks(){
	for(u8 i=0 ; i<OS_StructOS.NoOfCreatedTasks;i++){
		if(OS_StructOS.TaskTable[i]->Waiting.Blocking == OS_TASK_BLOCKING_ENABLE){
			OS_StructOS.TaskTable[i]->Waiting.TicksCount--;
			if(OS_StructOS.TaskTable[i]->Waiting.TicksCount == 0){
				OS_StructOS.TaskTable[i]->TaskState = OS_TASK_WAITING;
				OS_StructOS.TaskTable[i]->Waiting.Blocking = OS_TASK_BLOCKING_DISABLE;
			}
		}
	}
	/* Request waiting to SVC to update scheduler table */
	OS_REQUEST_SERVICE(SVC_TERMINATE);
}

uint8_t Global_u8IdleTaskLed;
void OS_voidIdleTask(){
	while(1){
		Global_u8IdleTaskLed^=1;  // For testing
//		__asm("NOP");
		__asm("WFE"); // CPU will enter sleep mode
	}
}

