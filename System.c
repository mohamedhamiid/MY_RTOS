/**********************************************************/
/* Author  : Mohamed Abdel Hamid                          */
/* Date    : 7 / 6 / 2024                             	  */
/* Version : V01                                          */
/* Email   : mohamedhamiid20@gmail.com                    */
/* Brief   : Handling system structures                   */
/**********************************************************/
#include <stdint.h>
#include "STD_TYPES.h"
#include "Task.h"
#include "Scheduler.h"

#include "Interrupt_Management.h"

#include "MyRTOSConfig.h"

/**
 * @brief Define values for multi-level queue and bit map
 */
#define OS_TASKS_NO_OF_CLUSTERS  					8
#define OS_TASKS_NO_OF_PRIORITIES_PER_CLUSTER       (OS_TASK_PRIORITY_LEVELS / OS_TASKS_NO_OF_CLUSTERS)

/**
 * @brief Structure defining the operating system (OS) attributes.
 */
struct {
    OS_structTask* CurrentTask;         // Pointer to the currently running task
    OS_structTask* NextTask;            // Pointer to the next task to be scheduled

	uint8_t isOsIsrRunning;
	uint8_t isIsrDisabled;
	uint8_t isIsrSuspended;
	uint8_t isOsIsrSuspended;
	uint8_t initialPriorityValue;
	uint8_t interruptId;
	uint8_t highestOsIsrPriority;

    u8  NoOfCreatedTasks;               // Number of tasks created in the system
    u32 _S_MSP_Task;                    // Start of main (OS) stack pointer
    u32 _E_MSP_Task;                    // End of main (OS) stack pointer
    u32 PSP_LastEnd;                    // End of the last allocated Process Stack Pointer (PSP)

    enum {
        OS_SUSPEND,                     // Operating system in suspend mode
        OS_RUNNING                      // Operating system in running mode
    } OS_enumMode;                      // Current mode of the operating system


    OS_structTask* TaskTable[100];      // Array of pointers to all tasks in the system
                                        // Assuming a maximum of 100 tasks in the system

    uint32_t bitMap0;
    uint8_t bitMap1[OS_TASKS_NO_OF_PRIORITIES_PER_CLUSTER];
//    ISR_tstructICB* OsIsrTable[100]; 	// Table of all CAT2 ISRs of System
} OS_StructOS;

/**
 * @brief Enumeration defining the Service Call (SVC) IDs for the operating system.
 */
enum {
	SVC_GET_ISR_ID = 1,      // SVC ID for Getting ISR ID (Autosar Compliant)
    SVC_ACTIVATE,        // SVC ID for activating a task
    SVC_TERMINATE,       // SVC ID for terminating a task
    SVC_WAITING,         // SVC ID for task waiting
    SVC_SUSPEND,         // SVC ID for suspending a task
} OS_enumSvcID;


