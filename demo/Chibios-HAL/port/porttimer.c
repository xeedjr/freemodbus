/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.4 2006/09/03 11:53:10 wolti Exp $
 */

/* ----------------------- AVR includes -------------------------------------*/

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "hal.h"

#define TIMER GPTD1

/* ----------------------- Defines ------------------------------------------*/
static void gpt2cb(GPTDriver *gptp);

/* ----------------------- Static variables ---------------------------------*/
static GPTConfig gpt2cfg =
{
  20000,         /* Timer clock.    */
  gpt2cb        /* Timer callback. */
};
static USHORT   usTim1Timerout50us_;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{

	gptStart(&TIMER, &gpt2cfg);

	usTim1Timerout50us_ = usTim1Timerout50us;

    vMBPortTimersDisable(  );

    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
	 gptStartOneShotI(&TIMER, usTim1Timerout50us_);
}


inline void
vMBPortTimersDisable(  )
{
	gptStopTimerI(&TIMER);
}

void vMBPortTimersDelay( USHORT usTimeOutMS )
{
	gptPolledDelay(&TIMER, 20 * usTimeOutMS);
}

static void gpt2cb(GPTDriver *gptp)
{
  ( void )pxMBPortCBTimerExpired(  );
}

