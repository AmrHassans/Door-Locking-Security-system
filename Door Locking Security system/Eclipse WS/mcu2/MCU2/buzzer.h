/******************************************************************************\
 *Module: DC Motor                                                            *
 *File Name: buzzer.h                                                         *
 *Description: Source file for the AVR DC Motor driver                        *
 *Author: Amr Hassan                                                          *
\******************************************************************************/


#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/*Buzzer Port Configurations */
#define BUZZER_PORT_ID                     PORTC_ID

/*Buzzer Pin Configurations */
#define Buzzer_PIN_ID                      PIN2_ID





/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the BUZZER:
 *
 */
void BUZZER_init(void);

void BUZZER_On (void);

void BUZZER_Off (void);



#endif /* BUZZER_H_ */
