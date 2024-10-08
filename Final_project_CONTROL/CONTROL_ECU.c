/*
 * CONTROL_ECU.c
 *
 *  Created on: Jul 22, 2024
 *      Author: DELL
 */



#include "uart.h"
#include <util/delay.h>
#include "twi.h"
#include "timer.h"
#include "dcmotor.h"
#include <avr/io.h>
#include "external_eeprom.h"
#include "buzzer.h"
#include "lcd.h"



#define CONTROL_READY 0x10
#define HMI_REC 0x11
#define PASS_CORRECT 0x21
#define PASS_WRONG 0x20
#define IDLE 0x00

#define NEW_PASS 0x31
#define OPEN_DOOR 0x32
#define MENU 0x33
#define HMI_SENT 0x34
#define SUCCESS_CHANGE 0x35
#define FAILED_CHANGE 0x36
#define RESPONSE_PASS 0x37


#define UNLOCKING 0x40
#define LOCKING 0x41
#define HOLD 0x42
#define PASS_LIMIT 0x43
#define StartMotorActions 0x44
#define NEW_PASS2 0x45



void check_newpassword(void);
void check_password(void);
void MotorActions(void);
void BuzzerActions(void);
void Initializations(void);



uint8 isEmpty = 0;
uint8 FalseTries = 0;
uint8 status = 0;
uint8 ActionsDone = 0;
uint8 ActionsBuzzer = 0;
uint8 NoOfSeconds = 0;
uint8 MatchedPass = 0x01;

int main(void){

	Initializations();

	check_newpassword();
	LCD_clearScreen();

	while(1){


		status =  UART_recieveByte();
		if(status == OPEN_DOOR){
		check_password();
		}
		else if(status == NEW_PASS){
			do{
				check_newpassword();
				 LCD_clearScreen();
				}while((MatchedPass != 0x01));

		}
		status = IDLE;


	}
}




void Initializations(void){
	TWI_init();
		UART_ConfigType uart_configurations = {DATA_8_BIT,EVEN_PARITY,ONE_STOP_BIT,9600};
		UART_init(&uart_configurations);
		LCD_init();
		SREG |= (1<<7);

		DcMotor_Init();
		isEmpty = 0x01;

}




void BuzzerActions(void){
	NoOfSeconds++;
	if(NoOfSeconds < 60){
		UART_sendByte(PASS_LIMIT);
		Buzzer_on();
			}
	else{
			Buzzer_off();
		Timer1_deInit();
		NoOfSeconds = 0;
		ActionsBuzzer = 0x01;
		UART_sendByte(HOLD);
		}

	}


void MotorActions(void){
	NoOfSeconds++;
	LCD_clearScreen();
	if(NoOfSeconds < 15){
		UART_sendByte(UNLOCKING);

	DcMotor_Rotate(CW,FULL_SPEED);
	}
	else if(NoOfSeconds >= 15 && NoOfSeconds < 18 ){
		UART_sendByte(HOLD);
		DcMotor_Rotate(STOP,OFF);
	}
	else if(NoOfSeconds >= 18 && NoOfSeconds < 33){
		UART_sendByte(LOCKING);
		DcMotor_Rotate(A_CW,FULL_SPEED);
	}
	else{
	ActionsDone = 0x01;
	NoOfSeconds = 0;
	Timer1_deInit();
	UART_sendByte(HOLD);
	DcMotor_Rotate(STOP,OFF);

	}
}



