/*
 * bobs_functions.c
 *
 * Created: 7/29/2024 9:05:16 AM
 *  Author: Robert F
 */ 
 
//----------------------I moved my custom USART functions here to prevent them from being overwritten by updates from START
#include "bobs_functions.h"
#include "Seven_Seg_LED.h"
#include "import_preferences.h"
#include <math.h>
#include <float.h>
#include "atmel_start.h"
#include "atmel_start_pins.h"
#include <string.h>
#include <stdio.h>
#include "usart_basic.h"
#include <clock_config.h>
#include <util/delay.h>
#include <eeprom_functions.h>
#include "bluetooth_data_in.h" 
#include <wdt.h>
//extern uint16_t send_data_delay;
extern uint32_t Unfinished_Business;
extern bool SYBTisProvidingPower;

int i;
bool OverrideSafeties;

int TurnEverythingOn(void){
	#ifdef SmallChips
		FT2_set_level(true);			 // turn on 3.3v regulators for LEDs on small chip board  on 5th gen board it is light load IGBT
	#endif
	#ifdef LargeChips
		LargeChip_Display_set_level(true);
	#endif
		PWR_ON_set_level(true);  // connects 6v battery to voltage divider for measurement. also used to control switch on load test board
	return 1;
}

int ConvertToTemperature(int Temperature){
	 float logR2;
	 float T;
	 //long double c1 = 2.399163584e-03;
	 //long double c2 = 0.2684971175e-04;
	 //long double c3 = 8.748793546e-07;
	 logR2 = log(Temperature*10);
	 T = 1/(c1+ (c2*logR2) + c3*(logR2*logR2*logR2));
	 T = T- 273.15;
	 if(cal.Preferences & 1)T = ((T * 9.0)/ 5.0 + 32.0);
	 return T*10;
}

int AddDecimalPoint(int value){				// not used anywhere
	if(value > 1000){	
		temperature.ones = value%10;
		temperature.tens = (value/10)%10;
		temperature.hundreds = (value/100)%10;
	}
return value;
}
/*
int SeparateOne(int ValueToDisplay){	
return charactermap( ValueToDisplay%10);
}
int separateones = ValueToDisplay%10;

int separatetens = (adcVal/10)%10;
separatetens = charactermap(separatetens);
int separatehundreds = (adcVal/100)%10;

int separatethousands = (adcVal/1000);
*/
bool USART0_sendString(char *str)							//send to Bluetooth module
{
	for (size_t i = 0; i < strlen(str); i++) {
		USART0_sendChar(str[i]);
	}
	return true;
}
void USART0_sendChar(char c)								//send to Bluetooth module
{
	while (!(USART0.STATUS & USART_DREIF_bm)) {
	}
	USART0.TXDATAL = c;
}
//----------------------------------------------------------------------------------------------------
bool USART3_sendString(char *str)							//send to ATTINY
{
	BlueLED_set_level(true);
	for (size_t i = 0; i < strlen(str); i++) {
		USART_3_write(str[i]);
	}
	return true;
}

