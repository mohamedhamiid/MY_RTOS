/****************************************************************/
/* Author  : Mohamed Abdel Hamid                                */
/* Date    : 7 / 6 / 2024                             	        */
/* Version : V01                                                */
/* Email   : mohamedhamiid20@gmail.com                          */
/* Brief   : Handling Memory operations like creating stacks    */
/****************************************************************/

#ifndef INC_MEM_MANAGEMENT_H_
#define INC_MEM_MANAGEMENT_H_

/* Used in creating main stack */
#define OS_MAIN_STACK_SIZE    3072 // 3KB

/** OS_enumCreateStack
 * @brief Creates a stack frame for a new task.
 *
 * This function sets up the initial stack frame for a task by initializing the stack with appropriate
 * register values and stack pointers. It prepares the stack for the task to be run.
 *
 * @param Add_structTask Pointer to the task structure that includes task-specific information such as
 *                       the stack pointer and the task function.
 *
 * @return OS_enumErrorStatus An enumeration indicating the status of the stack creation process:
 *         - OS_OK: Indicates that the stack was successfully created.
 *
 * @details
 * The function performs the following steps to initialize the stack:
 * 1. Sets the `CurrentPSP` (Process Stack Pointer) to the start of the task's stack area (`_S_PSP_Task`).
 * 2. Adds dummy values for the task's registers onto the stack in the required order:
 *    - XPSR: Sets the T bit (bit 24) to 1.
 *    - PC: Sets the Program Counter to the task's function address.
 *    - LR: Sets the Link Register to 0xFFFFFFFD (indicating Thread mode with PSP).
 *    - R12, R3, R2, R1, R0, R4, R5, R6, R7, R8, R9, R10, R11: Sets these registers to 0.
 * 3. The stack pointer (`CurrentPSP`) is decremented as values are pushed onto the stack.
 * 4. Returns `OS_OK` to indicate successful stack creation.
 *
 * Example usage:
 * @code
 * OS_enumCreateStack(Add_structTask);
 * @endcode
 */
OS_enumErrorStatus OS_enumCreateMainStack();
/** OS_enumCreateMainStack
 * @brief Initializes the main stack for the operating system.
 *
 * This function sets up the main stack for the operating system by defining the start and end of the stack
 * and ensuring proper alignment between the main stack and the first task's stack.
 *
 * @return OS_enumErrorStatus An enumeration indicating the status of the main stack creation process:
 *         - OS_OK: Indicates that the main stack was successfully created.
 *
 * @details
 * The function performs the following steps to initialize the main stack:
 * 1. Sets the start of the main stack (`_S_MSP_Task`) to the end of the stack region (`_estack`).
 * 2. Defines the end of the main stack (`_E_MSP_Task`) by subtracting the main stack size (`OS_MAIN_STACK_SIZE`)
 *    from the start of the main stack.
 * 3. Ensures 8-byte alignment between the main stack and the first task's stack by setting `PSP_LastEnd` to the
 *    end of the main stack minus a padding value (`OS_STACK_PADDING`).
 * 4. Returns `OS_OK` to indicate successful main stack creation.
 *
 * Example usage:
 * @code
 * OS_enumErrorStatus status = OS_enumCreateMainStack();
 * if (status == OS_OK) {
 *     // Main stack was successfully created, and the system is ready to start tasks.
 * } else {
 *     // Handle error.
 * }
 * @endcode
 */
OS_enumErrorStatus OS_enumCreateStack(OS_structTask* Add_structTask);


#endif /* INC_MEM_MANAGEMENT_H_ */
