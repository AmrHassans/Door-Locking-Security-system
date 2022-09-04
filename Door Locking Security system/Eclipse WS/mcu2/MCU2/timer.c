/******************************************************************************\
 *Module:Timer                                                                *
 *File Name: timer.c                                                          *
 *Description: Source file for the AVR Timer driver                           *
 *Author: Amr Hassan                                                          *
\******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include"common_macros.h"
#include "timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
static volatile void (*g_Timer0CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2CallBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/


/*************************************Timer0***********************************/

ISR(TIMER0_OVF_vect)
{
	if(g_Timer0CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer0CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


ISR(TIMER0_COMP_vect)
{
	if(g_Timer0CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer0CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*************************************Timer1***********************************/

ISR(TIMER1_OVF_vect)
{
	if(g_Timer1CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer1CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


ISR(TIMER1_COMPA_vect)
{
	if(g_Timer1CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer1CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*************************************Timer2***********************************/
ISR(TIMER2_OVF_vect)
{
	if(g_Timer2CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer2CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


ISR(TIMER2_COMP_vect)
{
	if(g_Timer2CallBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_Timer2CallBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer_init(const TIMER_ConfigType * config_Ptr)
{
	if (config_Ptr->Timer_ID == TIMER0)
	{
		TCNT0 = (config_Ptr->Init_Value); /* Set Timer Initial Value to 0*/

		if (config_Ptr->mode == NORMAL)
		{
			SET_BIT(TIMSK,OCIE0);
		}
		else if (config_Ptr->mode == CTC)
		{
			SET_BIT(TIMSK,TOIE0);


			/*Set Timer compare Value*/
			OCR0 = (config_Ptr->Comp_Value) ;
		}

		/* Configure timer control register
		 * 1.Overflow mode & Compare mode FOC0 = 1
		 * 2.clock = F_CPU/8
		 */

		SET_BIT(TCCR0,FOC0);

		/* CLOCK configuration */
		TCCR0 = (TCCR0 & 0xF8) | (config_Ptr->clock);

		/* MODE Configuration */
		TCCR0 = (TCCR0 & 0xB7) | (((config_Ptr->mode)&0x01)<<WGM00);
		TCCR0 = (TCCR0 & 0xB7) | (((config_Ptr->mode)&0x02)<<WGM01);

	}
	else if (config_Ptr -> Timer_ID == TIMER1)
	{
		TCCR1A =0;
		SET_BIT(TCCR1A,FOC1A); /* for non-PWM mode */
		SET_BIT(TCCR1A,FOC1B);
		TCCR1B = config_Ptr -> clock;
		TCNT1 = config_Ptr -> Init_Value;

		if (config_Ptr -> mode == NORMAL)
		{
			SET_BIT(TIMSK, TOIE1); /* enable interrupts for overflow mode */
			SET_BIT(TIFR,TOV1);

		}
		else if (config_Ptr -> mode == CTC)
		{
			SET_BIT(TCCR1B, WGM12);
			OCR1A = config_Ptr -> Comp_Value;
			SET_BIT(TIMSK, OCIE1A); /* enable interrupts for CTC mode */
			SET_BIT(TIFR,OCF1A); /* enable CTC flag */
		}
	}

	else if (config_Ptr -> Timer_ID == TIMER2)
	{
		if (config_Ptr -> mode == NORMAL)
		{
			SET_BIT(TIMSK, TOIE2); /* enable interrupts for overflow mode */
			SET_BIT(TIFR,TOV2); /* enable overflow flag */

		}
		else if (config_Ptr -> mode == CTC)
		{
			SET_BIT(TCCR0, WGM21);
			OCR2 = config_Ptr -> Comp_Value;
			SET_BIT(TIMSK, OCIE2); /* enable interrupts for CTC mode */
			SET_BIT(TIFR,OCF2); /* enable CTC flag */
		}

		SET_BIT(TCCR2, FOC2); /* for non-PWM */
		TCNT2 = config_Ptr -> Init_Value;
		TCCR2 = 0;
		TCCR2 |= (config_Ptr -> clock);


	}


}


/*************************************Timer0***********************************/

void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_Timer0CallBackPtr = a_ptr;
}


/*************************************Timer1***********************************/

void Timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_Timer1CallBackPtr = a_ptr;
}


/*************************************Timer2***********************************/

void Timer2_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_Timer2CallBackPtr = a_ptr;
}


/********************************************************************************/
void Timer_DeInit(TIMER_ID Timer_ID)
{
	if (Timer_ID == TIMER0)
	{
		TCCR0 = 0 ;
		TCNT0 = 0 ;
		OCR0 = 0;

		CLEAR_BIT(TIMSK,OCIE0) ;
		CLEAR_BIT(TIMSK,TOIE0) ;

		g_Timer0CallBackPtr = NULL_PTR;
	}
	else if (Timer_ID == TIMER1)
	{
		TCCR1A =0;

		/* disable clock */
		TCCR1B = 0;

		TCNT1 = 0;

		/* disable interrupts for overflow mode */
		CLEAR_BIT(TIMSK, TOIE1);

		/* clear compare value for CTC mode */
		OCR1A = 0;

		/* disable interrupts for CTC mode */
		CLEAR_BIT(TIMSK, OCIE1A);

		g_Timer1CallBackPtr = NULL_PTR;
	}
	else if (Timer_ID == TIMER2)
	{

		TCCR2 = 0;
		TCNT2 = 0; /* clear initial value */

		OCR2 = 0; /* clear compare value for CTC mode*/

		CLEAR_BIT(TIMSK, OCIE2); /* disable interrupts for CTC mode */
		CLEAR_BIT(TIMSK, TOIE2); /* disable interrupts for overflow mode */

		g_Timer2CallBackPtr = NULL_PTR;
	}

}
