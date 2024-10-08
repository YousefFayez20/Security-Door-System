/*
 * buzzer.h
 *
 *  Created on: Jul 27, 2024
 *      Author: DELL
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"
#include "std_types.h"
#include "common_macros.h"


#define BUZZER_PORT PORTD_ID
#define BUZZER_PIN PIN7_ID

void Buzzer_init(void);


void Buzzer_on(void);


void Buzzer_off(void);



#endif /* BUZZER_H_ */
