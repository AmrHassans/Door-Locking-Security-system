/******************************************************************************\
 *Module: DC Motor                                                            *
 *File Name: buzzer.c                                                         *
 *Description: Source file for the AVR DC Motor driver                        *
 *Author: Amr Hassan                                                          *
\******************************************************************************/


#include "common_macros.h"
#include "gpio.h"
#include "buzzer.h"






void BUZZER_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID,Buzzer_PIN_ID,PIN_OUTPUT);

}


void BUZZER_On(void)
{
	GPIO_writePin(BUZZER_PORT_ID, Buzzer_PIN_ID, LOGIC_HIGH);

}

void BUZZER_Off(void)
{
	GPIO_writePin(BUZZER_PORT_ID, Buzzer_PIN_ID, LOGIC_LOW);

}
