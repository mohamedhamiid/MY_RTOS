/**********************************************************/
/* Author  : Mohamed Abdel Hamid                          */
/* Date    : 7 / 6 / 2024                             	  */
/* Version : V01                                          */
/* Email   : mohamedhamiid20@gmail.com                    */
/* Brief   : Handling system structures                   */
/**********************************************************/
#ifndef INC_TASK_H_
#define INC_TASK_H_

#include "STD_TYPES.h"

typedef enum {
	noAutoStart,
	AutoStart
}OS_enumTaskAutoStart;



// Task Structure
typedef struct{
	/* Entered by user */
	u8 Priority;
	u8 TaskName[30] ;
	u16 StackSize;
	void (*func)(void); // Pointer to C function of the task
	OS_enumTaskAutoStart AutoStart ;
	/* Not entered by user */
	struct{
		enum{
			OS_TASK_BLOCKING_DISABLE,
			OS_TASK_BLOCKING_ENABLE
		}Blocking;
		u32 TicksCount;
	}Waiting;
	u32 _S_PSP_Task ; // Start of task stack
	u32 _E_PSP_Task ; // End   of task stack
	pu32 CurrentPSP ;
	enum{
		OS_TASK_SUSPEND,
		OS_TASK_WAITING,
		OS_TASK_READY,
		OS_TASK_RUNNING
	}TaskState;
}OS_structTask;

typedef enum {
	OS_OK  ,
	OS_EXCEED_AVAILABLE_STACK,
	FIFO_INIT_ERROR,
	TASK_CREATION_ERROR

}OS_enumErrorStatus;


/* APIs */
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
OS_enumErrorStatus OS_enumInit();
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
OS_enumErrorStatus OS_enumCreateTask(OS_structTask* Add_structTask);
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
OS_enumErrorStatus OS_enumActivateTask(OS_structTask* Add_structTask);
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
OS_enumErrorStatus OS_enumTerminateTask(OS_structTask* Add_structTask);
/** OS_enumDelayTask
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
OS_enumErrorStatus OS_enumDelayTask(OS_structTask* Add_structTask,u32 Copy_u32NoOfTicks);
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
OS_enumErrorStatus OS_enumStartOS();


#endif /* INC_TASK_H_ */
