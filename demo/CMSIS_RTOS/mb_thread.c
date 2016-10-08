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

	eMBInit( MB_RTU, 1, 1, 115200, MB_PAR_NONE );

	eMBEnable();

	while(1) {
		eMBPoll();
	}
}
osThreadDef(mb_thread, osPriorityNormal, 128);