bool USART3_sendHEX(char *str,uint8_t length)				//send to ATTINY
{
	BlueLED_set_level(true);
	for (size_t i = 0; i < length; i++) {
		USART_3_write(str[i]);
	}
	return true;
}
//----------------------------------------------------------------------------------------------------
void USART1_sendChar(char c)								// only used for testing
{
	while (!(USART1.STATUS & USART_DREIF_bm)) {
		;
	}
	USART1.TXDATAL = c;
}
void USART1_sendString(char *str)
{
	for (size_t i = 0; i < strlen(str); i++) {
		USART1_sendChar(str[i]);
	}

}
//-----------------------------------------------------------------------------------------------------
uint8_t USART_1_read_with_timeout()
{
	uint32_t timeout = 0;
	//TCA0.SINGLE.CNT = 0x0;
	while (!(USART1.STATUS & USART_RXCIF_bm)){//|| timeout > 2400){			//while waiting for response or timeout  !status breaks while loop. 																//inside while loop, count down or up.
		timeout ++;
		if(timeout > 3200)return 255;					// 3000 looks like a good timeout
	}												// if time elapses first, return error.
	return USART1.RXDATAL;
}
//---------------------------------------------------------------------------------------
void USART_1_disable_rx()
{
	USART1.CTRLB  &= ~USART_RXEN_bm;
}
//-------------------------------------------------old identify battery type here-------------------------------------------------------------------------------------
/*
bool oldIdentify_Battery_Type(uint32_t Resistance)   // this works well. not currently in use
{
	if (Resistance > 64 && Resistance < 68) Resistance = SYBT_Type_V66;
	else if (Resistance > 98 && Resistance < 102) Resistance = SYBT_Type_C10;
	else if (Resistance > 198 && Resistance < 202) Resistance = SYBT_Type_B20;
	else if (Resistance > 675 && Resistance < 699) Resistance = SYBT_Type_C69;
	else if (Resistance > 20000) Resistance = SYBT_Type_Error;
	array_SYBT[SYBTcount].Resistance = Resistance;
	
 switch (Resistance){
	 case SYBT_Type_V66:
	 resistance.ones = 125;
	 resistance.tens = 125;
	 resistance.hundreds = Letter_v_dp;
	 Rows = Rows | 64;
	 return true;
	 case SYBT_Type_C10:
	 resistance.ones =  63;
	 resistance.tens = 6;
	 resistance.hundreds = Letter_c_dp;
	 Rows = Rows | 64;
	 return true;
	 case SYBT_Type_B20:
	 resistance.ones =  63;
	 resistance.tens = 91;
	 resistance.hundreds = Letter_b_dp;
	 Rows = Rows | 64;
	 return true;
	 case SYBT_Type_C69:
	 resistance.ones = LED_number_9;
	 resistance.tens = LED_number_6;
	 resistance.hundreds = Letter_c;
	 Rows = Rows | 64;
	 return true;
	 case SYBT_Type_Error:				//max output from AD
	 resistance.ones =  DASH;
	 resistance.tens = DASH;
	 resistance.hundreds = DASH;
	 Rows = Rows | 64;
	 Rows = Rows ^ 64;
	 return true;
	 default:
	 return false;
	}
}
*/
//--------------------------------------------------------------------------------------------------------------------------------------
 bool Identify_Battery_Type(uint32_t Resistance,uint8_t *Rows)   // this works well.  it returns TRUE if any values match a battery type
 {
	uint8_t tempRow = *Rows;
  
	tempRow = tempRow | RedGreenResistanceLED;  
	tempRow = tempRow ^ RedGreenResistanceLED;
	if((cal.Preferences & 2) && (Resistance < 999 )){	// user requested resistance value and the value is valid
		 *Rows = tempRow;
		  return false;					// do nothing. Just return the raw resistznce value.
		}
	 // these use a range of resistance values to compensate for inaccuracies in the measurement 
	 // Tolerance values are multiplied by 2 for C69 and E85. This was done to give higher resistances more room for error.

	 if ((Resistance > SYBT_Type_V66 - cal.SYBT_Idnt_Tolrnc) && Resistance < (SYBT_Type_V66 + cal.SYBT_Idnt_Tolrnc)) Resistance = SYBT_Type_V66;
	 else if ((Resistance > SYBT_Type_C10 - cal.SYBT_Idnt_Tolrnc) && Resistance < (SYBT_Type_C10 + cal.SYBT_Idnt_Tolrnc)) Resistance = SYBT_Type_C10;
	 else if ((Resistance > SYBT_Type_B20 - cal.SYBT_Idnt_Tolrnc) && Resistance < (SYBT_Type_B20 + cal.SYBT_Idnt_Tolrnc)) Resistance = SYBT_Type_B20;
	 else if ((Resistance > SYBT_Type_E3S - cal.SYBT_Idnt_Tolrnc) && Resistance < (SYBT_Type_E3S + cal.SYBT_Idnt_Tolrnc)) Resistance = SYBT_Type_E3S;
	 else if ((Resistance > SYBT_Type_C69 - (cal.SYBT_Idnt_Tolrnc*2)) && Resistance < (SYBT_Type_C69 + (cal.SYBT_Idnt_Tolrnc*2))) Resistance = SYBT_Type_C69;
	 else if ((Resistance > SYBT_Type_E85 - (cal.SYBT_Idnt_Tolrnc*2)) && Resistance < (SYBT_Type_E85 + (cal.SYBT_Idnt_Tolrnc*2))) Resistance = SYBT_Type_E85;
	 else if (Resistance > (cal.FlavrVal1 - cal.SYBT_Idnt_Tolrnc) && Resistance < (cal.FlavrVal1 + cal.SYBT_Idnt_Tolrnc)) Resistance = cal.FlavrVal1;
	 else if (Resistance > (cal.FlavrVal2 - cal.SYBT_Idnt_Tolrnc) && Resistance < (cal.FlavrVal2 + cal.SYBT_Idnt_Tolrnc)) Resistance = cal.FlavrVal2;
	 else if (Resistance > (cal.FlavrVal3 - cal.SYBT_Idnt_Tolrnc) && Resistance < (cal.FlavrVal3 + cal.SYBT_Idnt_Tolrnc)) Resistance = cal.FlavrVal3;
	 else if (Resistance > (cal.FlavrVal4 - cal.SYBT_Idnt_Tolrnc) && Resistance < (cal.FlavrVal4 + cal.SYBT_Idnt_Tolrnc)) Resistance = cal.FlavrVal4;
	 else if (Resistance <= 1) Resistance = SYBT_Type_Short;		// open circuit
	 else if (Resistance == 999) Resistance = SYBT_Type_Error;		// open circuit
	 array_SYBT[1].Resistance = Resistance;
	 
	 switch (Resistance){
		 case SYBT_Type_V66:						// assign "v.66" to display
			#ifndef ATTINY
				 resistance.ones = LED_number_6;
				 resistance.tens = LED_number_6;
				 resistance.hundreds = Letter_v_dp;
				 *Rows = tempRow | GreenResistanceLED;
			 #endif
			 #ifdef ATTINY
			 	 resistance.ones = ASCII_6;
			 	 resistance.tens = ASCII_6;
			 	 resistance.hundreds = ASCII_v_dp;
			 	//*Rows = tempRow | GreenResistanceLED;
			 #endif
		 return true;
		 case SYBT_Type_C10:						// assign "c.10" to display
		 	#ifndef ATTINY			
				 resistance.ones = LED_number_0;
				 resistance.tens = LED_number_1;
				 resistance.hundreds = Letter_c_dp;
				 *Rows = tempRow | GreenResistanceLED;
			#endif
			#ifdef ATTINY
			 	 resistance.ones = ASCII_0;
			 	 resistance.tens = ASCII_1;
			 	 resistance.hundreds = ASCII_c_dp;
			 	//*Rows = tempRow | GreenResistanceLED;
			#endif 	 
		 return true;
		 case SYBT_Type_B20:						// assign "b.20" to display
			#ifndef ATTINY
				 resistance.ones = LED_number_0;
				 resistance.tens = LED_number_2;
				 resistance.hundreds = Letter_b_dp;
				 *Rows = tempRow | GreenResistanceLED;		 
			#endif 		 
			#ifdef ATTINY
			 	 resistance.ones = ASCII_0;
			 	 resistance.tens = ASCII_2;
			 	 resistance.hundreds = ASCII_b_dp;
			 	//*Rows = tempRow | GreenResistanceLED;
			#endif 		 		 
		 return true;
		 case SYBT_Type_E3S:						// assign "E.3S" to display
			#ifndef ATTINY
				 resistance.ones = LED_number_5;	// "5" and "E" look the same on LED display
				 resistance.tens = LED_number_3;
				 resistance.hundreds = Letter_E_dp;
				 *Rows = tempRow | GreenResistanceLED;		 
			#endif 		 
			#ifdef ATTINY
			 	 resistance.ones = ASCII_5;			// "5" and "E" look the same on LED display
			 	 resistance.tens = ASCII_3;
			 	 resistance.hundreds = ASCII_E_dp;
			 	//*Rows = tempRow | GreenResistanceLED;
			#endif 		 		 
		 return true;
		 case SYBT_Type_C69:						// assign "c.69" to display
			#ifndef ATTINY		 
				 resistance.ones = LED_number_9;
				 resistance.tens = LED_number_6;
				 resistance.hundreds = Letter_c_dp;
				 *Rows = tempRow | GreenResistanceLED;
			#endif 		 
			#ifdef ATTINY
			 	 resistance.ones = ASCII_9;
			 	 resistance.tens = ASCII_6;
			 	 resistance.hundreds = ASCII_c_dp;
			 	//*Rows = tempRow | GreenResistanceLED;
			#endif 		 			 			 
		 return true;
		 case SYBT_Type_E85:						// assign "E.85" to display
			#ifndef ATTINY			 
				 resistance.ones = LED_number_5;
				 resistance.tens = LED_number_8;
				 resistance.hundreds = Letter_E_dp;
				 *Rows = tempRow | GreenResistanceLED;
			#endif 		 
			#ifdef ATTINY
			 	 resistance.ones = ASCII_5;
			 	 resistance.tens = ASCII_8;
			 	 resistance.hundreds = ASCII_E_dp;
			 	//*Rows = tempRow | GreenResistanceLED;
			#endif 						 
		 return true;		 
		 case SYBT_Type_Error:						// assign "---" to display
			#ifdef ATTINY		 
				 resistance.ones = DASH;
				 resistance.tens = DASH;
				 resistance.hundreds = DASH;
				 *Rows = tempRow | RedResistanceLED;
			#endif	 
			#ifdef ATTINY
			 	 resistance.ones = ASCII_DASH;
			 	 resistance.tens = ASCII_DASH;
			 	 resistance.hundreds = ASCII_DASH;
			 	//*Rows = tempRow | GreenResistanceLED;
			#endif 			 
		 return true;
		 case SYBT_Type_Short:						// do not change display.
		 	*Rows = tempRow | RedResistanceLED;
		 return false;

		 default:
		 *Rows = tempRow;
		 break;
	 }
	 if(Resistance == cal.FlavrVal1){						// assign custom name 1 to display
		#ifndef ATTINY		 
			resistance.ones = ASCII_charactermap(cal.FlavrName1[2]);
			resistance.tens = ASCII_charactermap(cal.FlavrName1[1]);
			resistance.hundreds = ASCII_charactermap(cal.FlavrName1[0]);
		*Rows = tempRow | GreenResistanceLED;
		#endif	 
		#ifdef ATTINY
			resistance.ones = cal.FlavrName1[2];
			resistance.tens = cal.FlavrName1[1];
			resistance.hundreds = cal.FlavrName1[0];
			//*Rows = tempRow | GreenResistanceLED;
		#endif 	
		return true;
	 }
	 else if(Resistance == cal.FlavrVal2){						// assign custom name 2 to display
		#ifndef ATTINY				 
			resistance.ones = ASCII_charactermap(cal.FlavrName2[2]);
			resistance.tens = ASCII_charactermap(cal.FlavrName2[1]);
			resistance.hundreds = ASCII_charactermap(cal.FlavrName2[0]);
			*Rows = tempRow | GreenResistanceLED;
		#endif
		#ifdef ATTINY
			resistance.ones = cal.FlavrName2[2];
			resistance.tens = cal.FlavrName2[1];
			resistance.hundreds = cal.FlavrName2[0];
			//*Rows = tempRow | GreenResistanceLED;
		#endif		 	 
		 return true;
	 }
	 else if(Resistance == cal.FlavrVal3){						// assign custom name 3 to display
		#ifndef ATTINY				 
			resistance.ones = ASCII_charactermap(cal.FlavrName3[2]);
			resistance.tens = ASCII_charactermap(cal.FlavrName3[1]);
			resistance.hundreds = ASCII_charactermap(cal.FlavrName3[0]);
			*Rows = tempRow | GreenResistanceLED;
		#endif
		#ifdef ATTINY
			resistance.ones = cal.FlavrName3[2];
			resistance.tens = cal.FlavrName3[1];
			resistance.hundreds = cal.FlavrName3[0];
			//*Rows = tempRow | GreenResistanceLED;
		#endif			 
		return true;
	 }
	 else if(Resistance == cal.FlavrVal4){						// assign custom name 4 to display
		#ifndef ATTINY
			resistance.ones = ASCII_charactermap(cal.FlavrName4[2]);
			resistance.tens = ASCII_charactermap(cal.FlavrName4[1]);
			resistance.hundreds = ASCII_charactermap(cal.FlavrName4[0]);
			*Rows = tempRow | GreenResistanceLED;
		#endif
		#ifdef ATTINY
			resistance.ones = cal.FlavrName4[2];
			resistance.tens = cal.FlavrName4[1];
			resistance.hundreds = cal.FlavrName4[0];
			//*Rows = tempRow | GreenResistanceLED;
		#endif			 
		return true;
	 }
return false; 
}
 
 
 /*
 Measure battery voltage
 Measure load test circuit
 Load test volts should be about 65% of battery voltage
 Close relay
 Delay to give time for relay to close
 Load test volts should be nearly identical to battery voltage
 Turn on IGBT for 1/3 second
 Load test volts should drop to voltage across closed IGBT: less than 1 volt
 Measure battery voltage again
 Turn off IGBT
 Load test volts should be nearly identical to battery voltage
 Turn off relay
 Load test volts should be about 65% of battery voltage
 Measure battery voltage again.
 */
 
 
 //-------------------------------------------------------Cal factors corrected for SYBT 2--------------------------- 
 bool Test_Battery(int batterystatus, uint32_t *DisplayVoltageDropTimer,int *Test_Status,int *SYBT_adc_cal,int *LOAD_adc_cal)
 {	 
	 int Sybt_Loaded_volts1 = 0;
	 int Sybt_Loaded_volts2 = 0;
	 
#ifdef FifthGen
	 int Sybt_Light_Load_volts1 = 0;
	 int Sybt_Light_Load_volts2 = 0;
	 int IGBT2_Closed_volts = 0;
#endif	 

	 int Sybt_Unloaded_volts1 = 0;
	 int Sybt_Unloaded_volts2 = 0;
	 int Relay_Open_volts = 0;
	 int Relay_Closed_volts = 0;
	 int IGBT_Closed_volts = 0;
	 
	 uint8_t TestStatus = 0;	 
	 char BatteryTestString [50];
	 memset(BatteryTestString,0,50*sizeof(char));
	 bool IGBT_ok = false;
	 bool RelayOpen_ok = false;
	 bool RelayClose_ok = false;
	// THREE_DIGIT_SPI(DP,DP,DP,2,false);										// blank display during test
	 temperature.ones = 0;													// clear old data
	 temperature.tens = 0;													// clear old data
	 temperature.hundreds = 0;
	 int LowerRange;
	 int UpperRange;	 													// clear old data
	 *Test_Status = 0;
	 *DisplayVoltageDropTimer = 0;
	 
	 wdt_reset();
	 ReverseMarchingAnts_set_level(false);
	 MarchingAnts_set_level(true);
	 PWR_ON_set_level(true);  // turn on 6v battery
	 SYBTisProvidingPower = false;
	 
	 if(SYBT.HeatSinkTemp > 1500){
		USART0_sendString("Heatsink is HOT,\r\n");	
		*Test_Status = 255;
		return false;
	 }
	 else{
		 Load_IGBT_set_level(true);  //true opens IGBT
		 Load_Relay_set_level(false);  // false opens relay
		 ADC_0_get_conversion(ADC_SYBT_Volts);	
		 ADC_0_get_conversion(ADC_SYBT_Volts);	
		 ADC_0_get_conversion(ADC_SYBT_Volts);	
		 Sybt_Unloaded_volts1 = (((float)cal.SYBT_ad_cal/1000) * (ADC_0_get_conversion(ADC_SYBT_Volts) >> 2));//10;			//get open circuit voltage.  should be about 1.7 v at divider.
	 
		 if(Sybt_Unloaded_volts1 < 50){ USART0_sendString("Battery voltage too low,\r\n"); return false;}						// abort test if SYBT voltage is too low.
		 ADC_0_get_conversion(ADC_Load_Test_Volts);	 
		 Relay_Open_volts = (((float)*LOAD_adc_cal/1000) * (ADC_0_get_conversion(ADC_Load_Test_Volts) >> 2));//10;			// should be about 60 to 75 volts actual, or 65% of battery voltage.
		 // if relay volts is between 60 and 80% of battery  if (66*1.7)"112" > 108 and (71*1.26)"90" is < 108 
		 //if(((((Relay_Open_volts*142) - 5000) < Sybt_Unloaded_volts1 * 100) && (((Relay_Open_volts*142) + 5000) > Sybt_Unloaded_volts1 * 100 )) || OverrideSafeties){	//it is OK to continue with relay test
		 LowerRange =  floor((((float)Relay_Open_volts*(float)cal.LoadTestTolerance_A)-(float)cal.LoadTestTolerance_B*100)/100);
		 UpperRange =  floor((((float)Relay_Open_volts*(float)cal.LoadTestTolerance_A)+(float)cal.LoadTestTolerance_B*100)/100);
		 if(((LowerRange < Sybt_Unloaded_volts1) && (UpperRange > Sybt_Unloaded_volts1)) || OverrideSafeties){	
	//	 if(((LowerRange < (float)Sybt_Unloaded_volts1) && (UpperRange > (float)Sybt_Unloaded_volts1)) || OverrideSafeties){	
	//	 if((((((float)Relay_Open_volts*(float)cal.LoadTestTolerance_A*10)-(float)cal.LoadTestTolerance_B*100)<(float)Sybt_Unloaded_volts1*100)
	//	 &&
	//	 ((((float)Relay_Open_volts*(float)cal.LoadTestTolerance_A*10)+(float)cal.LoadTestTolerance_B*100)>(float)Sybt_Unloaded_volts1*100 )) || OverrideSafeties){	//it is OK to continue with relay test
		
			RelayOpen_ok = true;						
			 _delay_ms(100);
			 Load_Relay_set_level(true);		// true closes relay
			 _delay_ms(200);					//get relay closed voltage.  should be about 2.6 v.  Needs time for relay to close and ADC to acquire.
			ADC_0_get_conversion(ADC_Load_Test_Volts);  
			Relay_Closed_volts = (((float)*LOAD_adc_cal/1000) * (ADC_0_get_conversion(ADC_Load_Test_Volts) >> 2));//   /10;
			// should be about 108 volts actual, or 99% of battery voltage.
			// if relay volts is between 105 and 90% of battery  if (108*1.1)"120" < 108 or (71*1.2)"90" is > 108
			if((((Relay_Closed_volts - 100) < Sybt_Unloaded_volts1 ) && ((Relay_Closed_volts + 100) > Sybt_Unloaded_volts1 )) || OverrideSafeties){				// it is ok to continue with load test.	
	//		if(true){			
				//USART0_sendString("Relay Closed OK\r\n");
				RelayClose_ok = true;
				
		wdt_reset();
					
			//#ifdef FifthGen 
				//FT2_set_level(false);  //false closes IGBT  		// I commented these out as a reminder that I'm currently not using the light load design at all
			//#endif					
				Load_IGBT_set_level(false);  //false closes IGBT
				_delay_ms(3);
				IGBT_Closed_volts = (((float)*LOAD_adc_cal/1000) * (ADC_0_get_conversion(ADC_Load_Test_Volts) >> 2));//10;
				// if relay volts is below 2v 
				if(((IGBT_Closed_volts) < 100) || OverrideSafeties ){																							// it is ok to continue with load test.	
				
					IGBT_ok = true;		
					Sybt_Loaded_volts1 = (((float)*SYBT_adc_cal/1000) * (ADC_0_get_conversion(ADC_SYBT_Volts) >> 2));//10; //get SYBT voltage under load. 
			//		 _delay_ms(200);
					 Load_IGBT_set_level(true);  //true opens IGBT
					 _delay_ms(1000);
	//	wdt_reset(); 
	//				 Load_IGBT_set_level(false);  //false closes IGBT
	//				 _delay_ms(200);
	//				 Load_IGBT_set_level(true);  //true opens IGBT
	//				 _delay_ms(1000);
		wdt_reset(); 
					 Load_IGBT_set_level(false);  //false closes IGBT
					 _delay_ms(200);
					 Sybt_Loaded_volts2 = (((float)*SYBT_adc_cal/1000) * (ADC_0_get_conversion(ADC_SYBT_Volts) >> 2));//10; //get SYBT voltage under load. 
						// IGBT_Closed_volts = (1.39 * (ADC_0_get_conversion(ADC_Load_Test_Volts) >> 2));//10; 
					 Load_IGBT_set_level(true);  //true opens IGBT
					 _delay_ms(500);  
				
		wdt_reset();
				
					Relay_Closed_volts = (((float)*LOAD_adc_cal/1000) * (ADC_0_get_conversion(ADC_Load_Test_Volts) >> 2));//10;
					Sybt_Unloaded_volts2 = (((float)*SYBT_adc_cal/1000) * (ADC_0_get_conversion(ADC_SYBT_Volts) >> 2));//10;
					Load_Relay_set_level(false);  //false opens relay//verify IGBT opened.  Should be close to battery voltage.
					//_delay_ms(100);
	 				Beeper_set_level(true);
	 				_delay_ms(50);
	 				Beeper_set_level(false);
					Relay_Open_volts = (((float)*LOAD_adc_cal/1000) * (ADC_0_get_conversion(ADC_Load_Test_Volts) >> 2));//10;
	
					array_SYBT[SYBTcount].Volts_noload1 = Sybt_Unloaded_volts1;  //Copy loaded and unloaded volts into array.
					array_SYBT[SYBTcount].Volts_noload2 = Sybt_Unloaded_volts2;  //Copy loaded and unloaded volts into array.
					array_SYBT[SYBTcount].Volts_wload1 = Sybt_Loaded_volts1;
					array_SYBT[SYBTcount].Volts_wload2 = Sybt_Loaded_volts2;
					SYBT.Voltage_Drop = abs(Sybt_Unloaded_volts1 - Sybt_Loaded_volts2);
					array_SYBT[SYBTcount].Voltage_Drop = SYBT.Voltage_Drop;
					//array_SYBT[SYBTcount].Temperature = Temperature;
					//array_SYBT[SYBTcount].Resistance = Resistance;												
			   }										// end of IGBT closed OK
			}											// end of Relay Closed OK 			
		 }
	} 												// End of Relay Open OK 
//---------------------------sends to primary serial connection---------------------------	
	if(IGBT_ok)TestStatus = 1;										//if(IGBT_ok)USART0_sendString(" IGBT OK ");
	else TestStatus = 8;											//else USART0_sendString(" IGBT Fail  ");
	if(RelayOpen_ok) TestStatus = (TestStatus | 2);					//if(RelayOpen_ok)USART0_sendString(" Relay Open OK ");
	else  TestStatus = (TestStatus | 16);							//else USART0_sendString(" Relay Open Fail");
	if(RelayClose_ok) TestStatus = (TestStatus | 4);				//if(RelayClose_ok)USART0_sendString(" Relay Closed OK ");
	else TestStatus = (TestStatus | 32);							//else USART0_sendString(" Relay Closed Fail ");
#ifdef FifthGen
	sprintf(BatteryTestString,"TestStat,%i,Result,%d,%d,%d,%d,%d,%d,%d,%d,End\r\n",TestStatus,Sybt_Unloaded_volts1,LowerRange,UpperRange,Sybt_Loaded_volts2,Relay_Open_volts,Relay_Closed_volts,IGBT_Closed_volts,SYBT.Voltage_Drop);
#endif
#ifndef FifthGen
	sprintf(BatteryTestString,"TestStat,%i,Result,%d,%d,%d,%d,%d,%d,%d,%d,End\r\n",TestStatus,Sybt_Unloaded_volts1,LowerRange,UpperRange,Sybt_Loaded_volts2,Relay_Open_volts,Relay_Closed_volts,IGBT_Closed_volts,SYBT.Voltage_Drop);
#endif
	
//	sprintf(BatteryTestString,"TestStat,%d,Result,%d,%d,%d,%d,%d,%d,%d,End\r\n",TestStatus,Sybt_Unloaded_volts1,Sybt_Unloaded_volts2,Sybt_Loaded_volts1,Sybt_Loaded_volts2,Relay_Open_volts,Relay_Closed_volts,IGBT_Closed_volts);
	USART0_sendString(BatteryTestString);
	*Test_Status = TestStatus;	
	MarchingAnts_set_level(false);
	if(TestStatus == 7)return true;	
	else return false;  
 }						// end of load test function
