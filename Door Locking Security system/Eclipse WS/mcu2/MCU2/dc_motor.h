/******************************************************************************\
 *Module: DC Motor                                                            *
 *File Name: dc_motor.h                                                       *
 *Description: Header file for the AVR DC Motor driver                        *
 *Author: Amr Hassan                                                          *
\******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DC_MOTOR_PORT_ID                      PORTC_ID


#define DC_MOTOR_IN1_PIN_ID                   PIN3_ID
#define DC_MOTOR_IN2_PIN_ID                   PIN4_ID







/*******************************************************************************
 *                                Decelerations                                *
 *******************************************************************************/

typedef enum
{
	STOP , CW , A_CW
}DcMotor_State;




/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 *
 *   1. The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 *   2. Stop at the DC-Motor at the beginning through the GPIO driver.
 */


void DcMotor_Init(void);




void DcMotor_Rotate(DcMotor_State state);



#endif /* DC_MOTOR_H_ */
