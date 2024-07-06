/**********************************************************/
/* Author  : Mohamed Abdel Hamid                          */
/* Date    : 7 / 6 / 2024                             	  */
/* Version : V01                                          */
/* Email   : mohamedhamiid20@gmail.com                    */
/* Brief   : Handling system structures                   */
/**********************************************************/
#include "STD_TYPES.h"
#include "Task.h"
#include "Scheduler.h"


/**
 * @brief Structure defining the operating system (OS) attributes.
 */
struct {
    u8  NoOfCreatedTasks;               // Number of tasks created in the system
    u32 _S_MSP_Task;                    // Start of main (OS) stack pointer
    u32 _E_MSP_Task;                    // End of main (OS) stack pointer
    u32 PSP_LastEnd;                    // End of the last allocated Process Stack Pointer (PSP)

    enum {
        OS_SUSPEND,                     // Operating system in suspend mode
        OS_RUNNING                      // Operating system in running mode
    } OS_enumMode;                      // Current mode of the operating system

    OS_structTask* CurrentTask;         // Pointer to the currently running task
    OS_structTask* NextTask;            // Pointer to the next task to be scheduled

    OS_structTask* TaskTable[100];      // Array of pointers to all tasks in the system
                                        // Assuming a maximum of 100 tasks in the system
} OS_StructOS;

/**
 * @brief Enumeration defining the Service Call (SVC) IDs for the operating system.
 */
enum {
    SVC_ACTIVATE,        // SVC ID for activating a task
    SVC_TERMINATE,       // SVC ID for terminating a task
    SVC_WAITING,         // SVC ID for task waiting
    SVC_SUSPEND,         // SVC ID for suspending a task
} OS_enumSvcID;


