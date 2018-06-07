#include "queue.h"

/***********************************************
Function: Create a empty stack;
************************************************/
void initQueue(Queue *Q,uint32_t maxSize)
{
		Q->queuesize = maxSize;
		Q->q = (uint32_t *)malloc(sizeof(uint32_t) * Q->queuesize); //分配内存
		Q->tail = 0;
		Q->head = 0;
}

bool isQueueFull(Queue *Q)
{
	if((Q->tail+1)% Q->queuesize == Q->head)
	{
			return true;
	}
	else
	{
			return false;
	}
}

bool isQueueEmpty(Queue *Q)
{
	if(Q->head == Q->tail)
	{
			return true;
	}
	else
	{
			return false;
	}
}

bool enQueue(Queue *Q, uint8_t key)
{
	int tail = (Q->tail+1) % Q->queuesize; //取余保证，当quil=queuesize-1时，再转回0
	if (tail == Q->head)                   //此时队列没有空间
	{
			return false;
	}
	else
	{
			Q->q[Q->tail] = key;
			Q->tail = tail;
	}

	return true;
}

bool deQueue(Queue *Q,uint8_t* value)
{
	int tmp;
	if(Q->tail == Q->head)     //判断队列不为空
	{
			return false;
	}
	else
	{
			tmp = Q->q[Q->head];
			Q->head = (Q->head+1) % Q->queuesize;
	}

	*value = tmp;
	return true;
}
