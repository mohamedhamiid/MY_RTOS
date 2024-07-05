/*
 * Mem_Management.h
 *
 *  Created on: Jul 5, 2024
 *      Author: mh_sm
 */

#ifndef INC_MEM_MANAGEMENT_H_
#define INC_MEM_MANAGEMENT_H_

#define OS_MAIN_STACK_SIZE    3072 // 3KB

OS_enumErrorStatus OS_enumCreateMainStack();
OS_enumErrorStatus OS_enumCreateStack(OS_structTask* Add_structTask);


#endif /* INC_MEM_MANAGEMENT_H_ */