void check_password(void){
        uint8 key[5];
    	uint8 val[5];
		uint8 i = 0;
		uint8 correct = 0x01;

	    Timer1_ConfigType TimerConfigurations = {0, 7813, CLK_1024,CTC};

while(FalseTries < 3){
	    correct = 0x01;
		LCD_displayString(" password: ");
		for(i = 0; i<5; i++){

			key[i] =UART_recieveByte();

			LCD_intgerToString(key[i]);
		}

		while(UART_recieveByte() != HMI_SENT);

		 EEPROM_readByte(0x0311, &val[0]);  /* Read 0x0F from the external EEPROM */
		 EEPROM_readByte(0x0312, &val[1]);  /* Read 0x0F from the external EEPROM */
		 EEPROM_readByte(0x0313, &val[2]);  /* Read 0x0F from the external EEPROM */
		 EEPROM_readByte(0x0314, &val[3]);  /* Read 0x0F from the external EEPROM */
		 EEPROM_readByte(0x0315, &val[4]);  /* Read 0x0F from the external EEPROM */
		 for(i = 0; i<5; i++){

		 			if(key[i] == val[i]){

		 				//continue;
		 			}
		 			else{
		 				correct = 0x00;
		 				//break;
		 			}

		 		}
		 LCD_clearScreen();

		 if (correct == 0x01){
			 LCD_displayString("Matched");
			 UART_sendByte(PASS_CORRECT);
			 _delay_ms(100);
			 LCD_clearScreen();
			 FalseTries = 0;
			 if(UART_recieveByte() == StartMotorActions){
			 Timer1_setCallBack(MotorActions);
			 Timer1_init(&TimerConfigurations);
			 while(ActionsDone != 0x01){
			 }
			 ActionsDone = 0x00;
			 LCD_clearScreen();
			 }
			 else if(UART_recieveByte() == NEW_PASS){
				 check_newpassword();
				 LCD_clearScreen();

			 }
			 break;

			 //Correct actions
		 }
		 else{
			 FalseTries++;
			 LCD_displayString("Not Matched");
			 UART_sendByte(PASS_WRONG);
		 }

			if(FalseTries == 3){
				LCD_displayString("Limit Reached");
				_delay_ms(500);
				LCD_clearScreen();

				Timer1_setCallBack(BuzzerActions);
				Timer1_init(&TimerConfigurations);
				 while(ActionsBuzzer != 0x01){
				}
				ActionsBuzzer = 0x00;
				LCD_clearScreen();
				UART_sendByte(MENU);
				//UART_sendByte(MENU);
					//Activate Buzzer for 1-minute.
                   //Display error message on LCD for 1 minute.
				// System should be locked no inputs from Keypad will be accepted during
				//this time period.
				//Go to Step 2 the main options again
				}
			_delay_ms(500);
			LCD_clearScreen();
}
FalseTries = 0;
}

void check_newpassword(void){
	        uint8 key[5];
			uint8 key2[5];
			uint8 i = 0;
			MatchedPass = 0x00;
			while((MatchedPass == 0x00)){
			MatchedPass = 0x01;
			LCD_displayString(" pass 1: ");
			for(i = 0; i<5; i++){

				key[i] = UART_recieveByte();

				LCD_intgerToString(key[i]);
			}
			while(UART_recieveByte() != HMI_SENT);
			LCD_moveCursor(1,0);
			LCD_displayString(" pass 2: ");
			for(i = 0; i<5; i++){


				key2[i] =UART_recieveByte();

				LCD_intgerToString(key2[i]);
			}

			while(UART_recieveByte() != HMI_SENT){}
			for(i = 0; i<5; i++){
				if(key[i] == key2[i]){
				}
				else{
					MatchedPass = 0x00;
					break;
				}
						}
			LCD_clearScreen();
			if(MatchedPass == 0x01){
				LCD_displayString("passwords Matched ");
				//save to eeprom
				EEPROM_writeByte(0x0311, key[0]); /* Write 0x0F in the external EEPROM */
				_delay_ms(10);
				EEPROM_writeByte(0x0312, key[1]); /* Write 0x0F in the external EEPROM */
				_delay_ms(10);
				EEPROM_writeByte(0x0313, key[2]); /* Write 0x0F in the external EEPROM */
			    _delay_ms(10);
				EEPROM_writeByte(0x0314, key[3]); /* Write 0x0F in the external EEPROM */
				_delay_ms(10);
				EEPROM_writeByte(0x0315, key[4]); /* Write 0x0F in the external EEPROM */
				_delay_ms(10);
				MatchedPass = 0x01;
				LCD_clearScreen();
			}
			else{
				LCD_displayString("Not Matched");
				MatchedPass = 0x00;

				_delay_ms(500);
				LCD_clearScreen();
			}

			}
			_delay_ms(500);


		}






