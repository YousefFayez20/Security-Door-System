/*
 * dcmotor.h
 *
 *  Created on: Jul 1, 2024
 *      Author: DELL
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include "std_types.h"

#define IN1_PORT PORTB_ID
#define IN2_PORT PORTB_ID
#define IN1_PIN PIN0_ID
#define IN2_PIN PIN1_ID
#define FULL_SPEED 1
#define OFF 0


typedef enum{
	CW,A_CW,STOP
}DcMotor_State;

void DcMotor_Init(void);


void DcMotor_Rotate(DcMotor_State state,uint8 speed);

void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* DCMOTOR_H_ */
