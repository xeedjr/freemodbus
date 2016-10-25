/*
 * mb_thread.c
 *
 *  Created on: 08 окт. 2016 г.
 *      Author: Bogdan
 */

#include "cmsis_os.h"
#include "mb.h"

osThreadId mb_thread_ID = {0};
void mb_thread (void const *argument) {

	while(1) {
		eMBPoll();
	}
}
osThreadDef(mb_thread, osPriorityNormal, 256);

void xMBPortPollThreadInit( void ) {
	mb_thread_ID = osThreadCreate(osThread(mb_thread), NULL);
	mb_thread_ID->p_name = "mb_thread";
}

