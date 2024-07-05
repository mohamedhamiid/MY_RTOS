/*
 * Mem_Management.c
 *
 *  Created on: Jul 5, 2024
 *      Author: mh_sm
 */
#include "System.h"
#include "Task.h"
#include "Mem_Management.h"

extern u32 _estack ;

OS_enumErrorStatus OS_enumCreateStack(OS_structTask* Add_structTask){
	OS_enumErrorStatus Error = OS_OK;

	/* Task Frame
	 * XPSR
	 * PC
	 * LR
	 * R12
	 * R4
	 * R3
	 * R2
	 * R1
	 * R0
	 *
	 * And we will add some values MANUALLY
	 * R5
	 * R6
	 * R7
	 * R8
	 * R9
	 * R10
	 * R11 */

	Add_structTask->CurrentPSP = Add_structTask->_S_PSP_Task;

	/* We will add dummy values for these */

	// XPSR --> T bit (bit24) must be 1
	Add_structTask->CurrentPSP--;
	*(Add_structTask->CurrentPSP) = 0x01000000;

	// PC
	Add_structTask->CurrentPSP--;
	*(Add_structTask->CurrentPSP) = (u32)Add_structTask->func;

	// LR --> 0xFFFFFFED Thread + PSP
	Add_structTask->CurrentPSP--;
	*(Add_structTask->CurrentPSP) = 0xFFFFFFFD;


	for(u32 i=0 ; i<13 ; i++){
		Add_structTask->CurrentPSP--;
		*(Add_structTask->CurrentPSP) = 0;
	}

	return Error;
}

OS_enumErrorStatus OS_enumCreateMainStack(){

	OS_enumErrorStatus error = OS_OK ;
	OS_StructOS._S_MSP_Task = &_estack;
	OS_StructOS._E_MSP_Task = OS_StructOS._S_MSP_Task - OS_MAIN_STACK_SIZE;
	// Align 8 bytes between Main stack and first task
	OS_StructOS.PSP_LastEnd = (OS_StructOS._E_MSP_Task-OS_STACK_PADDING);

	return error;
}
