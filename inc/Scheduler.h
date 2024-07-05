/*
 * Scheduler.h
 *
 *  Created on: Jul 5, 2024
 *      Author: mh_sm
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

void OS_voidSortSchedulerTable();

void OS_voidUpdateReadyQueue();

/* In this function we decide the nextTask and make it running
 * And make the current task ready */
void OS_voidDecideNext();
/* Called by SVC handler (Handler Mode) to execute
 * what should be done in each service*/
void OS_voidSvcServices(pu32 Add_u32StackFrame);
void OS_voidIdleTask();
void OS_enumUpdateNoOfTicks();



#endif /* INC_SCHEDULER_H_ */
