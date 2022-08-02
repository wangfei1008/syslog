//////////////////////////////////////////////////////////////////////////////////////
/// @file queue.c
/// @Introduce 
///          
/// @author Wang Fei
/// @version 1.0
/// @date 2011-12-09
/////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "queue.h"

void initqueue(queue* phandle)
{
	pthread_mutex_init(&phandle->lock, NULL);
	clearqueue(phandle);
	phandle->front = phandle->back = NULL;
}

void clearqueue(queue* phandle)
{
	pthread_mutex_lock(&phandle->lock);
	node* pqnode = phandle->front;
	
	while(pqnode != NULL)
	{
		phandle->front = phandle->front->next;
		free(pqnode);
		pqnode = phandle->front;
	}

	phandle->back = NULL;
	phandle->size = 0;
	pthread_mutex_unlock(&phandle->lock);
}

int pushqueue(queue* phandle, qtype value)
{
	node* pnew;
	if(NULL == (pnew = (qnode*)malloc(sizeof(qnode))))
	{
		perror("pushqueue malloc error.");
		return -1;
	}

	pnew->data = value;
	pnew->next = NULL;

	pthread_mutex_lock(&phandle->lock);
	if(NULL == phandle->back)
		phandle->front = phandle->back = pnew;
	else
		phandle->back = phandle->back->next = pnew;

	phandle->size++;
	pthread_mutex_unlock(&phandle->lock);

	return 0;
}

qtype popqueue(queue* phandle)
{
	node* pnew = NULL;
	qtype val = NULL;

	pthread_mutex_lock(&phandle->lock);
	if(NULL == phandle->front)
	{
		pthread_mutex_unlock(&phandle->lock);
		return NULL;
	}

	val = phandle->front->data;
	pnew = phandle->front;
	phandle->front = pnew->next;
	phandle->size--;
	if(NULL == phandle->front)
		phandle->back = NULL;
	pthread_mutex_unlock(&phandle->lock);

	free(pnew);

	return val;
}

int lengthqueue(queue* phandle)
{
	return phandle->size;
}