//------------------------------------------------------------------------------------------------------------------------------------- 
/*if (!Switch_1_get_level()){
		char string [20];
		uint8_t number_battery = 1;
		//THREE_DIGIT_SPI(DASH,DASH,DASH,15);   Not used wit ATTINY
		void USART_0_disable_RX();
			while (number_battery <= SYBTcount){
		
					LED0_set_level(false);		
					USART1_sendString("SYBT#  ");	
					sprintf(string,"%d \t", number_battery);
					USART1_sendString(string);
					USART1_sendString("Volts ");	
					sprintf(string,"%d \t", array_SYBT[number_battery].Volts);
					USART1_sendString(string);
					USART1_sendString("Temperature ");	
					sprintf(string,"%d \t", array_SYBT[number_battery].Temperature);
					USART1_sendString(string);
					
					switch (array_SYBT[number_battery].Resistance){
					case 66:
					USART1_sendString("V-66\r");	
					break;
					case 100:
					USART1_sendString("C-10\r\n");	
					break;
					case 200:
					USART1_sendString("B-20\r\n");
					break;	
					default:
					USART1_sendString("Resistance ");		
					sprintf(string,"%d \r\n", array_SYBT[number_battery].Resistance);
					USART1_sendString(string);
					}
					number_battery++;
				}
				USART_0_enable_rx();
				//SYBTcount = 0;
		}
	else {LED0_set_level(true);}
*/	/*	
	bool Measure_Battery(int SYBTcont,uint8_t Volts, uint8_t Temperature, uint8_t Resistance, bool Switch_2_Pressed){
		LED4_set_level(false);
		Load_IGBT_set_level(true);
		SYBTcount ++;
		Switch_2_Pressed = true;
		array_SYBT[SYBTcount].Volts = Volts;
		array_SYBT[SYBTcount].Temperature = Temperature;
		array_SYBT[SYBTcount].Resistance = Resistance;
		//Volts = SYBTcount;
		 volts.ones = LED_charactermap(SYBTcount%10);
		 volts.tens = LED_charactermap((SYBTcount/10)%10);
		 volts.hundreds = LED_charactermap((SYBTcount/100)%10);
		 	
	if (Switch_2_get_level() && Switch_2_Pressed){ 
		LED4_set_level(true);
		Load_IGBT_set_level(false);
		Switch_2_Pressed = false;
		}	
		return 	Switch_2_Pressed;
	}
	*/
