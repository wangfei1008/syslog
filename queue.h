#ifndef C_LANGUAGE_QUEUE_20111209_WANGFEI_H
#define C_LANGUAGE_QUEUE_20111209_WANGFEI_H

#include <time.h>

typedef void* qtype;
typedef struct queuel queue;
typedef struct node qnode;

typedef struct node
{
	qtype data;
	qnode* next;
}node;

typedef struct queuel
{
	node* front;
	node* back;
	unsigned int size;
	pthread_mutex_t lock;
}queuel;

void initqueue(queue* phandle);

void clearqueue(queue* phandle);

int pushqueue(queue* phandle, qtype value);

qtype popqueue(queue* phandle);

int lengthqueue(queue* phandle);

#endif//C_LANGUAGE_QUEUE_20111209_WANGFEI_H
