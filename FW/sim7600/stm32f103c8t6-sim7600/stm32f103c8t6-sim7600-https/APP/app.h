#ifndef	_APP_H_
#define	_APP_H_

#include "stdint.h"
#include "ucos_ii.h"

#define QUEUE_FULL    1
#define QUEUE_EMPTY   1

/**************** �û��������� *******************/
void Task_Start(void *p_arg);
//void Task_LED2(void *p_arg);


int is_queue_full(OS_EVENT *pevent);
int is_queue_empty(OS_EVENT *pevent);

#endif	//_APP_H_
