/*
 * EventGroup.c
 *
 *  Created on: Dec 28, 2024
 *      Author: mh_sm
 */

#include "FIFO.h"
#include "Task.h"
#include "EventGroup.h"



OS_tenuEventState OS_enumCreateEventGroup(OS_tstructEvent* Add_structEvent){
	Add_structEvent->bits = 0;
	OS_enumFifoInit (&(Add_structEvent->Global_structWaitingQueue),Add_structEvent->Global_structWaitingQueueFIFO , 100);
	return OS_EVENT_INIT_OK;
}

u16 OS_u16EventsWaitBits(OS_tstructEvent* Add_structEvent,u16 Copy_u16Bits, u8 Copy_u8WaitForAll,u8 Copy_u8ClearOnReturn , u8 Copy_u8TickToWait){

	u16 Loc_u8waitingBits = Copy_u16Bits;


	if(Copy_u8WaitForAll && (Add_structEvent->bits == Loc_u8waitingBits)){ // All are set.
		u16 Loc_u16ReturnedMask = Add_structEvent->bits;
		if(Copy_u8ClearOnReturn)
			Add_structEvent->bits = 0;
		return Loc_u16ReturnedMask;
	}

	if(!Copy_u8WaitForAll &&  Add_structEvent->bits && (Add_structEvent->bits <= Loc_u8waitingBits)){
		u16 Loc_u16ReturnedMask = Add_structEvent->bits;
		if(Copy_u8ClearOnReturn)
			Add_structEvent->bits = 0;
		return Loc_u16ReturnedMask;
	}

	if(Add_structEvent->bits && (Add_structEvent->bits > Loc_u8waitingBits)){
		if(Copy_u8ClearOnReturn)
			Add_structEvent->bits = 0;
		return OS_EVENT_NOT_SET; // Invalid
	}


	if(Copy_u8TickToWait){
		OS_structTask * Loc_structCurrentTask = OS_StructOS.CurrentTask;
		Add_structEvent->noOfWaiting++;
		OS_enumFifoEnqueue(&(Add_structEvent->Global_structWaitingQueue), &Loc_structCurrentTask);
		OS_enumDelayTask(Loc_structCurrentTask,Copy_u8TickToWait);
	}

	/* After Activation of task*/

	if(Copy_u8WaitForAll && (Add_structEvent->bits == Loc_u8waitingBits)){ // All are set.
		u16 Loc_u16ReturnedMask = Add_structEvent->bits;
		if(Copy_u8ClearOnReturn)
			Add_structEvent->bits = 0;
		return Loc_u16ReturnedMask;
	}

	if(!Copy_u8WaitForAll &&  Add_structEvent->bits && (Add_structEvent->bits <= Loc_u8waitingBits)){
		u16 Loc_u16ReturnedMask = Add_structEvent->bits;
		if(Copy_u8ClearOnReturn)
			Add_structEvent->bits = 0;
		return Loc_u16ReturnedMask;
	}

	if(Add_structEvent->bits && (Add_structEvent->bits > Loc_u8waitingBits)){
		if(Copy_u8ClearOnReturn)
			Add_structEvent->bits = 0;
		return OS_EVENT_NOT_SET; // Invalid
	}

	if(Copy_u8ClearOnReturn)
		Add_structEvent->bits = 0;

	return OS_EVENT_NOT_SET;

}

OS_tenuEventState OS_enumSetEvent(OS_tstructEvent* Add_structEvent,u16 Copy_u16Bits){

	Add_structEvent->bits |= Copy_u16Bits;

	//OS_enumActivateMultipleTasks(&(Add_structEvent->Global_structWaitingQueue),Add_structEvent->noOfWaiting);

	Add_structEvent->noOfWaiting = 0;

	return OS_EVENT_SET;
}


