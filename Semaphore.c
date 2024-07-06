/**********************************************************/
/* Author  : Mohamed Abdel Hamid                          */
/* Date    : 7 / 6 / 2024                             	  */
/* Version : V01                                          */
/* Email   : mohamedhamiid20@gmail.com                    */
/**********************************************************/
#include "Task.h"
#include "FIFO.h"
#include "Semaphore.h"
/** OS_enumInitSemaphore
 * @brief Initializes a semaphore structure with initial values.
 *
 * This function initializes a semaphore structure with the specified number of available resources,
 * initializes the waiting queue, and sets other necessary parameters.
 *
 * @param Add_structSemaphore Pointer to the semaphore structure to initialize.
 * @param Copy_u8NoOfAvailable Number of available resources for the semaphore.
 * @return OS_tenuSemaphoreState Returns OS_SEMAPHORE_INIT_OK indicating successful initialization.
 *
 * @details
 * The function performs the following initialization steps:
 * 1. Sets the initial number of available resources (`s`) for the semaphore.
 * 2. Initializes the count of tasks waiting (`noOfWaiting`) to 0.
 * 3. Sets the current owner of the semaphore (`currentOwner`) to NULL.
 * 4. Initializes a FIFO queue (`Global_structWaitingQueue`) for tasks waiting on the semaphore.
 *
 * Example usage:
 * @code
 * OS_tstructSemaphore mySemaphore;
 * OS_enumInitSemaphore(&mySemaphore, 5);
 * // Initializes 'mySemaphore' with 5 available resources and initializes the waiting queue.
 * @endcode
 */
OS_tenuSemaphoreState OS_enumInitSemaphore(OS_tstructSemaphore* Add_structSemaphore, u8 Copy_u8NoOfAvailable){
	Add_structSemaphore->s = Copy_u8NoOfAvailable ;
	Add_structSemaphore->noOfWaiting = 0;
	Add_structSemaphore->currentOwner = NULL;

	OS_enumFifoInit (&(Add_structSemaphore->Global_structWaitingQueue),Add_structSemaphore->Global_structWaitingQueueFIFO , 100);

	return OS_SEMAPHORE_INIT_OK;
}
/**
 * @brief Attempts to acquire a semaphore for a given task.
 *
 * This function attempts to acquire a semaphore for a specific task by decrementing the semaphore's
 * available resource count (`s`). If the semaphore is already acquired by the same task (`currentOwner`),
 * it returns OS_SEMAPHORE_ALREADY_ACQUIRED. If the semaphore's resources are insufficient (`s < 0`) and
 * tasks are waiting, it adds the task to the semaphore's waiting queue and terminates the task.
 * Otherwise, it assigns the semaphore to the current task as its owner.
 *
 * @param Add_structSemaphore Pointer to the semaphore structure to acquire.
 * @param Add_structTask Pointer to the task attempting to acquire the semaphore.
 * @return OS_tenuSemaphoreState Returns OS_SEMAPHORE_ALREADY_ACQUIRED if the semaphore is already acquired
 *         by the same task, OS_SEMAPHORE_BUSY if the semaphore's resources are insufficient and tasks are waiting,
 *         or OS_SEMAPHORE_AVAILABLE if the semaphore is successfully acquired.
 *
 * @details
 * The function performs the following steps to acquire the semaphore:
 * 1. Decrements the available resource count (`s`) of the semaphore.
 * 2. Checks if the task already owns the semaphore (`currentOwner`). If true, returns OS_SEMAPHORE_ALREADY_ACQUIRED.
 * 3. If the semaphore's resources (`s`) are less than 0 and there are waiting tasks (`noOfWaiting` > 0):
 *    - Increments the count of waiting tasks.
 *    - Enqueues the task into the semaphore's waiting queue (`Global_structWaitingQueue`).
 *    - Terminates the task using `OS_enumTerminateTask`.
 *    - Returns OS_SEMAPHORE_BUSY.
 * 4. Otherwise, assigns the semaphore to the task as its owner (`currentOwner`).
 * 5. Returns OS_SEMAPHORE_AVAILABLE indicating successful acquisition.
 *
 * Example usage:
 * @code
 * OS_tstructSemaphore mySemaphore;
 * OS_structTask myTask;
 * OS_enumAcquireSemaphore(&mySemaphore, &myTask);
 * // Attempts to acquire 'mySemaphore' for 'myTask' and handles the semaphore state accordingly.
 * @endcode
 */
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
/**
 * @brief Releases a semaphore, allowing the next waiting task to acquire it.
 *
 * This function releases a semaphore by incrementing its available resource count (`s`).
 * If there are tasks waiting on the semaphore (`noOfWaiting > 0`), it dequeues the next task
 * from the semaphore's waiting queue (`Global_structWaitingQueue`), assigns it as the current owner
 * of the semaphore, activates the task using `OS_enumActivateTask`, and returns OS_SEMAPHORE_AVAILABLE.
 * If no tasks are waiting (`noOfWaiting <= 0`), it returns OS_SEMAPHORE_BUSY indicating that the semaphore
 * is still in use.
 *
 * @param Add_structSemaphore Pointer to the semaphore structure to release.
 * @return OS_tenuSemaphoreState Returns OS_SEMAPHORE_AVAILABLE if a task was successfully dequeued and assigned
 *         as the semaphore's owner, or OS_SEMAPHORE_BUSY if no tasks are waiting.
 *
 * @details
 * The function performs the following steps to release the semaphore:
 * 1. Increments the available resource count (`s`) of the semaphore.
 * 2. Checks if there are tasks waiting (`noOfWaiting > 0`).
 *    - Decreases the count of waiting tasks (`noOfWaiting`).
 *    - Dequeues the next task from the semaphore's waiting queue.
 *    - Sets the dequeued task as the current owner of the semaphore (`currentOwner`).
 *    - Activates the dequeued task using `OS_enumActivateTask`.
 *    - Returns OS_SEMAPHORE_AVAILABLE.
 * 3. If no tasks are waiting (`noOfWaiting <= 0`), returns OS_SEMAPHORE_BUSY.
 *
 * Example usage:
 * @code
 * OS_tstructSemaphore mySemaphore;
 * OS_enumReleaseSemaphore(&mySemaphore);
 * // Releases 'mySemaphore', allowing the next waiting task to acquire it if available.
 * @endcode
 */
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
