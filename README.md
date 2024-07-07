# MyRTOS (Real-Time Operating System) for ARM Cortex M3

MyRTOS is a lightweight, real-time operating system designed for ARM Cortex M3 microcontrollers, with a focus on efficiency. It provides a priority preemptive round-robin scheduler, memory management and a set of APIs to help you manage tasks and synchronization in your embedded systems projects.

## Features

- **Preemptive Priority Round-Robin Scheduling Algorithm**: Ensures fair task execution based on priority levels.
- **Task Management**: Easily create, activate, and terminate tasks in your embedded applications.
- **Timing Control** : Implement time-based waiting for tasks.
- **Optimized Context Switching**: Minimizes latency and optimizes interrupt handling, achieving a context switching time of 66 microseconds.
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
## Testing
### Round Robin Scheduling
#### Scheduling
![context switching time](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/88eca129-4c1e-4783-b405-5985010daff0)
#### Process Stacks in switching between T2 and T3
![annotated Stack and registers before context](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/b05299ee-2c9b-45bf-a360-39e7aeb0f4bd)
![annotated Stack and registers after storing task 2 registers values in memory](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/43a5e4dd-ac6d-4db0-929b-f263e7e7c56c)
![Annotated Stack and registers restore task 3 data](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/d7bc8dd2-8f46-4b3e-886a-56b2908a67fc)
![Annotated PSP i![Uploading [annota![Annotated Stack and registers restore task 3 data](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/0ef632fa-fc67-4d30-acdd-43713f1dd4c1)
ted]Stack and registers before context.png…]()
s set to Task 3](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/6c9f6c5b-cd23-4179-8e8d-0f333b9bea33)
<hr>
### Preepmtion based on priority
#### T2 is activated from T1, T3 is activated from T2, and T4 is activated from T3, such that the priority is T4 > T3 > T2 > T1.
![preemtion](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/da1a4fab-1f80-4310-9cb1-b8d61bda1f09)
<hr>
### Timing Control
1. Periodicity of T1 is 100
2. Periodicity of T2 is 100
3. Periodicity of T3 is 500
<br></br>
![T1=100,T2=100,T3=500](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/109f3603-8a5d-4150-99b3-ae68e00a8cd8)
![T1 deadline](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/7cc2ea38-5fbe-4431-b5df-41d8725d0a05)
![T3 deadline](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/b8237f5f-9287-4e3f-954d-46c527a00f02)
<hr>
### Synchronization and priority inversion
![1](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/65cf820a-a750-4eb3-b0a3-07fc589e32f9)
<hr>
### Deadlock
![Deadlock example](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/e2a834ef-a666-47c6-bce4-885f82495248)
<hr>
### No task activated (Idle state) --> CPU enters sleep mode
![Idle Task when using WFE](https://github.com/mohamedhamiid/MY_RTOS/assets/111571591/278317c3-616b-4673-9582-fca0cfea6664)



