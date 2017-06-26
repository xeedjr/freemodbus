/*
 * mb_thread.c
 *
 *  Created on: 08 окт. 2016 г.
 *      Author: Bogdan
 */

#include "cmsis_os.h"
#include "mb.h"
#include "hal.h"

osThreadId mb_thread_ID = {0};
void mb_thread (void const *argument) {

	while(1) {
		eMBPoll();
	}
}
osThreadDef(mb_thread, USER_MB_THREAD_PRIORITY, USER_MB_THREAD_WORK_AREA_SIZE);

void xMBPortPollThreadInit( void ) {
	mb_thread_ID = osThreadCreate(osThread(mb_thread), NULL);
	if (mb_thread_ID == NULL) {
		abort();
	}
	mb_thread_ID->p_name = "mb_thread";
}

