/*
 * timer.c
 *
 *  Created on: Jul 25, 2024
 *      Author: DELL
 */
#include "std_types.h"
#include "timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>
static volatile void (*g_callBackPtr)(void) = NULL_PTR;


ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

void Timer1_init(const Timer1_ConfigType * Config_Ptr){
	TCNT1 = Config_Ptr->initial_value;
	if(Config_Ptr ->mode == CTC){
		TCCR1B |= (1<<WGM01);
		OCR1A = Config_Ptr->compare_value;
		TIMSK  |= (1<<OCIE1A);
		TCCR1A = (1<<FOC1A);
		TCCR1B = (TCCR1B) | (Config_Ptr -> prescaler);

	}
	if ((Config_Ptr -> mode) == NORMAL)
		{
			// Set prescaler
			TCCR1B = (TCCR1B) | (Config_Ptr -> prescaler);
			// Enable overflow interrupt
			TIMSK = (1<<TOIE1);

		}
}
	void Timer1_deInit(void)
	{
		TCCR1A =0 ;
		TCCR1B =0 ;
		TCNT1 = 0 ;
		OCR1A = 0 ;
		TIMSK = 0 ;
		//g_callBackPtr = NULL_PTR ;
	}
	void Timer1_setCallBack(void(*a_ptr)(void))
	{
		/* Save the address of the Call back function in a global variable */
		g_callBackPtr = a_ptr;


	}


