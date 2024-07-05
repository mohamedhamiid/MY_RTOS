/*
 * CortexM_OS_Porting.h
 *
 *  Created on: Apr 9, 2024
 *      Author: mh_sm
 */

#ifndef INC_CORTEXM_OS_PORTING_H_
#define INC_CORTEXM_OS_PORTING_H_

#include "STD_TYPES.h"
#include "stm32f103xb.h"
#include "core_cm3.h"


#define OS_SET_PSP(add)   		     __asm volatile("MOV R0,%[stackValue] \n\t MSR PSP , R0": :[stackValue] "r" (add))
#define OS_GET_PSP(add)   		     __asm volatile("MRS R0 , PSP \n\t MOV %[stackReturnedValue] , R0":[stackReturnedValue]"=r"(add))
#define OS_SWITCH_TO_PSP()  	     __asm("MRS R3 , CONTROL \n\t ORR R3 , R3 , #0x2 \n\t MSR CONTROL , R3")
#define OS_SWICTH_TO_MSP()           __asm("MRS R3 , CONTROL \n\t BIC R3 , R3 , #0x2 \n\t MSR CONTROL , R3")
#define OS_SWITCH_TO_PRIVELEGE()     __asm("MRS R3 , CONTROL \n\t BIC R3 , R3 , #0x1 \n\t MSR CONTROL , R3")
#define OS_SWITCH_TO_NOT_PRIVELEGE() __asm("MRS R3 , CONTROL \n\t ORR R3 , R3 , #0x1 \n\t MSR CONTROL , R3")

#define OS_TRIGGER_PENDSV()           SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

void OS_voidHwInit();
void OS_voidStartTimer();
#endif /* INC_CORTEXM_OS_PORTING_H_ */
