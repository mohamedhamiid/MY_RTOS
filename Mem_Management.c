/**********************************************************/
/* Author  : Mohamed Abdel Hamid                          */
/* Date    : 7 / 6 / 2024                             	  */
/* Version : V01                                          */
/* Email   : mohamedhamiid20@gmail.com                    */
/**********************************************************/
#include "System.h"
#include "Task.h"
#include "Mem_Management.h"

/* Externed from startup code */
extern u32 _estack ;

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
OS_enumErrorStatus OS_enumCreateMainStack(){

	OS_enumErrorStatus error = OS_OK ;
	OS_StructOS._S_MSP_Task = &_estack;
	OS_StructOS._E_MSP_Task = OS_StructOS._S_MSP_Task - OS_MAIN_STACK_SIZE;
	// Align 8 bytes between Main stack and first task
	OS_StructOS.PSP_LastEnd = (OS_StructOS._E_MSP_Task-OS_STACK_PADDING);

	return error;
}
