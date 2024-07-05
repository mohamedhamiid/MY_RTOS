/*
 * MYRTOS_FIFO.c
 *
 *  Created on: Apr 9, 2024
 *      Author: mh_sm
 */

#include "FIFO.h"


OS_enumBufferStatus OS_enumFifoInit(OS_tBuffer* fifo,ELEMENT_TYPE* buff , u32 length){

	if(!buff )
		return FIFO_NULL;

	fifo->base = buff ;
	fifo->head = fifo->base ;
	fifo->tail = fifo->base ;
	fifo->length = length;
	fifo->counter=0;

	return FIFO_NO_ERROR;

}

/* *********************************** */

OS_enumBufferStatus OS_enumFifoEnqueue (OS_tBuffer* fifo,ELEMENT_TYPE item){

	/* fifo null*/

	if (!fifo->base || !fifo->length)
		return FIFO_NULL;
	/*fifo is full*/

	/* fifo full */
	if ((fifo->head == fifo->tail) && (fifo->counter == fifo->length))
		return FIFO_FULL;

	*(fifo->tail)=item;
	fifo->counter++;

	/*for circular fifo again */

	/* circular enqueue */
	if (fifo->tail == (((unsigned int)fifo->base + (4*fifo->length )) - 4 ))
		fifo->tail = fifo->base;
	else
		fifo->tail++;

	return FIFO_NO_ERROR;


}

/* *********************************** */

OS_enumBufferStatus OS_enumFifoDequeue (OS_tBuffer* fifo,ELEMENT_TYPE* item){
	/* check fifo valid */
	if (!fifo->base || !fifo->length)
		return FIFO_NULL;

	/* fifo empty */
	if (fifo->head == fifo->tail)
		return FIFO_EMPTY;



	*item = *(fifo->head);
	fifo->counter--;

	/* circular dequeue */
	if (fifo->head == (((unsigned int)fifo->base + (4*fifo->length )) - 4 ))
		fifo->head = fifo->base;
	else
		fifo->head++;

	return FIFO_NO_ERROR;
}




/* *********************************** */

OS_enumBufferStatus OS_enumIsFifoFull (OS_tBuffer* fifo){

	if(!fifo->head || !fifo->base || !fifo->tail)
		return FIFO_NULL;
	if(fifo->counter == fifo->length)
		return FIFO_FULL;

	return FIFO_NO_ERROR;
}
