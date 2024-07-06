# MyRTOS (Real-Time Operating System) for ARM Cortex M3 and Portability

MyRTOS is a lightweight, real-time operating system designed for ARM Cortex M3 microcontrollers, with a focus on efficiency and portability across different platforms.

## Features

- **Priority Round-Robin Scheduling Algorithm**: Ensures fair task execution based on priority levels.
  
- **Optimized Context Switching**: Minimizes latency and optimizes interrupt handling, achieving a context switching time of 60 microseconds.
  
- **Sleep Mode in Idle**: Enhances power efficiency by allowing the system to enter sleep mode when no tasks are active.
  
- **Semaphore as Synchronization Tool**: Provides robust synchronization between tasks, with built-in support for handling priority inversion.

## Getting Started

### Prerequisites

- ARM Cortex M3 development environment (e.g., ARM Keil, GCC for ARM)
- [List any additional dependencies]

### Installation

1. Clone the MyRTOS repository from GitHub:

   ```bash
   git clone https://github.com/mohamedhamiid/MY_RTOS

