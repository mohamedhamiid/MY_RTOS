#include <string.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "Task.h"
#include "Semaphore.h"

#include "RCC_interface.h"

OS_structTask t1,t2,t3;
u8 Global_u8Task1Led,Global_u8Task2Led,Global_u8Task3Led;
void task1 (){
	while(1){
		Global_u8Task1Led ^= 1;
	}
}
void task2 (){
	while(1){
		Global_u8Task2Led ^= 1;
	}
}
void task3 (){
	while(1){
		Global_u8Task3Led ^= 1;
	}
}

/* Testing OS for same priorities tasks
 * Expected Output: Round Robin Scheduling with 1ms tick*/
int main(){

	// HW Init
	RCC_voidInitSysClock();

	// Variable to define error status
	OS_enumErrorStatus loc_enumERROR = OS_OK;

	OS_enumInit();
	if(loc_enumERROR != OS_OK)
		while(1);

	t1.func = task1;
	t1.Priority = 1;
	strcpy(t1.TaskName,"Task 1");
	t1.StackSize = 1024;

	loc_enumERROR = OS_enumCreateTask(&t1);
	if(loc_enumERROR != OS_OK)
		while(1);

	t2.func = task2;
	t2.Priority = 1;
	strcpy(t2.TaskName,"Task 2");
	t2.StackSize = 1024;

	loc_enumERROR = OS_enumCreateTask(&t2);
	if(loc_enumERROR != OS_OK)
		while(1);

	t3.func = task3;
	t3.Priority = 1;
	strcpy(t3.TaskName,"Task 3");
	t3.StackSize = 1024;

	loc_enumERROR = OS_enumCreateTask(&t3);
	if(loc_enumERROR != OS_OK)
		while(1);

	loc_enumERROR= OS_enumActivateTask(&t1);
	if(loc_enumERROR != OS_OK)
			while(1);
	loc_enumERROR= OS_enumActivateTask(&t2);
	if(loc_enumERROR != OS_OK)
			while(1);
	loc_enumERROR= OS_enumActivateTask(&t3);
	if(loc_enumERROR != OS_OK)
			while(1);

	OS_enumStartOS();

	while(1);


}
