/********************************************************************/
/* Author  : Mohamed Abdel Hamid                                    */
/* Date    : 7 / 6 / 2024                             	            */
/* Version : V01                                                    */
/* Email   : mohamedhamiid20@gmail.com                              */
/* Brief   : Handling Task creation, termination, waiting and etc.  */
/********************************************************************/
#include <string.h>
#include <STD_TYPES.h>
#include "FIFO.h"
#include "System.h"
#include "Mem_Management.h"
#include "Scheduler.h"
#include "Porting_CortexM.h"
#include "MyRTOSConfig.h"
#include "Task.h"
extern OS_structTask Global_structIdleTask;

extern OS_tBuffer Global_structReadyQueue[OS_TASK_PRIORITY_LEVELS]; // 8 * 32 = 256
extern OS_structTask* Global_structReadyQueueFIFO[OS_TASK_PRIORITY_LEVELS][5];
/** OS_enumCreateTask
 * @brief Creates a new task and initializes its stack and state.
 *
 * This function creates a new task by allocating and configuring its stack,
 * adding it to the scheduler table (TaskTable), and updating its state to suspended.
 *
 * @param Add_structTask Pointer to the task structure to create and initialize.
 * @return OS_enumErrorStatus Returns OS_OK indicating successful task creation.
 *
 * @details
 * The function performs the following steps to create a task:
 * 1. Allocates stack space for the task and checks it does not exceed the PSP stack limit.
 * 2. Aligns the stack space with an 8-byte padding to ensure proper memory alignment.
 * 3. Creates the stack using `OS_enumCreateStack`.
 * 4. Adds the task to the scheduler table (`TaskTable`) of the operating system.
 * 5. Increments the count of created tasks (`NoOfCreatedTasks`).
 * 6. Sets the initial state of the task to suspended (`TaskState = OS_TASK_SUSPEND`).
 *
 * Example usage:
 * @code
 * OS_structTask t1;
 * t1.func = task1;
 * t1.Priority = 1;
 * strcpy(t1.TaskName,"Task 1");
 * t1.StackSize = 1024;
 *
 * loc_enumERROR = OS_enumCreateTask(&t1);
 * if(loc_enumERROR != OS_OK)
 * 	 while(1);
 * @endcode
 */
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
/** OS_enumActivateTask
 * @brief Activates a suspended task, changing its state to waiting.
 *
 * This function activates a suspended task by changing its state from suspended
 * (`OS_TASK_SUSPEND`) to waiting (`OS_TASK_WAITING`). It then requests activation
 * of the task via a service call to the operating system.
 *
 * @param Add_structTask Pointer to the task structure to activate.
 * @return OS_enumErrorStatus Returns OS_OK indicating successful task activation.
 *
 * @details
 * The function performs the following steps to activate a task:
 * 1. Changes the state of the task from suspended to waiting (`TaskState = OS_TASK_WAITING`).
 * 2. Requests activation of the task by calling `OS_REQUEST_SERVICE(SVC_ACTIVATE)`.
 *
 * Example usage:
 * @code
 * t1.func = task1;
 * t1.Priority = 2;
 * strcpy(t1.TaskName,"Task 1");
 * t1.StackSize = 1024;
 *
 * loc_enumERROR = OS_enumCreateTask(&t1);
 * if(loc_enumERROR != OS_OK)
 * 	while(1);
 *
 * loc_enumERROR= OS_enumActivateTask(&t1);
 * if(loc_enumERROR != OS_OK)
 * 		while(1);
 *
 * // Activates 'myTask' by changing its state to waiting and requesting activation via SVC.
 * @endcode
 */
OS_enumErrorStatus OS_enumActivateTask(OS_structTask* Add_structTask){
	/* Change State from Suspend to Waiting */
	Add_structTask->TaskState = OS_TASK_WAITING ;

	/* Request activate to SVC  */
	OS_REQUEST_SERVICE(SVC_ACTIVATE);
	return OS_OK;

}
/** OS_enumTerminateTask
 * @brief Terminates a task by changing its state to suspended.
 *
 * This function terminates a task by changing its state from any state to suspended
 * (`OS_TASK_SUSPEND`). It then requests termination of the task via a service call
 * to the operating system.
 *
 * @param Add_structTask Pointer to the task structure to terminate.
 * @return OS_enumErrorStatus Returns OS_OK indicating successful task termination.
 *
 * @details
 * The function performs the following steps to terminate a task:
 * 1. Changes the state of the task to suspended (`TaskState = OS_TASK_SUSPEND`).
 * 2. Requests termination of the task by calling `OS_REQUEST_SERVICE(SVC_TERMINATE)`.
 *
 * Example usage:
 * @code
 * OS_structTask myTask;
 * OS_enumTerminateTask(&myTask);
 * // Terminates 'myTask' by changing its state to suspended and requesting termination via SVC.
 * @endcode
 */
