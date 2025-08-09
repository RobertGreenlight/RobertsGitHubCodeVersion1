/*
 * AutoCalibrate.c
 *
 * Created: 7/29/2024 9:09:04 AM
 *  Author: Robert F
 */ 

#include <stdio.h>
#include <string.h>
#include <wdt.h>
#include <clock_config.h> 
#include <util/delay.h>

#include "import_preferences.h"
#include "bobs_functions.h"
#include "bluetooth_data_in.h"
#include "atmel_start_pins.h"

extern uint32_t Unfinished_Business;
extern uint32_t SleepTimer;

//--------------------------------------------Begin Auto Calibration Routine------------------------------------------------------
void BeginAutoCalibration(struct cal *ptr,char buffer[30]){
	int TempOrResCal = 0;
	char string[30];
	memset(string,0,30*sizeof(char));
	FT2_set_level(true);																	//Close relay and leave closed for the duration of the test
	BlueLED_set_level(false);
	ptr->StartandRunCalibration = 0;														// this is also used to suspend periodic battery information transmissions 
	ptr->Preferences = ptr->Preferences | 6;												// Set SYBT Type and Temperature to display Resistance	
	NewSetUnfinishedBusiness(Unfinished_AutoCalRunning,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	sscanf(buffer,"CL1X%d",&TempOrResCal);
	SleepTimer = 0;
	ptr->Sleep_2 = 800000;
	
	if(TempOrResCal == 1){
		ptr->StartandRunCalibration = 1;									// initialize resistor cal
		NewSetUnfinishedBusiness(Unfinished_AutoCalResStart,&Unfinished_Business); 	
	}
	else if(TempOrResCal == 2){														// initialize thermistor cal
		ptr->StartandRunCalibration = 2;
		NewSetUnfinishedBusiness(Unfinished_AutoCalTempStart,&Unfinished_Business);
	}
}
//--------------------------------------------Read Calibration Value for 6.6k and export to app-----------------------------------
void EnterCalValue66(struct cal *ptr){

	char string[30];
	memset(string,0,30*sizeof(char));
	SleepTimer = 0;
	if(ptr->StartandRunCalibration & 1)ptr->CalibrationValue66 = MeasureSYBTresistance(&resistance);								// read resistor ADC
	else if(ptr->StartandRunCalibration & 2)ptr->CalibrationValue66 = MeasureSYBTtemperature(true,(uint32_t*)21,0,&temperature);	// read temperature ADC. Entered arguments 21,true,0 are to satisfy the function when used as part of battery test routine
	ptr->StartandRunCalibration |= 4;						 // 
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	NewClearUnfinishedBusiness(Clear_AutoCalResistance,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_AutoCal66,&Unfinished_Business);
	sprintf(string,"ENTR66,%d,%dOHM\r\n",ptr->CalibrationValue66,ptr->StartandRunCalibration);
	SleepTimer = 0;
	TransmitADcalData(string,true);
	NewSetUnfinishedBusiness(Unfinished_AutoCalRetransmit,&Unfinished_Business);			//The output is buffered and retransmitted if the app doesn't respond.
//	USART0_sendString(string);	
//	_delay_ms(80);
//	USART0_sendString(string);	
}
//--------------------------------------------Import Calibration Value for 10k----------------------------------------------------
/*void EnterCalValue10(struct cal *ptr){
	
	char string[30];
	memset(string,0,30*sizeof(char));	
	if(ptr->StartandRunCalibration & 1)ptr->CalibrationValue10 = MeasureSYBTresistance(&resistance);								// read resistor ADC
	else if(ptr->StartandRunCalibration & 2)ptr->CalibrationValue10 = MeasureSYBTtemperature(true,(uint32_t*)21,0,&temperature);	// read temperature ADC
	ptr->StartandRunCalibration |= 8;						 // this is also used to suspend periodic battery information transmissions 
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	NewClearUnfinishedBusiness(Clear_AutoCalResistance,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_AutoCal10,&Unfinished_Business);
	sprintf(string,"ENTR10,%d,%dOHM\n",ptr->CalibrationValue10,ptr->StartandRunCalibration);
	USART0_sendString(string);	
	_delay_ms(80);
	USART0_sendString(string);
}
*/
//--------------------------------------------Import Calibration Value for 20k----------------------------------------------------
void EnterCalValue20(struct cal *ptr){

	char string[30];
	memset(string,0,30*sizeof(char));
	SleepTimer = 0;	
	if(ptr->StartandRunCalibration & 1)ptr->CalibrationValue20 = MeasureSYBTresistance(&resistance);								// read resistor ADC
	else if(ptr->StartandRunCalibration & 2)ptr->CalibrationValue20 = MeasureSYBTtemperature(true,(uint32_t*)21,0,&temperature);	// read temperature ADC
	ptr->StartandRunCalibration |= 8;	
	NewClearUnfinishedBusiness(Clear_AutoCalResistance,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_AutoCal20,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	sprintf(string,"ENTR20,%d,%dOHM\r\n",ptr->CalibrationValue20,ptr->StartandRunCalibration);
	SleepTimer = 0;
	TransmitADcalData(string,true);
	NewSetUnfinishedBusiness(Unfinished_AutoCalRetransmit,&Unfinished_Business);			//The output is buffered and retransmitted if the app doesn't respond.
//	USART0_sendString(string);
//	_delay_ms(80);
//	USART0_sendString(string);
}
//--------------------------------------------Import Calibration Value for 69k----------------------------------------------------
void EnterCalValue69(struct cal *ptr){

	char string[30];
	memset(string,0,30*sizeof(char));
	SleepTimer = 0;
	if(ptr->StartandRunCalibration & 1)ptr->CalibrationValue69 = MeasureSYBTresistance(&resistance);								// read resistor ADC
	else if(ptr->StartandRunCalibration & 2)ptr->CalibrationValue69 = MeasureSYBTtemperature(true,(uint32_t*)21,0,&temperature);	// read temperature ADC
	ptr->StartandRunCalibration |= 16;
	NewClearUnfinishedBusiness(Clear_AutoCalResistance,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_AutoCal69,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	sprintf(string,"ENTR69,%d,%dOHM\r\n",ptr->CalibrationValue69,ptr->StartandRunCalibration);
	SleepTimer = 0;
	TransmitADcalData(string,true);
	NewSetUnfinishedBusiness(Unfinished_AutoCalRetransmit,&Unfinished_Business);			//The output is buffered and retransmitted if the app doesn't respond.
//	USART0_sendString(string);
//	_delay_ms(80);
//	USART0_sendString(string);
}
//--------------------------------------------Run Auto Calibration Routine--------------------------------------------------------
void RunAutoCalibration(struct cal *ptr){
		
	float CalACorrection = 8;
	float CalBCorrection = 8;
	float CalACorrectionStart;
	float CalBCorrectionStart;
	register float ResultFor20;
	register float ResultFor66;
	register float ResultFor69;
	float CalA = 1500;
	float CalB = 800;
	register int CalAiterations = 0;
	register int CalBiterations = 0;
	char string[70];
	memset(string,0,70*sizeof(char));	
//	CalACorrectionStart = (((float)ptr->CalibrationValue20*CalA)/2000 + (float)ptr->CalibrationValue20)/(((float)ptr->CalibrationValue66*CalA)/660 + (float)ptr->CalibrationValue66)-1;
//	CalBCorrectionStart = (2000*(CalB - (float)ptr->CalibrationValue20)/(float)ptr->CalibrationValue20)/(660*(CalB - (float)ptr->CalibrationValue66)/(float)ptr->CalibrationValue66)-1;
	CalACorrectionStart = (((float)cal.CalibrationValue69*CalA)/6890 + (float)cal.CalibrationValue69)/(((float)cal.CalibrationValue66*CalA)/660 + (float)cal.CalibrationValue66)-1;
	CalBCorrectionStart = (6890*(CalB - (float)cal.CalibrationValue69)/(float)cal.CalibrationValue69)/(660*(CalB - (float)cal.CalibrationValue66)/(float)cal.CalibrationValue66)-1;

	while(!((CalACorrection <= 1.001)&&(CalACorrection >= 0.999))){
//	while(!((CalACorrection <= 1.00001)&&(CalACorrection >= 0.99999))){
		ResultFor69 = (((float)ptr->CalibrationValue69*CalA)/6890 + (float)ptr->CalibrationValue69);
//		ResultFor20 = (((float)ptr->CalibrationValue20*CalA)/2000 + (float)ptr->CalibrationValue20);
		ResultFor66 = (((float)ptr->CalibrationValue66*CalA)/660 + (float)ptr->CalibrationValue66);
		if(ResultFor66 == 0);   //skip to next increment
		else CalACorrection = ResultFor69/ResultFor66;
		CalA = CalA +0.2;
		CalAiterations++;
		wdt_reset();
		if(CalAiterations > 16000)break;
		
	}
	
	while(!((CalBCorrection <= 1.001)&&(CalBCorrection >= 0.999))){		
//	while(!((CalBCorrection <= 1.0001)&&(CalBCorrection >= 0.9999))){
		ResultFor69 = (6890*(CalB - (float)ptr->CalibrationValue69)/(float)ptr->CalibrationValue69);
//		ResultFor20 = (2000*(CalB - (float)ptr->CalibrationValue20)/(float)ptr->CalibrationValue20);
		ResultFor66 = (660*(CalB - (float)ptr->CalibrationValue66)/(float)ptr->CalibrationValue66);
		if(ResultFor66 == 0);   //skip to next increment
		CalBCorrection = ResultFor69/ResultFor66;
		CalB = CalB + 0.2;
		CalBiterations++;
		wdt_reset();
		if(CalBiterations > 14000)break;
	}
	
		
	if(ptr->StartandRunCalibration  == 29){		// must see entries for 6.6k, 20k, and 68.9k plus "resistance" button.
		ptr->ResistA_cal = (int)CalA;	
		ptr->ResistB_cal = (int)CalB;
	}
	if(ptr->StartandRunCalibration  == 30){	// must see entries for 6.6k, 20k, and 68.9k plus "temperature" button
		ptr->Therm_ResA_cal = (int)CalA;
		ptr->Therm_ResB_cal = (int)CalB;
	}	
	// ten items sent
	sprintf(string,"CAL%d,%d,%f,%f,%d,%d,%d,%d,%d,%dCal\r\n",(int)CalA,(int)CalB,CalACorrection,CalBCorrection,ptr->CalibrationValue66,ptr->CalibrationValue20,ptr->CalibrationValue69,CalAiterations,CalBiterations,ptr->StartandRunCalibration);
	//sprintf(string,"CAL,%f,%f,%f,%f,%f,%f,%d,%d,%d,%d,%dCal\n",CalA,CalB,CalACorrectionStart,CalBCorrectionStart,CalACorrection,CalBCorrection,calibration.StartandRunCalibration,CalAiterations,CalBiterations,calibration.CalibrationValue66,calibration.CalibrationValue20);
	USART0_sendString(string);
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	NewClearUnfinishedBusiness(Finished_AutoCAl,&Unfinished_Business);
	FT2_set_level(false);																	//Open relay
	BlueLED_set_level(true);	
	return;	 	
}
//--------------------------------------------Save Auto Calibration Routine--------------------------------------------------------
void SaveAutoCalibration(struct cal *ptr){			// this mimics received Bluetooth commands so that the same functions for saving data can be used
	char string[30];
	memset(string,0,30*sizeof(char));
	if(ptr->StartandRunCalibration  == 29){						// If resistance cal
		sprintf(string,"RA1%dY",ptr->ResistA_cal);	
		OhmmeterCalA(string);
		memset(string,0,30*sizeof(char));
		sprintf(string,"RA2%dY",ptr->ResistB_cal);
		OhmmeterCalB(string);
		NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	}
	if(ptr->StartandRunCalibration  == 30){						// If temperature cal
		memset(string,0,30*sizeof(char));
		sprintf(string,"TR1%dY",ptr->Therm_ResA_cal);
		ThermistorCalA(string);
		USART0_sendString(string);
		memset(string,0,30*sizeof(char));
		sprintf(string,"TR2%dY",ptr->Therm_ResB_cal);
		ThermistorCalB(string);
		NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);	
	}	
	ptr->StartandRunCalibration |= 64;
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
//	sprintf(string,"ENTR99,%d,%dOHM\n",ptr->CalibrationValue20,ptr->StartandRunCalibration);
	sprintf(string,"ENTR99,%d,%dOHM\r\n",ptr->CalibrationValue69,ptr->StartandRunCalibration);
	ptr->StartandRunCalibration = 0;
	USART0_sendString(string);
	_delay_ms(80);
	USART0_sendString(string);
	
	return;
}
//---------------------------------------this handles the LED display tasks-----------------------------------------------------------------------------
void SwitchToAutoCal(uint8_t *Rows){
	#ifndef ATTINY
		volts_noload.hundreds = Letter_a;					// display "ato"
		volts_noload.tens = Letter_t;
		volts_noload.ones = Letter_o;
	#endif
	#ifdef ATTINY
		volts_noload.hundreds = ASCII_a;					// display "ato"
		volts_noload.tens = ASCII_t;
		volts_noload.ones = ASCII_o;
	#endif
	
if(Unfinished_Business & Unfinished_AutoCal66){
	#ifndef ATTINY
		volts_noload.tens = LED_number_6;							//display "66"
		volts_noload.ones = LED_number_6;
		volts_noload.hundreds = (volts_noload.hundreds| 128);	
	#endif
	#ifdef ATTINY
		volts_noload.tens = ASCII_6;
		volts_noload.ones = ASCII_6;
	#endif
	
/*	temperature.hundreds = 0;
	temperature.tens = LED_number_6;	
	temperature.ones = LED_number_6;	
	resistance.ones = LED_charactermap((calibration.CalibrationValue66)%10);
	resistance.tens = LED_charactermap((calibration.CalibrationValue66/10)%10);
	resistance.hundreds = LED_charactermap((calibration.CalibrationValue66/100)%10);  //resistance.tens = (resistance.tens | 128);	
*/
}
else if(Unfinished_Business & Unfinished_AutoCal20){
	#ifndef ATTINY
		volts_noload.tens = LED_number_2;						//display "20"
		volts_noload.ones = LED_number_0;
		volts_noload.hundreds = (volts_noload.hundreds| 128);
	#endif
	#ifdef ATTINY
		volts_noload.tens = ASCII_2;
		volts_noload.ones = ASCII_0;
	#endif	
		
/*	temperature.hundreds = 0;	
	temperature.tens = LED_number_1;				
	temperature.ones = LED_number_0;
	resistance.ones = LED_charactermap((calibration.CalibrationValue10)%10);
	resistance.tens = LED_charactermap((calibration.CalibrationValue10/10)%10);
	resistance.hundreds = LED_charactermap((calibration.CalibrationValue10/100)%10);  //resistance.tens = (resistance.tens | 128);
*/
}
else if(Unfinished_Business & Unfinished_AutoCal69){
	#ifndef ATTINY
		volts_noload.tens = LED_number_6;						//display "69"
		volts_noload.ones = LED_number_9;
		volts_noload.hundreds = (volts_noload.hundreds| 128);
	#endif	
	#ifdef ATTINY
		volts_noload.tens = ASCII_6;
		volts_noload.ones = ASCII_9;
	#endif	
			
/*	temperature.hundreds = 0;	
	temperature.tens = LED_number_2;					
	temperature.ones = LED_number_0;
	resistance.ones = LED_charactermap((calibration.CalibrationValue20)%10);
	resistance.tens = LED_charactermap((calibration.CalibrationValue20/10)%10);
	resistance.hundreds = LED_charactermap((calibration.CalibrationValue20/100)%10);  //resistance.tens = (resistance.tens | 128);
*/
}	
//	*Rows |= 208;										// turns on green LEDs
//	*Rows ^= 16;										// turns off red LED
//	calibration.Sleep_1 = 800000;
//	calibration.Sleep_2 = 800000;
}
//-------------------------------------------------------------------------------------------------------------
