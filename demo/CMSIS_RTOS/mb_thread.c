/*
 * mb_thread.c
 *
 *  Created on: 08 окт. 2016 г.
 *      Author: Bogdan
 */

#include "cmsis_os.h"
#include "mb.h"

osThreadId mb_thread_ID;
void mb_thread (void const *argument) {

	while(1) {
		eMBPoll();
	}
}
osThreadDef(mb_thread, osPriorityNormal, 128);

void xMBPortPollThreadInit( void ) {
	mb_thread_ID = osThreadCreate(osThread(mb_thread), NULL);
}

