/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *   - Initial version and ATmega168 support
 * Modfications Copyright (C) 2006 Tran Minh Hoang:
 *   - ATmega8, ATmega16, ATmega32 support
 *   - RS485 support for DS75176
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
 * File: $Id: portserial.c,v 1.6 2006/09/17 16:45:53 wolti Exp $
 */


#include <stdint.h>
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "hal.h"

#define MB_UART UARTD1

char rx_char = 0;
char tx_char = 0;

#define RTS_ENABLE 1
#define RTS_HIGH palSetPad(GPIOA, 12)
#define RTS_LOW  palClearPad(GPIOA, 12)
#define RTS_INIT RTS_LOW

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {

  (void)uartp;
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {

  (void)uartp;

  chSysLockFromISR();
  pxMBFrameCBTransmitterEmpty(  );
  chSysUnlockFromISR();
}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {

  (void)uartp;
  (void)e;
}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {

  (void)uartp;
  (void)c;

/*  chSysLockFromISR();
  rx_char = c;
  pxMBFrameCBByteReceived(  );
  chSysUnlockFromISR();
*/}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {

  (void)uartp;
  chSysLockFromISR();
  pxMBFrameCBByteReceived(  );
  uartStartReceiveI(&MB_UART, 1, &rx_char);
  chSysUnlockFromISR();
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg_1 = {
  txend1,
  txend2,
  rxend,
  rxchar,
  rxerr,
  921600,
  0,
  0,
  0
};

void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
#ifdef RTS_ENABLE
    //UCSRB |= _BV( TXEN ) | _BV(TXCIE);
#else

#endif

    if( xRxEnable )
    {
		uartStartReceiveI(&MB_UART, 1, &rx_char);
    }
    else
    {
    	uartStopReceiveI(&MB_UART);
    }

    if( xTxEnable )
    {
		pxMBFrameCBTransmitterEmpty(  );
#ifdef RTS_ENABLE
        RTS_HIGH;
#endif
    }
    else
    {
		uartStopSendI(&MB_UART);
#ifdef RTS_ENABLE
        RTS_LOW;
#endif
    }

}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	uart_cfg_1.speed = ulBaudRate;
	uartStart(&MB_UART, &uart_cfg_1);


#ifdef RTS_ENABLE
    RTS_INIT;
#endif


    return TRUE;
}

BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	tx_char = ucByte;
	uartStartSendI(&MB_UART, 1, &tx_char);

    return TRUE;
}

BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
	*pucByte = rx_char;

    return TRUE;
}


/*#ifdef RTS_ENABLE
SIGNAL( SIG_UART_TRANS )
{
    RTS_LOW;
}
#endif

*/
