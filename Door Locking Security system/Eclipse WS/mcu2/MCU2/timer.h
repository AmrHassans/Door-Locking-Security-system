/******************************************************************************\
 *Module:Timer                                                                *
 *File Name: timer.h                                                          *
 *Description: Header file for the AVR Timer driver                           *
 *Author: Amr Hassan                                                          *
\******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"



/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

typedef enum
{
	TIMER0 , TIMER1 , TIMER2
}TIMER_ID;


typedef enum
{
	NO_CLOCK , F_CPU_CLOCK , F_CPU_8 , F_CPU_64 , F_CPU_256 , F_CPU_1024
}TIMER_CLOCK;

typedef enum
{
	NORMAL , PWM , CTC = 2 , FAST_PWM
}TIMER0_MODE;

typedef struct
{
	TIMER_ID Timer_ID;
	TIMER_CLOCK clock;
	TIMER0_MODE mode;
	uint32 Comp_Value;
	uint32 Init_Value;


}TIMER_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 *
 *     1. The function responsible for trigger the Timer with the non-PWM Mode.
 *     2. Setup the prescaler with F_CPU/8.
 */

void Timer_init(const TIMER_ConfigType * config_Ptr);


void Timer0_setCallBack(void(*a_ptr)(void));
void Timer1_setCallBack(void(*a_ptr)(void));
void Timer2_setCallBack(void(*a_ptr)(void));



void Timer_DeInit(TIMER_ID Timer_ID);

#endif /* TIMER_H_ */
