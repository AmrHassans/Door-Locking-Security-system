/******************************************************************************\
 *Module: DC Motor                                                            *
 *File Name: dc_motor.c                                                       *
 *Description: Source file for the AVR DC Motor driver                        *
 *Author: Amr Hassan                                                          *
\******************************************************************************/

#include"dc_motor.h"
#include"gpio.h"



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


void DcMotor_Init(void)
{

	/*  The Function responsible for setup the direction for the two motor pins through the GPIO driver.	 */
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_MOTOR_PORT_ID, DC_MOTOR_IN2_PIN_ID, PIN_OUTPUT);




	/* Stop at the DC-Motor at the beginning through the GPIO driver. */
	GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);


}


void DcMotor_Rotate(DcMotor_State state)
{

	switch(state)
	{
	case STOP:
/*Stop the Motor*/
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);

		break;
	case CW:
		/*Rotate the Motor Clockwise*/

		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_HIGH);
		break;
	case A_CW:
		/*Rotate the Motor Anti-Clockwise*/

		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_HIGH);
		GPIO_writePin(DC_MOTOR_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);
		break;
	}



}



