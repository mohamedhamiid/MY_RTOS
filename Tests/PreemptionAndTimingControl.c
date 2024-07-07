//#include <string.h>
//#include "STD_TYPES.h"
//#include "BIT_MATH.h"
//#include "Task.h"
//#include "Semaphore.h"
//
//#include "RCC_interface.h"
//OS_structTask t1,t2,t3,t4;
//u8 Global_u8Task1Led,Global_u8Task2Led,Global_u8Task3Led,Global_u8Task4Led;
//void task1 (){
//	while(1){
//		Global_u8Task1Led ^= 1;
//		OS_enumDelayTask(&t1, 100);
//
//	}
//}
//void task2 (){
//	while(1){
//		Global_u8Task2Led ^= 1;
//		OS_enumDelayTask(&t2, 100);
//	}
//}
//void task3 (){
//	static u16 count = 0;
//	while(1){
//		Global_u8Task3Led ^= 1;
//		count++;
//		if(count == 30){
//			count=0;
//			OS_enumActivateTask(&t4);
//		}
//		OS_enumDelayTask(&t3, 500);
//	}
//}
//void task4 (){
//	static u16 count = 0;
//	while(1){
//		Global_u8Task4Led ^= 1;
//		OS_enumDelayTask(&t4, 1000);
//
//	}
//}
//
///* Testing OS for:
// * - Preemption by activating task which has higher priority
// * - Timing control such that each task will be executed every
// * specific time for example every 2ms
// * Expected Output: Round Robin Scheduling with 1ms tick*/
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
//	t1.func = task1;
//	t1.Priority = 2;
//	strcpy(t1.TaskName,"Task 1");
//	t1.StackSize = 1024;
//
//	loc_enumERROR = OS_enumCreateTask(&t1);
//	if(loc_enumERROR != OS_OK)
//		while(1);
//
//	t2.func = task2;
//	t2.Priority = 2;
//	strcpy(t2.TaskName,"Task 2");
//	t2.StackSize = 1024;
//
//	loc_enumERROR = OS_enumCreateTask(&t2);
//	if(loc_enumERROR != OS_OK)
//		while(1);
//
//	t3.func = task3;
//	t3.Priority = 2;
//	strcpy(t3.TaskName,"Task 3");
//	t3.StackSize = 1024;
//
//	loc_enumERROR = OS_enumCreateTask(&t3);
//	if(loc_enumERROR != OS_OK)
//		while(1);
//
//	t4.func = task4;
//	t4.Priority = 2;
//	strcpy(t4.TaskName,"Task 4");
//	t4.StackSize = 1024;
//
//	loc_enumERROR = OS_enumCreateTask(&t4);
//	if(loc_enumERROR != OS_OK)
//		while(1);
//
//	loc_enumERROR= OS_enumActivateTask(&t1);
//	if(loc_enumERROR != OS_OK)
//			while(1);
//	loc_enumERROR= OS_enumActivateTask(&t2);
//	if(loc_enumERROR != OS_OK)
//			while(1);
//	loc_enumERROR= OS_enumActivateTask(&t3);
//	if(loc_enumERROR != OS_OK)
//			while(1);
//
//	OS_enumStartOS();
//
//	while(1);
//
//
//}
