/****************************************************************/
/* Author  : Mohamed Abdel Hamid                                */
/* Date    : 7 / 6 / 2024                             	        */
/* Version : V01                                                */
/* Email   : mohamedhamiid20@gmail.com                          */
/* Brief   : Handling Ports according to STM32F1 ARM CortexM3   */
/****************************************************************/

#include "Task.h"
#include "System.h"
#include "Scheduler.h"
#include "Porting_CortexM.h"
#include "MyRTOSConfig.h"

/* Variable for ticker checking */
uint8_t Global_u8SystickLed;

/* FAULTS : useful for debugging */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

void SysTick_Handler(void)
{
	/* For Testing */
	Global_u8SystickLed ^= 1;
	/* Update Number of Ticks */
	OS_enumUpdateNoOfTicks();
	/* Decide Current and Next task */
	OS_voidDecideNext();
	/* Trigger PendSV for switch context */
	OS_TRIGGER_PENDSV();
}

/* SVC Handler */
/* Description:
 * Here we want to call a service so we want to know the SVC number
 * To do that we must get the PSP or MSP of the interrupted task
 * Because in Stack has the values R0 , R1 , R2 , R3 , R12 ,LR , XPSR
 * So we need the PC which points to the next instruction after SVC call
 * As the SVC instruction has size 2 bytes, the PC = SVC Location in flash + 2
 * Here We do the following steps:
 * 1- Check is it MSP or PSP
 * 2- Activate EQ
 * 3- If EQ then it's MSP so move it to R0
 * 4- If not EQ then it's PSP so move it to R0
 * 5- Call the C function
 * The calling has R0 as first attribute so we must make pointer to receive the sp value
 * NOTE: we use __attribute ((naked)) to prevent the handler to change stack by adding
 * any value */
__attribute ((naked)) void SVC_Handler(){
	//ChangeAccessLevel(PRIVILEGE);
	__asm("TST LR , #4 \n\t"
		  "ITE EQ \n\t" // If then equal
		  "MRSEQ R0 , MSP \n\t"
		  "MRSNE R0 , PSP \n\t"
		  "B OS_voidSvcServices"
	);
}



void OS_voidHwInit(){
	/* By default:
	 * CPU and SysTick clock = 72 MHZ
	 * 1 count -> (1/72) us
	 * x count -> 1 ms
	 * x = 72000 */

	/* Decrease PendSV priority to be equal to Systick Priority */
	__NVIC_SetPriority(PendSV_IRQn,15);

}
void OS_voidStartTimer(){
	/* By default:
	 * CPU and SysTick clock = 72 MHZ
	 * 1 count -> (1/72) us
	 * x count -> 1 ms
	 * x = 72000 */
	u32 Loc_u8Count = OS_TICK_TIME_IN_MS * OS_CPU_CLOCK_FREQ_IN_MHZ ;
	SysTick_Config(Loc_u8Count);
}

/** PendSV_Handler
 * @brief PendSV Handler for context switching between tasks.
 *
 * This function performs context switching by saving the context of the current task and restoring
 * the context of the next task. It is designed to be used with an ARM Cortex-M processor.
 * The function is marked as `naked` to avoid compiler-generated prologue and epilogue code.
 *
 * @details
 * The function performs the following steps:
 * 1. Checks if there is a next task to switch to (`OS_StructOS.NextTask != NULL`).
 * 2. Saves the current task's context:
 *    - Gets the current Process Stack Pointer (PSP) and updates the task structure.
 *    - Manually pushes the registers R4 to R11 onto the stack.
 * 3. Switches to the next task:
 *    - Sets the current task to the next task.
 *    - Clears the next task pointer.
 * 4. Restores the next task's context:
 *    - Manually restores the registers R4 to R11 from the stack.
 *    - Sets the PSP to the next task's PSP.
 * 5. Branches to the link register (LR) to exit the handler.
 */
__attribute((naked)) void PendSV_Handler(void)
{
	/* Context Switching */
	/* Save the context of current */
	if (OS_StructOS.NextTask != NULL){
		/* 1- Get current PSP
		 * the returned PSP is after the CPU pushes XPSR, LR, PC, R0->R3,R12 */
		OS_GET_PSP(OS_StructOS.CurrentTask->CurrentPSP);
		/* 2- Push R4-> R11 manually */
		OS_StructOS.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R4":"=r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R5":"=r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R6":"=r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R7":"=r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R8":"=r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R9":"=r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R10":"=r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP--;
		__asm volatile("MOV %0 , R11":"=r"(*(OS_StructOS.CurrentTask->CurrentPSP)));

		/* Restore the context of current */

		OS_StructOS.CurrentTask = OS_StructOS.NextTask;
		OS_StructOS.NextTask = 	NULL;
		/* The last value of the CurrentPSP of the current task is that is
		 * pointing to R11 */
		/* 1- Restore manually pushed registers */

		__asm volatile("MOV R11 , %0"::"r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R10 , %0"::"r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R9 , %0"::"r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R8 , %0"::"r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R7 , %0"::"r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R6 , %0"::"r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R5 , %0"::"r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP++;
		__asm volatile("MOV R4 , %0"::"r"(*(OS_StructOS.CurrentTask->CurrentPSP)));
		OS_StructOS.CurrentTask->CurrentPSP++;

		/* 2- Set PSP to the the task */
		OS_SET_PSP(OS_StructOS.CurrentTask->CurrentPSP);
		/* Now the CPU can restore automatically */
	}
	__asm volatile("BX LR");
}
