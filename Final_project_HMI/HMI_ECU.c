/*
 * HMI_ECU.c
 *
 *  Created on: Jul 22, 2024
 *      Author: DELL
 */


#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include "lcd.h"
#include "gpio.h"
#include <util/delay.h> /* For the delay functions */
#include <avr/io.h>

#define CONTROL_READY 0x10
#define HMI_REC 0x11
#define PASS_CORRECT 0x21
#define PASS_WRONG 0x20

#define NEW_PASS 0x31
#define OPEN_DOOR 0x32
#define IDLE 0x00
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

/*Functions Prototypes*/
void setPassword(void);
void sendPassword(void);
void enterpassword(void);
void re_enterpassword(void);
void OpenDoor(void);
void SetNewPassword(void);
void HoldKeypadLCD(void);
void Intializations(void);


/*Global Varaibles */
uint8 empty=0;
uint8 key = 0;
uint8 PassStatus = 0;
uint8 FalseTries = 0;
uint8 HoldTime = 0x00;
uint8 NoOfSeconds = 0;
uint8 DoorStatus = 0;
uint8 ChangePass = 0;
uint8 SuccessChange = 0;
uint8 MatchedPass = 0x01;
uint8 Pass1[5];
uint8 Pass2[5];


int main(void){

	Intializations();
	sendPassword();


	while(1){

		LCD_clearScreen();
		LCD_displayString("+ : Open Door ");
		LCD_moveCursor(1,0);
		LCD_displayString("- : change Pass ");
		key = KEYPAD_getPressedKey();

		if(key == '+')
		{
			OpenDoor();
		}

		else if(key == '-')
		{
			LCD_clearScreen();
			SetNewPassword();
		}

		_delay_ms(500);

}
}

void Intializations(void){
	SREG |= (1<<7);
	Timer1_setCallBack(HoldKeypadLCD);
	UART_ConfigType uart_configurations = {DATA_8_BIT,EVEN_PARITY,ONE_STOP_BIT,9600};
	UART_init(&uart_configurations);
	LCD_init();
	empty = 0x01;
}





void HoldKeypadLCD(void){

	NoOfSeconds++;

	if(NoOfSeconds == 60){
	HoldTime = 0x01;
	Timer1_deInit();
	NoOfSeconds = 0;
	}

}


void SetNewPassword(void){
	ChangePass = 1;
	OpenDoor();
	ChangePass = 0;
	if(PassStatus == PASS_CORRECT){
		UART_sendByte(NEW_PASS);
		sendPassword();
	}else{
		LCD_displayString("wrong password");
		_delay_ms(500);
	}


}



void OpenDoor(void){
	UART_sendByte(OPEN_DOOR);
	Timer1_ConfigType TimerConfigurations = {0, 7813, CLK_1024,CTC};
	FalseTries = 0;
	LCD_clearScreen();
	enterpassword();
	PassStatus = UART_recieveByte();
	while(FalseTries < 2 ){

				if(PassStatus == PASS_WRONG){
					LCD_clearScreen();
					FalseTries++;
					LCD_clearScreen();
					LCD_displayString("pass wrong ");
					_delay_ms(500);
					LCD_clearScreen();
					enterpassword();
					PassStatus = UART_recieveByte();
									}



				if(PassStatus == PASS_CORRECT){
					LCD_clearScreen();
					LCD_displayString("pass correct ");
					_delay_ms(500);
					LCD_clearScreen();
					if(ChangePass == 0){
						UART_sendByte(StartMotorActions);
					DoorStatus = UART_recieveByte();
					if(DoorStatus == UNLOCKING){
					LCD_displayString("Door is Unlocking");}
					while(UART_recieveByte() == UNLOCKING){

					}
					DoorStatus = UART_recieveByte();
					if(DoorStatus == HOLD){
					LCD_clearScreen();
					}
			while(UART_recieveByte() == HOLD){}
					DoorStatus = UART_recieveByte();
						if(DoorStatus == LOCKING){
							LCD_clearScreen();
						LCD_displayString("Door is Locking");}
						while(UART_recieveByte() == LOCKING){

											}
					}
					if(ChangePass == 1){
						UART_sendByte(NEW_PASS2);
					}


							LCD_clearScreen();
					FalseTries = 0;
					break;
				}
				if(FalseTries == 2){
					PassStatus = UART_recieveByte();
					Timer1_init(&TimerConfigurations);
					LCD_displayString("Hold for a min ");
					while(HoldTime != 0x01){
					}
					HoldTime = 0x00;
					DoorStatus = UART_recieveByte();
										if(DoorStatus == HOLD){
										LCD_clearScreen();
										}
					while(UART_recieveByte() == HOLD){}
					if(UART_recieveByte() == MENU){
							LCD_clearScreen();
							FalseTries = 0;
							break;
							}



				}


}


}


void sendPassword(void){
	do{
	setPassword();
	}while(MatchedPass == 0x00);
	MatchedPass = 0x01;

}


void setPassword(void){
	       enterpassword();
		   re_enterpassword();
			if(key == '=')
					{
				_delay_ms(500);
				LCD_clearScreen(); /* display the pressed keypad switch */
				LCD_displayString("passwords sent");
				_delay_ms(500);
				LCD_clearScreen();
					}


		}




void enterpassword(void){
	PassStatus = IDLE;
	uint8 counter = 0;
	uint8 key = 0;
	LCD_displayString("plz enter pass: ");
	LCD_moveCursor(1,0);


	while(counter < 5 ){

	key = KEYPAD_getPressedKey();
	if((key <= 9) && (key >= 0))
			{
		Pass1[counter] = key;
		LCD_displayString("*");   /* display the pressed keypad switch */
		UART_sendByte(key);
		_delay_ms(500);
			}
	if(!((key <= 9) && (key >= 0))){
		_delay_ms(500);
		continue;
	}
	counter++;



	}

	//PassStatus = UART_recieveByte(); it was working then stuck in set passwords then we commented it and put it in open door function
	key = KEYPAD_getPressedKey();
	while(key != '='){
		key = KEYPAD_getPressedKey();
		_delay_ms(500);
	}
	if(key == '='){
					LCD_clearScreen();
					_delay_ms(500);
					UART_sendByte(HMI_SENT);

				}
}

void re_enterpassword(void){
	uint8 counter = 0;
	uint8 i = 0;
	MatchedPass = 0x01;
	LCD_displayString("plz re-enter the: ");
					LCD_moveCursor(1,0);
					LCD_displayString("same pass: ");
					LCD_moveCursor(1,11);
					counter = 0;
					while(counter < 5 ){

					   key = KEYPAD_getPressedKey();
						if((key <= 9) && (key >= 0))
							{
							Pass2[counter] = key;

								LCD_displayString("*");   /* display the pressed keypad switch */
								UART_sendByte(key);
								_delay_ms(500);

								if(!((key <= 9) && (key >= 0))){
										_delay_ms(500);
										continue;
									}
							counter++;
							}

						}

					key = KEYPAD_getPressedKey();
						while(key != '='){
							key = KEYPAD_getPressedKey();
							_delay_ms(500);
						}
						if(key == '='){
										LCD_clearScreen();
										_delay_ms(500);
										UART_sendByte(HMI_SENT);
									}
						for(i = 0; i<5; i++){
										if(Pass1[i] == Pass2[i]){

											continue;
										}
										else{
											MatchedPass = 0x00;
											break;

										}

}
}

