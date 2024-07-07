//#include <string.h>
//#include "STD_TYPES.h"
//#include "BIT_MATH.h"
//#include "Task.h"
//#include "Semaphore.h"
//
//#include "RCC_interface.h"
//
//OS_tstructSemaphore s1,s2;
//OS_structTask t1,t2;
//u8 Global_u8Task1Led,Global_u8Task2Led;
//void task1 (){
//	static u8 count = 0;
//	while(1){
//		Global_u8Task1Led ^= 1;
//		count++;
//		if(count == 100){
//			OS_enumAcquireSemaphore(&s1, &t1);
//			OS_enumActivateTask(&t2);
//			OS_enumAcquireSemaphore(&s2, &t1);
//		}
//		if(count == 300){
//			count = 0;
//			OS_enumReleaseSemaphore(&s1);
//		}
//
//	}
//}
//void task2 (){
//	static u8 count = 0;
//	while(1){
//		Global_u8Task2Led ^= 1;
//		count++;
//		if(count == 200){
//			OS_enumAcquireSemaphore(&s2, &t2);
//			OS_enumAcquireSemaphore(&s1, &t2);
//		}
//		if(count == 300){
//			OS_enumTerminateTask(&t2);
//			count = 0;
//		}
//	}
//}
//
///* Testing OS for Deadlock
// * Expected Output: Deadlock will happen and Idle task will happen */
//int main(){
//
//	// HW Init
//	RCC_voidInitSysClock();
//
//	// Variable to define error status
//	OS_enumErrorStatus loc_enumERROR = OS_OK;
//
//	OS_enumInit();
//	if(loc_enumERROR != OS_OK)
//		while(1);
//
//	OS_enumInitSemaphore(&s1, 1);
//	OS_enumInitSemaphore(&s2, 1);
//
//	t1.func = task1;
//	t1.Priority = 4 ;
//	strcpy(t1.TaskName,"Task 1");
//	t1.StackSize = 1024;
//
//	loc_enumERROR = OS_enumCreateTask(&t1);
//	if(loc_enumERROR != OS_OK)
//		while(1);
//
//	t2.func = task2;
//	t2.Priority = 3 ;
//	strcpy(t2.TaskName,"Task 2");
//	t2.StackSize = 1024;
//
//	loc_enumERROR = OS_enumCreateTask(&t2);
//	if(loc_enumERROR != OS_OK)
//		while(1);
//
//
//	loc_enumERROR= OS_enumActivateTask(&t1);
//	if(loc_enumERROR != OS_OK)
//			while(1);
//
//	OS_enumStartOS();
//
//	while(1);
//
//
//}
//
