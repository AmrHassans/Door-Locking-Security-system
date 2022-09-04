/******************************************************************************\
 *Module: UART                                                                *
 *File Name: uart.h                                                           *
 *Description: Header file for the AVR UART driver                            *
 *Author: Amr Hassan                                                          *
\******************************************************************************/


#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "common_macros.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

typedef enum
{
	DISABLED , EVEN=2 , ODD=3
}PARITY_TYPE;

typedef enum
{
	_5_BIT, _6_BIT, _7_BIT, _8_BIT
}CHAR_SIZE;

typedef enum
{
	_1_BIT , _2_BIT
}STOP_BIT_SELECT;


typedef struct
{
	PARITY_TYPE parity ;
	CHAR_SIZE DATA_BIT_NUM;
	STOP_BIT_SELECT STOP_BIT ;
}UART_ConfigType;




/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(uint32 baud_rate, UART_ConfigType * config_Ptr);



void UART_sendByte(const uint8 data);

uint8 UART_reciveByte(void);

void UART_sendString(const uint8 *Str);


void UART_receiveString(uint8 *Str);


#endif /* UART_H_ */
