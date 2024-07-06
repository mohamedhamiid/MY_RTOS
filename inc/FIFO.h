/****************************************************************/
/* Author  : Mohamed Abdel Hamid                                */
/* Date    : 7 / 6 / 2024                             	        */
/* Version : V01                                                */
/* Email   : mohamedhamiid20@gmail.com                          */
/* Brief   : Handling FIFOs operations like enqueue and dequeue */
/****************************************************************/
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

/* APIs */
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
OS_enumBufferStatus OS_enumFifoInit (OS_tBuffer* fifo,ELEMENT_TYPE* buff , u32 length);
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
OS_enumBufferStatus OS_enumFifoEnqueue (OS_tBuffer* fifo,ELEMENT_TYPE item);
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
OS_enumBufferStatus OS_enumFifoDequeue (OS_tBuffer* fifo,ELEMENT_TYPE* item);
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
OS_enumBufferStatus OS_enumIsFifoFull (OS_tBuffer* fifo);


#endif /* INC_FIFO_H_ */
