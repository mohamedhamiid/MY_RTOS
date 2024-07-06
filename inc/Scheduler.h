/****************************************************************/
/* Author  : Mohamed Abdel Hamid                                */
/* Date    : 7 / 6 / 2024                             	        */
/* Version : V01                                                */
/* Email   : mohamedhamiid20@gmail.com                          */
/* Brief   : Handling Priority Round Robin Scheduling Algorithm */
/****************************************************************/

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_
/** OS_voidSortSchedulerTable
 * @brief Sorts the scheduler table based on task priorities.
 *
 * This function sorts the tasks in the scheduler table in ascending order of their priorities
 * using the Bubble Sort algorithm. Tasks with lower priority values will be placed before tasks
 * with higher priority values.
 *
 * @details
 * The function performs the following steps:
 * 1. Iterates over the task table multiple times, comparing each task's priority with the next task's priority.
 * 2. Swaps tasks if the current task's priority is greater than the next task's priority.
 * 3. Repeats the process until the entire table is sorted.
 *
 * Example usage:
 * @code
 * OS_voidSortSchedulerTable();
 * // The scheduler table is now sorted based on task priorities.
 * @endcode
 */
void OS_voidSortSchedulerTable();
/** OS_voidUpdateReadyQueue
 * @brief Updates the ready queue for task scheduling.
 *
 * This function updates the ready queue by removing all tasks from the queue and re-adding tasks
 * that are ready to be scheduled. It ensures that tasks with the highest priority are at the front
 * of the queue and only non-suspended tasks are considered.
 *
 * @details
 * The function performs the following steps:
 * 1. Frees the ready queue by dequeuing all tasks until the queue is empty.
 * 2. Iterates over the task table to identify tasks that are not suspended.
 * 3. Enqueues tasks that are ready to be scheduled based on their priority.
 *    - Tasks with a lower priority value are enqueued first.
 *    - If two tasks have the same priority, they are both enqueued.
 * 4. Updates the task state to `OS_TASK_READY` for enqueued tasks.
 *
 * Example usage:
 * @code
 * OS_voidUpdateReadyQueue();
 * // The ready queue is now updated with tasks ready to be scheduled.
 * @endcode
 */
void OS_voidUpdateReadyQueue();
/** OS_voidDecideNext
 * @brief Decides the next task to run on the CPU.
 *
 * This function determines which task should run next by checking the ready queue and the current task's state.
 * It ensures that the highest priority task is selected and maintains round-robin scheduling for tasks with the
 * same priority.
 *
 * @details
 * The function performs the following steps:
 * 1. Checks if the ready queue is empty and the current task is not suspended.
 *    - If both conditions are met, it continues running the current task:
 *      - Marks the current task as running.
 *      - Enqueues the current task back into the ready queue.
 *      - Sets the next task to the current task.
 * 2. If the ready queue is not empty or the current task is suspended:
 *    - Dequeues the next task from the ready queue.
 *    - Marks the next task as running.
 *    - Checks if the next task has the same priority as the current task and the current task is not suspended:
 *      - Enqueues the current task back into the ready queue.
 *      - Marks the current task as ready.
 *
 * Example usage:
 * @code
 * OS_voidDecideNext();
 * // The next task to run is now decided and ready to be scheduled.
 * @endcode
 */
void OS_voidDecideNext();
/** OS_voidSvcServices
 * @brief Handles Supervisor Call (SVC) services in Handler Mode.
 *
 * This function is called by the SVC handler to execute actions based on the SVC ID provided in the stack frame.
 * It performs different operations depending on the SVC ID:
 * - SVC_ACTIVATE: Activates or terminates a task. Updates scheduler and ready queues, decides the next task
 *   to run, and triggers a PendSV interrupt if the operating system is in running mode.
 * - SVC_WAITING: Suspends a task. Updates scheduler and ready queues.
 * - SVC_SUSPEND: Placeholder case for future expansion or specific handling of task suspension.
 *
 * @param Add_u32StackFrame Pointer to the stack frame containing SVC parameters.
 *
 * @details
 * The function performs the following steps based on the SVC ID:
 * 1. Updates the scheduler table by sorting tasks based on priorities.
 * 2. Updates the ready queue with tasks ready to be scheduled.
 * 3. If the operating system is in running mode (`OS_RUNNING`):
 *    - Decides the next task to run.
 *    - Triggers a PendSV interrupt to perform a context switch to the next task.
 *
 * Example usage:
 * @code
 * // Called by the SVC handler to execute the appropriate service based on SVC_ID
 * __asm("TST LR , #4 \n\t"
 * 	  "ITE EQ \n\t" // If then equal
 * 	  "MRSEQ R0 , MSP \n\t"
 * 	  "MRSNE R0 , PSP \n\t"
 * 	  "B OS_voidSvcServices"
 * );
 * @endcode
 */
void OS_voidSvcServices(pu32 Add_u32StackFrame);
/** OS_enumUpdateNoOfTicks
 * @brief Updates the tick count and handles task waiting based on blocking conditions.
 *
 * This function iterates through the task table and decrements the tick count for tasks
 * that are in a blocking state. If a task's tick count reaches 1, it transitions the task
 * state to `OS_TASK_WAITING`, disables blocking, and requests a service to update the scheduler.
 *
 * @details
 * The function performs the following steps for each task in the task table:
 * 1. Checks if the task's blocking mode is enabled (`OS_TASK_BLOCKING_ENABLE`).
 * 2. Decrements the tick count (`TicksCount`) for tasks that are blocking.
 * 3. If the tick count reaches 1:
 *    - Sets the task state to `OS_TASK_WAITING`.
 *    - Disables blocking for the task.
 *    - Requests a service (SVC_WAITING) to update the scheduler table.
 *
 * Example usage:
 * @code
 * OS_enumUpdateNoOfTicks();
 * // Updates tick counts and handles task waiting based on blocking conditions.
 * @endcode
 */
void OS_enumUpdateNoOfTicks();
/** OS_voidIdleTask
 * @brief Idle task function that runs when no other tasks are ready to execute.
 *
 * This function continuously toggles an LED (for testing purposes) and then puts the CPU
 * into a low-power state using the WFE (Wait For Event) instruction. The WFE instruction
 * allows the CPU to enter sleep mode until an event occurs, reducing CPU power consumption.
 *
 * @details
 * The function runs an infinite loop where:
 * - It toggles a global variable (`Global_u8IdleTaskLed`) to indicate its operation.
 * - It executes the WFE instruction to put the CPU into sleep mode, reducing power consumption
 *   until an interrupt or event wakes it up.
 *
 * Example usage:
 * @code
 * OS_voidIdleTask();
 * // Executes the idle task, toggling an LED and putting the CPU into low-power sleep mode.
 * @endcode
 */
void OS_voidIdleTask();



#endif /* INC_SCHEDULER_H_ */