//--------------------------------------------------------------------------------------------
/*
first sends "Num Batt",SYBT count, number_battery,battery volts,"Temp",
then sends either valid temperature or "Open"
then either valid SYBT name or "Open"
then SYBT resistance value, "9 Volt Batt",9 volt battery value, calibration preferences

Problem:  sending SYBT type or out of range appears to add one additional CSV item.
Problem: sending rest of data is inside the SYBT name if statement

*/
//---------------------------------------------------------------------------------------------------------------------------------------
bool Gather_Battery_Data(int SixVoltBattery, int FiveVoltBus,int ResistorZeroDrop, bool SYBTgood,uint32_t SleepTimer,uint32_t BluetoothSleepTimer){					//sends 14 item CSV list

	char BatteryDataString [100];
	char TempString[30];										//needs improvement.  SYBTcount and number_battery serve no purpose here
	//int number_battery = 1;
	int BatteryIdentified = 0;
	//THREE_DIGIT_SPI(DASH,DASH,DASH,15,false);				// NOT USED with ATTINY blanks display while transmitting data
	memset(BatteryDataString,0,100*sizeof(char));										// first 8 CSV data sets
	if(TypeOfBatteryConnected  == 3){
		sprintf(TempString,"E3SConnected\n\r");
	}
	else{
		sprintf(TempString,"PXConnected\n\r");
	}		
	sprintf(BatteryDataString,"%s,Batt%d,%u,%u,%u,%lu,%d,%lu,",TempString,(int)SYBT.InternalTemperature,(uint8_t)(cal.Sleep_1/10000),(uint8_t)(cal.Sleep_2/10000),(uint8_t)(cal.Sleep_3/10000),SleepTimer,array_SYBT[1].Volts_noload1,BluetoothSleepTimer);
	
	if(array_SYBT[1].Temperature < 3000){										//if valid temperature send temperature. else send "open"
		sprintf(TempString,"%d,",array_SYBT[1].Temperature);
		if(array_SYBT[1].Temperature == 2999){strcat(BatteryDataString,"Demo,");}		// if 2999, send Demo Mode message
		else strcat(BatteryDataString,TempString);              
	}
	else strcat(BatteryDataString,"Open,");      // data point 9
	
	if(!(cal.Preferences & 2) || (array_SYBT[1].Resistance == 999)){	//user requested SYBT type or the measurement is out of range
		switch (array_SYBT[1].Resistance){
			case SYBT_Type_V66:									//10
				strcat(BatteryDataString,"V-66");
				BatteryIdentified = 8;
				break;
			case SYBT_Type_C10:									//18			
				strcat(BatteryDataString,"C-10");
				BatteryIdentified = 8;
				break;
			case SYBT_Type_B20:									//18		
				strcat(BatteryDataString,"B-20");
				BatteryIdentified = 8;
				break;
			case SYBT_Type_E3S:									//18
				strcat(BatteryDataString,"E-3S");
				BatteryIdentified = 8;
				break;	
			case SYBT_Type_C69:
				strcat(BatteryDataString,"C-69");				//18
				BatteryIdentified = 8;
				break;
			case SYBT_Type_E85:
				strcat(BatteryDataString,"E-85");				//18
				BatteryIdentified = 8;
				break;
			case SYBT_DemoR:									//18	
				strcat(BatteryDataString,"Mode");
				BatteryIdentified = 4;
				break;
			case SYBT_Type_Short:								//10
				strcat(BatteryDataString,"Zero");
				BatteryIdentified = 4;
				break;	
			case SYBT_Type_Error:								//10
				strcat(BatteryDataString,"Open");
				BatteryIdentified = 4;
				break;
			default:
				BatteryIdentified = 0;
				break;
		}																	//end of switch statement		
		if(cal.FlavrVal1 == array_SYBT[1].Resistance){
			strcat(BatteryDataString,cal.FlavrName1);
			BatteryIdentified = 8;
		}
		else if(cal.FlavrVal2 == array_SYBT[1].Resistance){
			strcat(BatteryDataString,cal.FlavrName2);
			BatteryIdentified = 8;
		}
		else if(cal.FlavrVal3 == array_SYBT[1].Resistance){
			strcat(BatteryDataString,cal.FlavrName3);
			BatteryIdentified = 8;
		}
		else if(cal.FlavrVal4 == array_SYBT[1].Resistance){
			strcat(BatteryDataString,cal.FlavrName4);
			BatteryIdentified = 8;
		}																// end of else if group
	}																		//End of user requested SYBT type or the measurement is out of range
	if(!(BatteryIdentified == 8) && !(BatteryIdentified == 4)){				//if no battery type was identified, then send the resistance value.
		sprintf(TempString,"%d",array_SYBT[1].Resistance);
		strcat(BatteryDataString,TempString);
	}
	if(SYBTgood)BatteryIdentified |= 16;
	#ifdef HeatSinkThermistorInstalled
//	sprintf(TempString,",%d,%d,%d,%d,%dEnd\r\n",FiveVoltBus,BatteryIdentified,SixVoltBattery,cal.Preferences,(int)SYBT.HeatSinkTemp);   //11 to 14 data points
	sprintf(TempString,",%d,%d,%d,%d,%d,%d,%d,%d,%dEnd\r\n",FiveVoltBus,BatteryIdentified,SixVoltBattery,cal.Preferences,(int)SYBT.HeatSinkTemp,ResistorZeroDrop,SYBT.LXvoltage,twentyfourvoltsupply,TypeOfBatteryConnected);   //11 to 15 data points
	#endif
	#ifndef HeatSinkThermistorInstalled
	sprintf(TempString,",%d,%d,%d,%d,%d,%d,%dEnd\r\n",FiveVoltBus,BatteryIdentified,SixVoltBattery,cal.Preferences,SYBT.LXvoltage,twentyfourvoltsupply,TypeOfBatteryConnected);   //11 to 14 data points
	#endif		
	strcat(BatteryDataString,TempString);
	TransmitBatteryData(BatteryDataString,true);		//20
	return true;
}																	// end of transmit battery data
//-------------------------currently not used---Aug 19 2020--------------------------------------------------------------------------------------
/*
int Store_Battery_Data(int SYBTcount, int Volts,int Temperature,uint32_t Resistance){
	
	//Load_IGBT_set_level(true);
	array_SYBT[SYBTcount].Volts_noload1 = Volts;
	array_SYBT[SYBTcount].Temperature = Temperature;
	array_SYBT[SYBTcount].Resistance = Resistance;
	//Volts = SYBTcount;
	volts_noload.ones = LED_charactermap(SYBTcount%10);		//don t worry about ATTINY here because I'm not using this function anyway
	volts_noload.tens = LED_charactermap((SYBTcount/10)%10);
	volts_noload.hundreds = LED_charactermap((SYBTcount/100)%10);
	return -2000;
}
*/
//-----------------------------------------------------------------------------------------------------------------
int Check_SYBT_Status(bool BatteryPresent,int batterystatus,int SYBTcount,int Volts,int Temperature,uint32_t Resistance){
	
	if((Temperature > 200) && (Resistance < 45000)) batterystatus = 1;  //there are both resistance and thermistor values read, so there must be a battery installed. 
	else batterystatus = 0;
	if (BatteryPresent && (batterystatus = 1)) batterystatus = 2;
	//if ((Volts > 90) && batterystatus == 1)		
	return batterystatus;
}
//------------------------------------------------------------------------------------------------------------------------------
void NewSetUnfinishedBusiness(uint32_t Business, uint32_t *UnfinishedBusiness){
	
	*UnfinishedBusiness |= Business;
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void NewClearUnfinishedBusiness(uint32_t Business, uint32_t *UnfinishedBusiness){
	
	*UnfinishedBusiness |= Business;
	*UnfinishedBusiness ^= Business;
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
int MeasureSixVoltBattery(bool *BatteryOK,int SixV_adc_cal,bool SYBTisProvidingPower){
	int SixVoltBattery;	
	PWR_ON_set_level(true);  // turn on 6v battery
	ADC_0_get_conversion(ADC_Six_Volts); 
//	SixVoltBattery = 610;   // test adc program
	SixVoltBattery = ((float)SixV_adc_cal/1000 * (ADC_0_get_conversion(ADC_Six_Volts) >> 2));  //0.649 used in Gen3#1  0.654 used in Gen3#2, 1.25 used in Gen3#3
	//SixVoltBattery = ((float)0.649 * (ADC_0_get_conversion(ADC_Six_Volts) >> 2));  //0.649 used in Gen3#1  0.654 used in Gen3#2, 1.25 used in Gen3#3
	if(SYBTisProvidingPower)PWR_ON_set_level(false);  // turn off 6v battery
	if(SixVoltBattery > 475)*BatteryOK = true; else *BatteryOK = false;
	return SixVoltBattery;
}
//------------------------------------------------------------------------------------------------------------------------------
void GatherSYBTflavorData(void){
	char Flavorstring[60];
	memset(Flavorstring,0,60*sizeof(char)); // number of items: 8
	// number of items: 8
	sprintf(Flavorstring,"Updt:%3s,%5lu,%3s,%5lu,%3s,%5lu,%3s,%5lu eep\r\n",cal.FlavrName1,cal.FlavrVal1,cal.FlavrName2,cal.FlavrVal2,cal.FlavrName3,cal.FlavrVal3,cal.FlavrName4,cal.FlavrVal4);
	//sprintf(string2,"F1%3s,%5lu,F2%3s",calibration.FlavrName1,calibration.FlavrVal1,calibration.FlavrName2);
	//sprintf(string,"%5lu,F3%3s,%5lu\r\n",calibration.FlavrVal2,calibration.FlavrName3,calibration.FlavrVal3);
	//strcat(string,string2);
	NewSetUnfinishedBusiness(Unfinished_SYBT_Flavor_TX,&Unfinished_Business);
	TransmitFlavorData(Flavorstring,true);
	true;
}
//------------------------------------------------------------------------------------------------------------------------------
bool OvercurrentShutdown(int ResistorZeroValue){
	
}
//------------------------------------------------------------------------------------------------------------------------------
uint16_t Measure24voltSupply(void){
		//uint8_t twentyfourvoltsupply;
		ADC_0_get_conversion(ADC_TwentyFourVolts);
		twentyfourvoltsupply = ((ADC_0_get_conversion(ADC_TwentyFourVolts) >> 2)/2);  //0.649 used in Gen3#1  0.654 used in Gen3#2, 1.25 used in Gen3#3.034 (float).068 * 
		return twentyfourvoltsupply;
}
//------------------------------------------------------------------------------------------------------------------------------
