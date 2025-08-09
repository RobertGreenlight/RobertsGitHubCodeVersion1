/*
 * SYBT5_Routines.h
 *
 * Created: 7/29/2024 9:10:22 AM
 *  Author: Robert F
 */ 

#ifndef SYBT5_ROUTINES_H_
#define SYBT5_ROUTINES_H_
/*
#include <stdint-gcc.h>  //
#include <stdbool.h>	//
#include <stddef.h>		//
#include <float.h>		//
#include <adc_basic.h>		//
#include <atmel_start.h>		//
#include <util/delay.h>		//
#include <stdint-gcc.h>		//
#include <string.h>		//
#include <stdio.h>		//
#include "seven_seg_led.h"
#include "bobs_functions.h"
*/
#include "bobs_functions.h"
#include "Seven_Seg_LED.h"

#include "adc_basic.h"
#include "atmel_start.h"
#include <float.h>
#include <stdbool.h>
#include <stddef.h>
#include "stdio.h"
#include <stdint-gcc.h>
#include "string.h"
#include "util/delay.h"




#define LCD_L1_1 0x80
#define LCD_L1_2 0x81
#define LCD_L1_3 0x82
#define LCD_L1_4 0x83
#define LCD_L1_5 0x84
#define LCD_L1_6 0x85
#define LCD_L1_7 0x86
#define LCD_L1_8 0x87
#define LCD_L1_9 0x88
#define LCD_L1_10 0x89
#define LCD_L1_11 0x8A
#define LCD_L1_12 0x8B
#define LCD_L1_13 0x8C
#define LCD_L1_14 0x8D
#define LCD_L1_15 0x8E
#define LCD_L1_16 0x8F
#define LCD_L1_17 0x90
#define LCD_L1_18 0x91
#define LCD_L1_19 0x92
#define LCD_L1_20 0x93
#define LCD_L2_1 0xC0
#define LCD_L2_2 0xC1
#define LCD_L2_3 0xC2
#define LCD_L2_4 0xC3
#define LCD_L2_5 0xC4
#define LCD_L2_6 0xC5
#define LCD_L2_7 0xC6
#define LCD_L2_8 0xC7
#define LCD_L2_9 0xC8
#define LCD_L2_10 0xC9
#define LCD_L2_11 0xCA
#define LCD_L2_12 0xCB
#define LCD_L2_13 0xCC
#define LCD_L2_14 0xCD
#define LCD_L2_15 0xCE
#define LCD_L2_16 0xCF
#define LCD_L2_17 0xD0
#define LCD_L2_18 0xD1
#define LCD_L2_19 0xD2
#define LCD_L2_20 0xD3
#define LCD_L3_1 0x94
#define LCD_L3_2 0x95
#define LCD_L3_3 0x96
#define LCD_L3_4 0x97
#define LCD_L3_5 0x98
#define LCD_L3_6 0x99
#define LCD_L3_7 0x9A
#define LCD_L3_8 0x9B
#define LCD_L3_9 0x9C
#define LCD_L3_10 0x9D
#define LCD_L3_11 0x9E
#define LCD_L3_12 0x9F
#define LCD_L3_13 0xA0
#define LCD_L3_14 0xA1
#define LCD_L3_15 0xA2
#define LCD_L3_16 0xA3
#define LCD_L3_17 0xA4
#define LCD_L3_18 0xA5
#define LCD_L3_19 0xA6
#define LCD_L3_20 0xA7
#define LCD_L4_1 0xD4
#define LCD_L4_2 0xD5
#define LCD_L4_3 0xD6
#define LCD_L4_4 0xD7
#define LCD_L4_5 0xD8
#define LCD_L4_6 0xD9
#define LCD_L4_7 0xDA
#define LCD_L4_8 0xDB
#define LCD_L4_9 0xDC
#define LCD_L4_10 0xDD
#define LCD_L4_11 0xDE
#define LCD_L4_12 0xDF
#define LCD_L4_13 0xE0
#define LCD_L4_14 0xE1
#define LCD_L4_15 0xE2
#define LCD_L4_16 0xE3
#define LCD_L4_17 0xE4
#define LCD_L4_18 0xE5
#define LCD_L4_19 0xE6
#define LCD_L4_20 0xE7

