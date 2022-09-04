/******************************************************************************\
 *Module: UART                                                                *
 *File Name: uart.c                                                           *
 *Description: Source file for the AVR UART driver                            *
 *Author: Amr Hassan                                                          *
\******************************************************************************/

#include "uart.h"
#include "avr/io.h"
#include "common_macros.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/



void UART_init(uint32 baud_rate, UART_ConfigType * config_Ptr)
{

	uint16 ubrr_value = 0 ;

	/* U2X = 1 for double transmission speed */
 	SET_BIT(UCSRA,U2X);

	SET_BIT(UCSRB,TXEN);
	SET_BIT(UCSRB,RXEN);

	UCSRC = (UCSRC & 0xF9) | ((config_Ptr->DATA_BIT_NUM) <<  UCSZ0);


	UCSRC = (UCSRC & 0xCF) | ((config_Ptr->parity) <<  UPM0);


	UCSRC = (UCSRC & 0xF7) | ((config_Ptr->STOP_BIT) <<  USBS);




	ubrr_value = (uint16)(  ( (F_CPU / ((baud_rate) * 8UL) )  )- 1);

	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}



void UART_sendByte(const uint8 data)
{
	while(BIT_IS_CLEAR(UCSRA , UDRE)){}
	UDR = data;
}

uint8 UART_reciveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA , RXC)){}
	return UDR;
}



void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0 ;

	Str[i] = UART_reciveByte();

	while (Str[i] != '#')
	{
		i++;
		Str[i] = UART_reciveByte();

	}
	Str[i] = '\0';

}
