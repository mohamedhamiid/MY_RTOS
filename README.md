# MyRTOS (Real-Time Operating System) for ARM Cortex M3

MyRTOS is a lightweight, real-time operating system designed for ARM Cortex M3 microcontrollers, with a focus on efficiency.It provides a priority preemptive round-robin scheduler, memory management and a set of APIs to help you manage tasks and synchronization in your embedded systems projects.

## Features

- **Preemptive Priority Round-Robin Scheduling Algorithm**: Ensures fair task execution based on priority levels.
- **Task Management**: Easily create, activate, and terminate tasks in your embedded applications.
- **Timing Control** : Implement time-based waiting for tasks.
- **Optimized Context Switching**: Minimizes latency and optimizes interrupt handling, achieving a context switching time of 60 microseconds.
- **Sleep Mode in Idle**: Enhances power efficiency by allowing the system to enter sleep mode when no tasks are active.
- **Semaphore as Synchronization Tool**: Provides efficient synchronization between tasks, with built-in support for handling priority inversion.

## Getting Started

### Prerequisites

- ARM Cortex M3 development environment (e.g., ARM Keil,STM32CubeIde,etc)

### Installation

1. Clone the MyRTOS repository from GitHub:

   ```bash
   git clone https://github.com/mohamedhamiid/MY_RTOS

2. Include header files in your project.
3. Add source files to your project.
4. Configure the MyRTOS through _MyRTOSConfig_ according to your system requirements, such as tick time, CPU clock, etc.
5. Initialize MyRTOS using the _OS_enumInit()_ function in your application code.
6. Create task by _<mark>OS_structTask t1</mark>_ and configure it
   ```c
   	t1.func = task1;
   	t1.Priority = 1;
	strcpy(t1.TaskName,"Task 1");
	t1.StackSize = 1024;
8. Create tasks using the _OS_enumCreateTask(&t1)_ function and implement your task functions.
9. Start the operating system using MyRTOS_startOS to begin task scheduling.
   <span style="background-color: #FFFF00">Marked text</span>
