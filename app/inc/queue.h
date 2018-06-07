#ifndef __QUEUE_H_
#define __QUEUE_H_

#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct queue
{
	uint32_t queuesize;   //����Ĵ�С
	uint32_t head, tail;  //���е�ͷ��β�±�
	uint32_t *q;          //����ͷָ��
}Queue,*pQueue;

extern void initQueue(Queue *Q,uint32_t maxSize);
extern bool isQueueFull(Queue *Q);
extern bool isQueueEmpty(Queue *Q);
extern bool enQueue(Queue *Q, uint8_t key);
extern bool deQueue(Queue *Q,uint8_t* value);

#endif
