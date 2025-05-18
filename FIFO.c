/****************************************************************/
/* Author  : Mohamed Abdel Hamid                                */
/* Date    : 7 / 6 / 2024                             	        */
/* Version : V01                                                */
/* Email   : mohamedhamiid20@gmail.com                          */
/* Brief   : Handling FIFOs operations like enqueue and dequeue */
/****************************************************************/
#include "FIFO.h"
/* OS_enumFifoInit
 * @brief Initializes a FIFO (First In, First Out) buffer.
 *
 * This function sets up the internal structure of the FIFO to manage a buffer of a specified length
 * and prepares it for use by resetting its pointers and counters.
 *
 * @param fifo Pointer to the FIFO buffer structure to be initialized. This structure manages the buffer's state.
 * @param buff Pointer to the memory location where the actual data buffer is stored.
 *             This buffer will be used to hold the elements in the FIFO.
 * @param length Unsigned 32-bit integer representing the length of the buffer (i.e., the number of elements it can hold).
 *
 * @return OS_enumBufferStatus An enumeration indicating the status of the initialization process:
 *         - FIFO_NULL: Indicates that the provided buffer pointer (buff) is NULL, so the initialization cannot proceed.
 *         - FIFO_NO_ERROR: Indicates that the FIFO was successfully initialized.
 *
 * @details
 * The function performs the following steps:
 * 1. Checks if the `buff` pointer is NULL. If it is, returns `FIFO_NULL`.
 * 2. Initializes the FIFO structure:
 *    - `fifo->base` is set to the start of the provided buffer (`buff`).
 *    - `fifo->head` is set to the start of the buffer (where new elements will be added).
 *    - `fifo->tail` is set to the start of the buffer (where elements will be removed).
 *    - `fifo->length` is set to the specified length of the buffer.
 *    - `fifo->counter` is initialized to 0, indicating that the FIFO is currently empty.
 * 3. Returns `FIFO_NO_ERROR` if the FIFO is successfully initialized.
 *
 * Example usage:
 * @code
 * OS_tBuffer myFifo;
 * ELEMENT_TYPE buffer[100];
 * OS_enumBufferStatus status;
 *
 * status = OS_enumFifoInit(&myFifo, buffer, 100);
 * if (status == FIFO_NO_ERROR) {
 *     // FIFO initialized successfully, and ready for use.
 * } else {
 *     // Handle error.
 * }
 * @endcode
 */
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
/* OS_enumFifoEnqueue
 * @brief Enqueues an item into the FIFO (First In, First Out) buffer.
 *
 * This function adds an item to the FIFO buffer, handling circular wrapping of the buffer.
 *
 * @param fifo Pointer to the FIFO buffer structure to enqueue into.
 * @param item The item to be enqueued into the FIFO buffer.
 *
 * @return OS_enumBufferStatus An enumeration indicating the status of the enqueue operation:
 *         - FIFO_NULL: Indicates that the FIFO buffer structure is not valid.
 *         - FIFO_FULL: Indicates that the FIFO is full and cannot accept more items.
 *         - FIFO_NO_ERROR: Indicates that the item was successfully enqueued.
 *
 * @details
 * The function performs the following steps:
 * 1. Checks if the FIFO buffer structure is valid (i.e., `fifo->base` is not NULL and `fifo->length` is non-zero).
 *    If not, returns `FIFO_NULL`.
 * 2. Checks if the FIFO is full by comparing the head and tail pointers and the counter with the length.
 *    If full, returns `FIFO_FULL`.
 * 3. Enqueues the item by placing it at the current tail position and increments the counter.
 * 4. Updates the tail pointer to the next position, wrapping around to the base if necessary.
 * 5. Returns `FIFO_NO_ERROR` if the item was successfully enqueued.
 */
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


/* OS_enumFifoDequeue
 * @brief Dequeues an item from the FIFO (First In, First Out) buffer.
 *
 * This function removes an item from the FIFO buffer, handling circular wrapping of the buffer.
 *
 * @param fifo Pointer to the FIFO buffer structure to dequeue from.
 * @param item Pointer to where the dequeued item will be stored.
 *
 * @return OS_enumBufferStatus An enumeration indicating the status of the dequeue operation:
 *         - FIFO_NULL: Indicates that the FIFO buffer structure is not valid.
 *         - FIFO_EMPTY: Indicates that the FIFO is empty and there are no items to dequeue.
 *         - FIFO_NO_ERROR: Indicates that the item was successfully dequeued.
 *
 * @details
 * The function performs the following steps:
 * 1. Checks if the FIFO buffer structure is valid (i.e., `fifo->base` is not NULL and `fifo->length` is non-zero).
 *    If not, returns `FIFO_NULL`.
 * 2. Checks if the FIFO is empty by comparing the head and tail pointers.
 *    If empty, returns `FIFO_EMPTY`.
 * 3. Dequeues the item by reading it from the current head position and decrementing the counter.
 * 4. Updates the head pointer to the next position, wrapping around to the base if necessary.
 * 5. Returns `FIFO_NO_ERROR` if the item was successfully dequeued.
 */
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
/**
 * @brief Checks if the FIFO (First In, First Out) buffer is full.
 *
 * This function checks the status of the FIFO buffer to determine if it is full.
 *
 * @param fifo Pointer to the FIFO buffer structure to check.
 *
 * @return OS_enumBufferStatus An enumeration indicating the status of the FIFO:
 *         - FIFO_NULL: Indicates that the FIFO buffer structure is not valid (i.e., `fifo->head`, `fifo->base`, or `fifo->tail` is NULL).
 *         - FIFO_FULL: Indicates that the FIFO is full.
 *         - FIFO_NO_ERROR: Indicates that the FIFO is not full.
 *
 * @details
 * The function performs the following steps:
 * 1. Checks if the FIFO buffer structure is valid (i.e., `fifo->head`, `fifo->base`, and `fifo->tail` are not NULL).
 *    If any of these pointers are NULL, returns `FIFO_NULL`.
 * 2. Checks if the FIFO is full by comparing the counter with the length.
 *    If full, returns `FIFO_FULL`.
 * 3. Returns `FIFO_NO_ERROR` if the FIFO is not full.
 */
OS_enumBufferStatus OS_enumIsFifoFull (OS_tBuffer* fifo){

	if(!fifo->head || !fifo->base || !fifo->tail)
		return FIFO_NULL;
	if(fifo->counter == fifo->length)
		return FIFO_FULL;

	return FIFO_NO_ERROR;
}

OS_enumBufferStatus OS_enumIsFifoEmpty(OS_tBuffer* fifo){

	/* fifo empty */
	if (fifo->head == fifo->tail)
		return FIFO_EMPTY;
	else
		return FIFO_NO_ERROR;
}
