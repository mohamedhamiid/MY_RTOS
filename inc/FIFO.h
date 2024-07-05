/*
 * MYRTOS_FIFO.h
 *
 *  Created on: Apr 9, 2024
 *      Author: mh_sm
 */

#ifndef INC_FIFO_H_
#define INC_FIFO_H_

#include <stdio.h>
#include <stdint.h>
#include "Task.h"
#include "System.h"
/*customer can select element type */
#define ELEMENT_TYPE OS_structTask*
typedef struct{
	unsigned int counter;
	ELEMENT_TYPE* head;
	ELEMENT_TYPE* tail;
	ELEMENT_TYPE* base;
	unsigned int  length;
}OS_tBuffer;

typedef enum{
	FIFO_NO_ERROR,
	FIFO_FULL,
	FIFO_EMPTY,
	FIFO_NULL,

}OS_enumBufferStatus;

/*APIs*/
OS_enumBufferStatus OS_enumFifoInit (OS_tBuffer* fifo,ELEMENT_TYPE* buff , u32 length);
OS_enumBufferStatus OS_enumFifoEnqueue (OS_tBuffer* fifo,ELEMENT_TYPE item);
OS_enumBufferStatus OS_enumFifoDequeue (OS_tBuffer* fifo,ELEMENT_TYPE* item);
OS_enumBufferStatus OS_enumIsFifoFull (OS_tBuffer* fifo);


#endif /* INC_FIFO_H_ */