OS_enumErrorStatus OS_enumTerminateTask(OS_structTask* Add_structTask){
	/* Change State from Suspend to Waiting */
	Add_structTask->TaskState = OS_TASK_SUSPEND ;
	/* Request terminate to SVC  */
	OS_REQUEST_SERVICE(SVC_TERMINATE);
	return OS_OK;
}
/**
 * @brief Delays a task by enabling blocking and setting a tick count.
 *
 * This function delays a task by enabling blocking (`OS_TASK_BLOCKING_ENABLE`)
 * and setting the number of ticks (`TicksCount`) for which the task should wait.
 * It then terminates the task via `OS_enumTerminateTask`.
 *
 * @param Add_structTask Pointer to the task structure to delay.
 * @param Copy_u32NoOfTicks Number of ticks to delay the task.
 * @return OS_enumErrorStatus Returns OS_OK indicating successful task delay.
 *
 * @details
 * The function performs the following steps to delay a task:
 * 1. Sets blocking enable flag for the task (`Blocking = OS_TASK_BLOCKING_ENABLE`).
 * 2. Sets the number of ticks (`TicksCount`) for the task to wait.
 * 3. Terminates the task using `OS_enumTerminateTask`.
 *
 * Example usage:
 * @code
 * OS_structTask myTask;
 * OS_enumDelayTask(&myTask, 100);
 * // Delays 'myTask' for 100 ticks by setting blocking and terminating it.
 * @endcode
 */
OS_enumErrorStatus OS_enumDelayTask(OS_structTask* Add_structTask,u32 Copy_u32NoOfTicks){
	Add_structTask->Waiting.Blocking = OS_TASK_BLOCKING_ENABLE;
	Add_structTask->Waiting.TicksCount = Copy_u32NoOfTicks;
	OS_enumTerminateTask(Add_structTask);
	return OS_OK;
}
/**
 * @brief Initializes the operating system.
 *
 * This function initializes the operating system by performing the following steps:
 * 1. Initializes hardware using `OS_voidHwInit`.
 * 2. Sets the operating system mode to suspended (`OS_SUSPEND`).
 * 3. Creates the main stack using `OS_enumCreateMainStack`.
 * 4. Initializes the ready queue (`Global_structReadyQueue`) with a capacity of 100.
 * 5. Initializes the idle task (`Global_structIdleTask`) with the lowest priority,
 *    a task name "IDLE", a stack size of 300, and creates it using `OS_enumCreateTask`.
 *
 * @return OS_enumErrorStatus Returns OS_OK if initialization is successful, or an error code
 *         indicating specific initialization failure otherwise.
 *
 * @details
 * The function initializes essential components of the operating system before starting it.
 *
 * Example usage:
 * @code
 * OS_enumErrorStatus initStatus = OS_enumInit();
 * if (initStatus != OS_OK) {
 *     // Handle initialization error
 * }
 * @endcode
 */
OS_enumErrorStatus OS_enumInit(){
	OS_enumErrorStatus Error = OS_OK;

	OS_voidHwInit();
	// Update OS mode
	OS_StructOS.OS_enumMode = OS_SUSPEND;

	// Assign Main Stack
	Error += OS_enumCreateMainStack();

	// Create Ready Queue
	for(uint32_t i = 0; i<OS_TASK_PRIORITY_LEVELS;i++){
		if(OS_enumFifoInit(&Global_structReadyQueue[i], Global_structReadyQueueFIFO[i], 5)!=FIFO_NO_ERROR){
			Error+=FIFO_INIT_ERROR;
		}
	}

	// Idle Task
	strcpy(Global_structIdleTask.TaskName , "IDLE");
	Global_structIdleTask.Priority = 0;
	Global_structIdleTask.func = OS_voidIdleTask;
	Global_structIdleTask.StackSize = 300 ;
	Error += OS_enumCreateTask(&Global_structIdleTask);

	return Error;
}

/** OS_enumStartOS
 * @brief Starts the operating system.
 *
 * This function starts the operating system by performing the following steps:
 * 1. Sets the operating system mode to running (`OS_RUNNING`).
 * 2. Sets the current task to the idle task (`Global_structIdleTask`).
 * 3. Activates the idle task using `OS_enumActivateTask`.
 * 4. Starts the system timer using `OS_voidStartTimer`.
 * 5. Sets the Process Stack Pointer (PSP) to the idle task's CurrentPSP.
 * 6. Switches to the PSP mode and executes the idle task's function (`func`).
 *
 * @return OS_enumErrorStatus Returns OS_OK indicating successful OS start.
 *
 * @details
 * The function starts the OS execution flow from the idle task after initializing
 * essential components and setting up the idle task.
 *
 * Example usage:
 * @code
 * OS_enumErrorStatus startStatus = OS_enumStartOS();
 * if (startStatus != OS_OK) {
 *     // Handle OS start error
 * }
 * @endcode
 */
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






