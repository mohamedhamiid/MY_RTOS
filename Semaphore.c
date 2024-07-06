/*
 * semaphore.c
 *
 *  Created on: Jul 5, 2024
 *      Author: mh_sm
 */
#include "Task.h"
#include "FIFO.h"
#include "Semaphore.h"


OS_tenuSemaphoreState OS_enumInitSemaphore(OS_tstructSemaphore* Add_structSemaphore, u8 Copy_u8NoOfAvailable){
	Add_structSemaphore->s = Copy_u8NoOfAvailable ;
	Add_structSemaphore->noOfWaiting = 0;
	Add_structSemaphore->currentOwner = NULL;

	OS_enumFifoInit (&(Add_structSemaphore->Global_structWaitingQueue),Add_structSemaphore->Global_structWaitingQueueFIFO , 100);

	return OS_SEMAPHORE_INIT_OK;
}
OS_tenuSemaphoreState OS_enumAcquireSemaphore(OS_tstructSemaphore* Add_structSemaphore,OS_structTask*Add_structTask){
	Add_structSemaphore->s--;
	if(Add_structTask == Add_structSemaphore->currentOwner){
		return OS_SEMAPHORE_ALREADY_ACQUIRED;
	}
	if (Add_structSemaphore->s < 0 && Add_structSemaphore->currentOwner) {
		Add_structSemaphore->noOfWaiting++;
		OS_enumFifoEnqueue(&(Add_structSemaphore->Global_structWaitingQueue), Add_structTask);
		OS_enumTerminateTask(Add_structTask);
		return OS_SEMAPHORE_BUSY;
	}
	else{
		Add_structSemaphore->currentOwner = Add_structTask;
	}
	return OS_SEMAPHORE_AVAILABLE;
}
OS_tenuSemaphoreState OS_enumReleaseSemaphore(OS_tstructSemaphore* Add_structSemaphore){
	OS_structTask* loc_structDequeuedTask;
	Add_structSemaphore->s++ ;
	if (Add_structSemaphore->s <= 0) {
		Add_structSemaphore->noOfWaiting--;
		OS_enumFifoDequeue(&(Add_structSemaphore->Global_structWaitingQueue), &loc_structDequeuedTask);
		Add_structSemaphore->currentOwner = loc_structDequeuedTask;
		OS_enumActivateTask(loc_structDequeuedTask);
		return OS_SEMAPHORE_AVAILABLE;
	}
	return OS_SEMAPHORE_BUSY;
}
