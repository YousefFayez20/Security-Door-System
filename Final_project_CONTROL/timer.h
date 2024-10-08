/*
 * timer.h
 *
 *  Created on: Jul 25, 2024
 *      Author: DELL
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef enum{
	NO_CLK,CLK,CLK_8,CLK_64,CLK_256,CLK_1024
}Timer1_Prescaler;
typedef enum{
	NORMAL,CTC ,PWM,FAST_PWM
}Timer1_Mode;

typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;


void Timer1_init(const Timer1_ConfigType * Config_Ptr);

void Timer1_deInit(void);

void Timer1_setCallBack(void(*a_ptr)(void));
#endif /* TIMER_H_ */
