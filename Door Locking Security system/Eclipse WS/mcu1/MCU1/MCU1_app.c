/*
 ================================================================================================
 Name        : MCU1_app.c
 Author      : Amr Hassan
 Description : HMI-ECU
 Date        : 14/10/2021
 ================================================================================================
 */


#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "lcd.h"
#include "keypad.h"
#include "MCU1_app.h"
#include "timer.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/



void PASSCODE_Init (void)
{
	while (g_matchStatus == PASSCODE_NOT_MATCH)
	{
		LCD_clearScreen ();
		LCD_displayString ("Enter New CODE");
		LCD_moveCursor(1,0);

		/* Ask user to get the passcode */
		getPASSCODE(g_passcode);


		UART_sendByte(MC_READY_SEND);

		/* wait for a response */
		while (UART_reciveByte() != MC_READY_RECIVE);

		sendPasscodeByUART(g_passcode);

		/* confirm password Entered by user */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Re-Enter CODE:");
		LCD_moveCursor(1,0);

		getPASSCODE(g_passcode);
		UART_sendByte(MC_READY_SEND);

		while (UART_reciveByte() != MC_READY_RECIVE);
		sendPasscodeByUART(g_passcode);

		/* wait for a response from Control ECU about passwords matching */
		while (UART_reciveByte() != MC_READY_SEND);
		g_matchStatus = UART_reciveByte();

		if (g_matchStatus == PASSCODE_NOT_MATCH)
		{
			LCD_clearScreen();
			LCD_displayString("MisMatch !!");
			_delay_ms(DISPLAY_MESSAGE_DELAY);
		}
	}
	g_matchStatus = PASSCODE_NOT_MATCH;
}


void getPASSCODE (uint8 *arr )
{
	LCD_moveCursor(1,0);
	uint8 i = 0;
	uint8 key_num;

	while (i != PASSCODE_CH_LENGTH)
	{
		key_num = KEYPAD_getPressedKey();
		if (key_num >= 0 && key_num <= 9)
		{
			LCD_displayCharacter('*');
			*(arr + i) = key_num;
			i++;
		}
		_delay_ms(KEYPAD_INPUT_DELAY);
	}
	key_num = 0;

	/* stay till user press"ON/C" Enter key */
	while(KEYPAD_getPressedKey() != 13);
}

void MainMenu(void)
{
	LCD_clearScreen();

	LCD_displayStringRowColumn(0,0, "+:OPEN");
	LCD_displayStringRowColumn(1,0,"-:Change code");
}

void sendPasscodeByUART (uint8 * pass_arr)
{
	uint8 ctr;

	for (ctr = 0; ctr < PASSCODE_CH_LENGTH; ctr++)
	{
		UART_sendByte(pass_arr[ctr]);
		_delay_ms(100);
	}
}

void timerCallBack(void)
{
	g_sec++;
}


void DoorOpen (void)
{
	g_sec = 0 ;
	/* door is opening during 15 sec. */
	LCD_clearScreen();

	LCD_displayStringRowColumn(0,0,"Door is Opening");
	while (g_sec < DOOR_LOCK_UNLOCK_PERIOD);


	g_sec = 0 ;
	/* door is held open for 3 sec.  */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door Opened !");
	while (g_sec < DOOR_STAND_BY_PERIOD);


	g_sec = 0 ;
	/* door is closing during 15 sec. */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Door is Closing");
	while (g_sec < DOOR_LOCK_UNLOCK_PERIOD);
}






/*******************************************************************************
 *                                APPLICATION                                  *
 *******************************************************************************/


int main (void)
{
	/* Enable Interrupt */
	SREG |= (1<<7);

	/*initialize LCD and UART */
	LCD_init();



	UART_ConfigType UART_Config= {DISABLED , _8_BIT , _1_BIT} ;
	/* Choose baud rate = 9600 bps */
	UART_init(9600,&UART_Config)	;


	/*initialize Timer */

	/* Timer frequency = 8MHz/1024
	 * one clock-cycle time = 128 uSec
	 * timer to produce an interrupt every 1 second:
	 * set the compare value to be 1/(128u) = 7813
	 */

	TIMER_ConfigType Timer_Config = {TIMER1 , F_CPU_1024 , CTC, 7813 , 0};

	Timer_init(&Timer_Config);
	Timer1_setCallBack(timerCallBack);

	PASSCODE_Init();
	MainMenu();

	uint8 byteRecived = 0;
	uint8 keyPressed = 0;


	while (1)
	{
		keyPressed =KEYPAD_getPressedKey();
		if (keyPressed == '+')
		{
			LCD_clearScreen();
			LCD_displayString("Enter CODE");

			getPASSCODE(g_passcode);


			/* MC1 is ready to send */
			UART_sendByte(MC_READY_SEND);
			sendPasscodeByUART(g_passcode);

			UART_sendByte('+');

			/* MCU get the byte or no  */
			byteRecived = UART_reciveByte();
			if (byteRecived == DOOR_UNLOOK)
			{
				/* display door status on LCD */
				DoorOpen();

			}
			else if (byteRecived == PASSCODE_WRONG)
			{
				/* Clear the screen and display wrong passcode */
				LCD_clearScreen();
				LCD_displayString("Wrong PASSCODE !");
				_delay_ms(DISPLAY_MESSAGE_DELAY);
				LCD_displayStringRowColumn(1,0,"Enter Correct code");
				_delay_ms(DISPLAY_MESSAGE_DELAY);


			}
			MainMenu(); /* system back to  display main menu */


		}
		else if (keyPressed == '-')
		{
			LCD_clearScreen();
			LCD_displayString("Enter Old CODE");

			getPASSCODE(g_passcode);

			/* start sending to Control ECU  */
			UART_sendByte(MC_READY_SEND);

			sendPasscodeByUART(g_passcode);

			/* inform Control ECU the option that user chose */
			UART_sendByte(CHANGE_PASSCODE_OPTION);

			byteRecived = UART_reciveByte();
			if (byteRecived == CHANGE_PASSCODE)
			{
				PASSCODE_Init();
				LCD_clearScreen();
			}
			else if (byteRecived == PASSCODE_WRONG)
			{
				LCD_clearScreen();
				LCD_displayString("Wrong PASSCODE");
				_delay_ms(DISPLAY_MESSAGE_DELAY);
				LCD_displayStringRowColumn(1,0,"Enter Correct code");
				_delay_ms(DISPLAY_MESSAGE_DELAY);
			}
			MainMenu();
		}
	}
}


















