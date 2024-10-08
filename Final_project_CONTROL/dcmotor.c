/*
 * dcmotor.c
 *
 *  Created on: Jul 2, 2024
 *      Author: DELL
 */
#include <util/delay.h> /* For the delay functions */
#include "common_macros.h" /* For GET_BIT Macro */
#include "dcmotor.h"
#include "gpio.h"
#include <avr/io.h>

void DcMotor_Init(void){
	GPIO_setupPinDirection(IN1_PORT,IN1_PIN , PIN_OUTPUT);
	GPIO_setupPinDirection(IN2_PORT,IN2_PIN , PIN_OUTPUT);
	GPIO_writePin( IN1_PORT, IN1_PIN , LOGIC_LOW);
	GPIO_writePin( IN2_PORT, IN2_PIN , LOGIC_LOW);

}
void DcMotor_Rotate(DcMotor_State state,uint8 speed){
	if(state == CW){
		GPIO_writePin( IN1_PORT, IN1_PIN , LOGIC_HIGH);
		GPIO_writePin( IN2_PORT, PIN2_ID , LOGIC_LOW);
		PWM_Timer0_Start(speed);

	}
	else if(state == A_CW){
		GPIO_writePin( IN1_PORT, IN2_PORT , LOGIC_LOW);
		GPIO_writePin( IN2_PORT, IN2_PIN , LOGIC_HIGH);
		PWM_Timer0_Start(speed);

	}
	else if(state == STOP){
		GPIO_writePin( IN1_PORT, IN1_PIN , LOGIC_LOW);
		GPIO_writePin( IN2_PORT, IN2_PIN , LOGIC_LOW);
		PWM_Timer0_Start(0);

	}



}



void PWM_Timer0_Start(uint8 set_duty_cycle){
	TCNT0 = 0; //Set Timer Initial value

		OCR0  = set_duty_cycle*255; // Set Compare Value

		DDRB  = DDRB | (1<<PB3); //set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.

		/* Configure timer control register
		 * 1. Fast PWM mode FOC0=0
		 * 2. Fast PWM Mode WGM01=1 & WGM00=1
		 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
		 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
		 */
		TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
