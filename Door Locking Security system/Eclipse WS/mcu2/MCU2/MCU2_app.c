/*
 ================================================================================================
 Name        : MCU2_app.c
 Author      : Amr Hassan
 Description : control-ECU
 Date        : 14/10/2021
 ================================================================================================
 */

#include <avr/io.h>
#include <util/delay.h>
#include "std_types.h"
#include "gpio.h"
#include "dc_motor.h"
#include "twi.h"
#include "external_eeprom.h"
#include "uart.h"
#include "timer.h"
#include "buzzer.h"
#include "MCU2_app.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void PASSCODEInit(void)
{
	uint8 confirmPasscode[PASSCODE_CH_LENGTH];
	uint8 check = 0;

	while(!check)
	{

		/* wait for a response from MCU1 */
		while (UART_reciveByte() != MC_READY_SEND);

		/* inform HMI that Control ECU ready to receive the passcode */
		UART_sendByte(MC_READY_RECIVE);
		recivePasscodeByUART (g_recivedPasscode);


		while (UART_reciveByte() != MC_READY_SEND);
		UART_sendByte(MC_READY_RECIVE);

		/* inform HMI to send the confirmation password */
		recivePasscodeByUART(confirmPasscode);

		if (comparePasscode(g_recivedPasscode, confirmPasscode) == PASSCODE_MATCH)
		{
			UART_sendByte(MC_READY_SEND);
			UART_sendByte(PASSCODE_MATCH);
			storePasscode();

			check = 1;
		}
		else
		{
			UART_sendByte(MC_READY_SEND);
			UART_sendByte(PASSCODE_NOT_MATCH);
		}
	}
}


void DoorOpen (void)
{
	/* OPENINNG THE DOOR */
	g_sec = 0;
	DcMotor_Rotate(CW);

	while (g_sec < DOOR_LOCK_UNLOCK_PERIOD);

	g_sec = 0;
	/* WAIT 3 SECS  */
	DcMotor_Rotate(STOP);
	while (g_sec < DOOR_STAND_BY_PERIOD);

	g_sec = 0;
	/*CLOSE THE DOOR */
	DcMotor_Rotate(A_CW);
	while (g_sec < DOOR_LOCK_UNLOCK_PERIOD);

	DcMotor_Rotate(STOP);
}


void recivePasscodeByUART (uint8 * pass_arr)
{
	uint8 count;

	for (count = 0; count < PASSCODE_CH_LENGTH; count++)
	{
		*(pass_arr+count) = UART_reciveByte();
		_delay_ms(100);
	}
}

void timerCallBack (void)
{
	g_sec++;
}

void storePasscode(void)
{
	uint8 i ;
	for (i=0 ;i<PASSCODE_CH_LENGTH ; i++)
	{
		EEPROM_writeByte(EEPROM_STORE_ADDREESS + i, g_recivedPasscode[i]);
		_delay_ms(100);
	}
}


void updateStoredPasscode(void)
{

	uint8 i;
	for (i = 0; i < PASSCODE_CH_LENGTH; i++)
	{
		EEPROM_readByte(EEPROM_STORE_ADDREESS+i, g_storedPasscode+i);
	}
}

uint8 comparePasscode(uint8 a_passcode1[PASSCODE_CH_LENGTH],uint8 a_passcode2[PASSCODE_CH_LENGTH])
{
	updateStoredPasscode();

	uint8 i;

	for(i = 0; i < PASSCODE_CH_LENGTH; i++)
	{
		if (a_passcode1[i] != a_passcode2[i])
		{
			return PASSCODE_NOT_MATCH;
		}
	}

	return PASSCODE_MATCH;
}


/*******************************************************************************
 *                                APPLICATION                                  *
 *******************************************************************************/

int main (void)
{
	/*enable global interrupt*/
	SREG |= (1 << 7);


	/*initialize Timer */

	TIMER_ConfigType Timer_Config = {TIMER1 , F_CPU_1024 , CTC, 7813 , 0};
	Timer_init(&Timer_Config);
	Timer1_setCallBack(timerCallBack);


	UART_ConfigType UART_Config= {DISABLED ,  _8_BIT , _1_BIT} ;

	UART_init(9600,&UART_Config)	;

	DcMotor_Init();

	BUZZER_init();

	PASSCODEInit();

	uint8 byteRecived = 0;



	while (1)
	{
		if (UART_reciveByte() == MC_READY_SEND)
		{
			recivePasscodeByUART(g_recivedPasscode);
			byteRecived = UART_reciveByte();

			if ( byteRecived == '+')
			{
				if (comparePasscode(g_storedPasscode, g_recivedPasscode) == PASSCODE_MATCH)
				{
					UART_sendByte(DOOR_UNLOOK); /* inform HMI ECU to display that door is unlocking */
					DoorOpen(); /* start opening door process/task */
				}
				else
				{
					UART_sendByte(PASSCODE_WRONG);
					/* count number of wrong attempts, and turn on a buzzer of it exceeds the limit */
					g_wrongAttemptCtr++;

					if (g_wrongAttemptCtr == NUMBER_OF_ALLOWED_WRONG_ATTEMPTS)
					{
						BUZZER_On();
						g_sec = 0;
						while(g_sec < ALARM_PERIOD); /* turn on alarm for a certain period */
						BUZZER_Off();
						g_wrongAttemptCtr = 0; /* reset the counter */
					}
				}


			}
			else if (byteRecived == CHANGE_PASSCODE_OPTION)
			{
				if (comparePasscode(g_storedPasscode, g_recivedPasscode) == PASSCODE_MATCH)
				{
					UART_sendByte(CHANGE_PASSCODE); /* inform HMI to process changing password */
					PASSCODEInit();
				}
				else
				{
					UART_sendByte(PASSCODE_WRONG);
					/* count number of wrong attempts, and turn on a buzzer of it exceeds the limit */
					g_wrongAttemptCtr++;

					if (g_wrongAttemptCtr == NUMBER_OF_ALLOWED_WRONG_ATTEMPTS)
					{
						BUZZER_On();
						g_sec = 0;
						while(g_sec < ALARM_PERIOD); /* turn on alarm for a certain period */
						BUZZER_Off();
						g_wrongAttemptCtr = 0; /* reset the counter */
					}
				}
			}
		}
	}
}