#define CommDelay 120

#define SetEEPROMaddress 0x03 
#define WriteEEPROM 0x05                        //
#define ReadEEPROM 0x07                        //
#define ReadTemperature 0x09                   //
#define ReadSwitchPosition 0x0B                //

#define GotoStartAddress 0x00	                //
#define GotoShuntValLoAddress 0x02              //
#define GotoShuntValHiAddress 0x03              //
#define GotoWatthoursLoAddress 0x04             //  
#define GotoWatthoursHiAddress 0x05             //  
#define GotoMaxPwrLoAddress 0x08			    //
#define GotoMaxPwrHiAddress 0x09				//
#define GotoSerialNumberAddress 0x2C            //
#define GotoSYBTnameAddress 0x3C                //
#define GotoSYBTdateAddress 0x48				//
#define GotoErrorCheckBitLoAddress 0x50         //  80 decimal
#define GotoErrorCheckBitHiAddress 0x51         //  81 decimal
#define GotoEEPROMaddress64 0x64                // 
#define GotoCalFactorAddress 0x67               //
#define GotoHighTemperatureAddress 0x76
#define GotoGLcustomAddress 0x80

uint8_t compiledBatteryString[90];
uint8_t savedBatteryString[80];
uint8_t compiledDataString[40];
//uint8_t savedDataString[40];
//uint8_t revisedDataString[100];
//uint8_t compiledString[100];
uint8_t savedBatteryStringBitPosition;

uint8_t savedTemperatureString[4];
uint8_t savedSwitchPosition;
uint8_t savedHighTemperature;
uint8_t savedCalFactor;

char inputString[35];
uint8_t inputCount;


void WriteToSYBT(uint8_t Command);
uint8_t ReadSYBT(uint8_t readback);
bool GetDataFromSYBT(uint8_t Command);
bool GoToAddress(uint8_t Address);
bool GetSYBT5serialNumber(void);
bool GetSYBTname(void);
bool GetSYBTdate(void);
bool GetPrimaryEEPROM(void);
bool GetDataEEPROM(void);
bool GetSYBT5emperature(void);
bool GetSYBT5shuntValue(void);
bool GetSYBT5highTemperature(void);
bool GetSYBT5switchPosition(void);
bool GetSYBT5calFactor(void);
bool GetSYBT5shuntValue(void);

uint16_t CalculateErrorCheckBits(bool CalculateOrCheck,bool Save);

bool USART0_sendString(char *str);	
void USART0_getString(void);
bool USART0_sendHEX(char *str,uint8_t length);

bool USART_0_flush(void);
bool USART_1_flush(void);

void init_st7066u(void);
void WriteToDisplayPort(uint8_t byte);
void SendCommandToDisplayPort(uint8_t byte); 
void clear_display(void);
void return_home(void);

bool CollectionComplete;
bool SerialnumberOrCalFactor;
uint8_t bytecount;
uint8_t linecount;
volatile uint8_t SYBTreturnedData;

uint8_t Rows;

uint32_t loop;
uint8_t column;

uint8_t ConvertShuntBitsToValue(uint8_t H,uint8_t L);
void ConvertValueToShuntBits(uint8_t ShuntVal,uint8_t *H,uint8_t *L );

//void identify_commands(uint8_t buffer[35]);   //from standalone SYBT5 program

void ResetSYBTdate(uint8_t buffer[35]);
void ResetSYBTserialNumber(uint8_t buffer[35]);
void ResetSYBTname(uint8_t buffer[35]);
void ResetCalFactor(uint8_t buffer[35]);

void BlankDisplay(void);
bool IsConnectedToSYBT5(void);
bool LookForCommands(void);
bool LookForAnyChanges(void);
bool FastSYBT5query(void);
bool SendLXVoltsToLEDs(void);

uint8_t USART_1_read_with_timeout(void);



#endif /* SYBT5_ROUTINES_H_ */ SYBT5_ROUTINES_H_ */