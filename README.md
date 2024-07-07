# MyRTOS (Real-Time Operating System) for ARM Cortex M3

MyRTOS is a lightweight, real-time operating system designed for ARM Cortex M3 microcontrollers, with a focus on efficiency. It provides a priority preemptive round-robin scheduler, memory management and a set of APIs to help you manage tasks and synchronization in your embedded systems projects.

## Features

- **Preemptive Priority Round-Robin Scheduling Algorithm**: Ensures fair task execution based on priority levels.
- **Task Management**: Easily create, activate, and terminate tasks in your embedded applications.
- **Timing Control** : Implement time-based waiting for tasks.
- **Optimized Context Switching**: Minimizes latency and optimizes interrupt handling, achieving a context switching time of 60 microseconds.
- **Sleep Mode when Idle**: Enhances power efficiency by allowing the system to enter sleep mode when no tasks are active.
- **Semaphore as Synchronization Tool**: Provides efficient synchronization between tasks, with built-in support for handling priority inversion.

## Getting Started

### Prerequisites

- ARM Cortex M3 development environment (e.g., ARM Keil,STM32CubeIde,etc)

### Installation

1. Clone the MyRTOS repository from GitHub:

   ```bash
   git clone https://github.com/mohamedhamiid/MY_RTOS

2. Include header files in your project.
   ```c
   #include "Task.h"
   #include "Semaphore.h"
4. Add source files to your project.
5. Configure the MyRTOS through _MyRTOSConfig_ header file according to your system requirements, such as tick time, CPU clock, etc.
6. Initialize MyRTOS using the _OS_enumInit()_ function in your application code.
7. Initialize task by configuring it
   ```c
   void task1(){
      while(1){
         // Your Code
      }
   }
   OS_structTask t1
   t1.func = task1;
   t1.Priority = 1;
   strcpy(t1.TaskName,"Task 1");
   t1.StackSize = 1024;
8. Create task using the _OS_enumCreateTask(&t1)_ function.
9. Start the operating system using _OS_enumStartOS();_ to begin task scheduling.

## Usage
Here is APIs of MyRTOS:
1. Initialization:
   ```c
   OS_enumInit();
2. Task Creation:
   ```c
   void tast1(){
      while(1){
         // Your Code
      }
   }
   OS_structTask t1
   t1.func = task1;
   t1.Priority = 1;
   strcpy(t1.TaskName,"Task 1");
   t1.StackSize = 1024;
   loc_enumERROR = OS_enumCreateTask(&t3);
	if(loc_enumERROR != OS_OK)
		while(1);
3. Task Activation:
   ```c
   loc_enumERROR= OS_enumActivateTask(&t1);
	if(loc_enumERROR != OS_OK)
		while(1);
4. Task Termination:
    ```c
   loc_enumERROR= OS_enumTerminateTask(&t1);
	if(loc_enumERROR != OS_OK)
		while(1);
5. Operating System Starting:
   ```c
   OS_enumStartOS();
6. Task waiting:
    ```c
   void tast1(){
      while(1){
         // Your Code
         OS_enumDelayTask(&t1, 100);
      }
   }
7. Semaphore acquiring and releasing:
    ```c
    OS_tstructSemaphore s1;
    void task1(){
        while(1){
            // Your Code
            OS_enumAcquireSemaphore(&s1, &t1);
    			OS_enumAcquireSemaphore(&s2, &t1);
         }
    }
    main(){
            OS_enumInitSemaphore(&s1, 1); // 1 indicates binary semaphore
    }
## Features to be added
1. Inter-process Communication
2. Deadlock avoidance
