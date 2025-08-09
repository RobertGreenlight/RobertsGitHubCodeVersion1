/*
 * EepromFunctions.c
 *
 * Created: 7/29/2024 9:06:10 AM
 *  Author: Robert F
 */ 

#include "eeprom_functions.h"
#include "bobs_functions.h"
#include "nvmctrl_basic_example.h"
#include "import_preferences.h"
#include <string.h>
#include <stdio.h>
#include <nvmctrl_basic.h>
extern int16_t number_of_loops;
#include <float.h>

// Change firmware to reflect "s" for small chip and "L" for large chips. Located in Bluetooth data out.
//Set large or small chip in bobs_functions.h
// change define for large or small chips in bobs_functions.h, atmel_start_pins.h, main.c, and driver_init.c
bool CheckEEPROMvalues(){
//	uint8_t EEPROMstatus;
	uint8_t buffer[30];

	memset(buffer,0,30*sizeof(uint8_t));
	


	FLASH_0_read_eeprom_byte(EEPROM_STATUS_ADDR);  // not used?
	
	FLASH_0_read_eeprom_block(EEPROM_SYBT_AD_Cal,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.SYBT_ad_cal);
	FLASH_0_read_eeprom_block(EEPROM_LOAD_AD_Cal,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.Load_ad_cal);
	FLASH_0_read_eeprom_block(EEPROM_9VOLT_AD_Cal,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.Nine_V_ad_cal);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Resistor_A,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.ResistA_cal);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Resistor_B,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.ResistB_cal);
	cal.Preferences = FLASH_0_read_eeprom_byte(EEPROM_PREFERENCES_ADDR);
	FLASH_0_read_eeprom_block(EEPROM_Bluetooth_Name,(void*)&buffer,24);
	sscanf(buffer,"%24s",Bluetooth_Name);										// don't need "&" when it is the beginning of a string. 
	FLASH_0_read_eeprom_block(EEPROM_New_Bluetooth_Name,(void*)&buffer,1);
	sscanf(buffer,"%1d",&New_Bluetooth_Name);		  
	FLASH_0_read_eeprom_block(EEPROM_SYBT_FlavName1,(void*)&buffer,4);
	sscanf(buffer,"%3s",cal.FlavrName1);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_FlavResis1,(void*)&buffer,6);
	sscanf(buffer,"%5lu",&cal.FlavrVal1);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_FlavName2,(void*)&buffer,4);
	sscanf(buffer,"%3s",cal.FlavrName2);									// don't need "&" when it is the beginning of a string. 
	FLASH_0_read_eeprom_block(EEPROM_SYBT_FlavResis2,(void*)&buffer,6);
	sscanf(buffer,"%5lu",&cal.FlavrVal2);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_FlavName3,(void*)&buffer,4);
	sscanf(buffer,"%3s",cal.FlavrName3);									// don't need "&" when it is the beginning of a string. 
	FLASH_0_read_eeprom_block(EEPROM_SYBT_FlavResis3,(void*)&buffer,6);
	sscanf(buffer,"%5lu",&cal.FlavrVal3);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_FlavName4,(void*)&buffer,4);
	sscanf(buffer,"%3s",cal.FlavrName4);									// don't need "&" when it is the beginning of a string. 
	FLASH_0_read_eeprom_block(EEPROM_SYBT_FlavResis4,(void*)&buffer,6);
	sscanf(buffer,"%5lu",&cal.FlavrVal4);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Thermistor_Offset,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.SYBT_Therm_Offst);
	FLASH_0_read_eeprom_block(EEPROM_Int_Thermistor_Offset,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.Intrnl_Therm_Offst);
	FLASH_0_read_eeprom_block(EEPROM_Int_Thermistor1,(void*)&buffer,20);
	sscanf(buffer,"%lf",&cal.Int_Thermist1_cal);
	FLASH_0_read_eeprom_block(EEPROM_Int_Thermistor2,(void*)&buffer,20);
	sscanf(buffer,"%lf",&cal.Int_Thermist2_cal);
	FLASH_0_read_eeprom_block(EEPROM_Int_Thermistor3,(void*)&buffer,20);
	sscanf(buffer,"%lf",&cal.Int_Thermist3_cal);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Thermistor1,(void*)&buffer,20);
	sscanf(buffer,"%lf",&cal.Thermist1_cal);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Thermistor2,(void*)&buffer,20);
	sscanf(buffer,"%lf",&cal.Thermist2_cal);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Thermistor3,(void*)&buffer,20);
	sscanf(buffer,"%lf",&cal.Thermist3_cal);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Exponent1,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.Thermist1_Exp);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Exponent2,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.Thermist2_Exp);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Exponent3,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.Thermist3_Exp);
	FLASH_0_read_eeprom_block(EEPROM_Int_Exponent1,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.Int_Thermist1_Exp);			 
	FLASH_0_read_eeprom_block(EEPROM_Int_Exponent2,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.Int_Thermist2_Exp);
	FLASH_0_read_eeprom_block(EEPROM_Int_Exponent3,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.Int_Thermist3_Exp);
		//FLASH_0_read_eeprom_block(EEPROM_Preference_Bits,(void*)&buffer,2);
		//sscanf(buffer,"%i",&calibration.Preferences);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Therm_Res_A,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.Therm_ResA_cal);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Therm_Res_B,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.Therm_ResB_cal);
	FLASH_0_read_eeprom_block(EEPROM_Int_Therm_Res_A,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.Int_Therm_ResA_cal);
	FLASH_0_read_eeprom_block(EEPROM_Int_Therm_Res_B,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.Int_Therm_ResB_cal);
	FLASH_0_read_eeprom_block(EEPROM_6Volt_Battery_Cal,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.Six_V_ad_cal);
	FLASH_0_read_eeprom_block(EEPROM_SYBT_Ident_Tolerance,(void*)&buffer,2);
	sscanf(buffer,"%2d",&cal.SYBT_Idnt_Tolrnc);
	FLASH_0_read_eeprom_block(EEPROM_LoadTestTolerance_A,(void*)&buffer,3);
	sscanf(buffer,"%3d",&cal.LoadTestTolerance_A);
	FLASH_0_read_eeprom_block(EEPROM_LoadTestTolerance_B,(void*)&buffer,2);
	sscanf(buffer,"%2d",&cal.LoadTestTolerance_B);
	
	FLASH_0_read_eeprom_block(EEPROM_HS_Therm_Res_A,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.HS_Therm_ResA_cal);
	FLASH_0_read_eeprom_block(EEPROM_HS_Therm_Res_B,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.HS_Therm_ResB_cal);
	FLASH_0_read_eeprom_block(EEPROM_HS_Exponent1,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.HS_Thermist1_Exp);
	FLASH_0_read_eeprom_block(EEPROM_HS_Exponent2,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.HS_Thermist2_Exp);
	FLASH_0_read_eeprom_block(EEPROM_HS_Exponent3,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.HS_Thermist3_Exp);
	FLASH_0_read_eeprom_block(EEPROM_HS_Thermistor_Offset,(void*)&buffer,4);
	sscanf(buffer,"%d",&cal.HS_Thermist_Offset);
	
	FLASH_0_read_eeprom_block(EEPROM_LXbattery_AD_Cal,(void*)&buffer,4);
	sscanf(buffer,"%4d",&cal.LXvoltage_cal);
	
	RestoreSleepSettings();									//separated out so it can be used to restore value after exiting demo mode
	
//		FLASH_0_read_eeprom_block(EEPROM_Firmware_Version,(void*)&buffer,30);
//		sscanf(buffer,"%s",&Firmware_Version);
			  
				 //sscanf(buffer,"CA1%i",&temp3);
		//if (!(EEPROMstatus & EEPROM_SYBT_AD_STAT))calibration.SYBT_ad_cal = FLASH_0_read_eeprom_byte(EEPROM_SYBT_AD_Cal);
		//if (!(EEPROMstatus & EEPROM_SYBT_AD_STAT))FLASH_0_write_eeprom_byte(EEPROM_SYBT_AD_Cal, 57);
return true;	
}

void RestoreSleepSettings(void){
	uint8_t buffer[30];
	memset(buffer,0,30*sizeof(uint8_t));
	FLASH_0_read_eeprom_block(EEPROM_Sleep_1,(void*)&buffer,2);
	sscanf(buffer,"%2lu",&cal.Sleep_1);
	cal.Sleep_1 *= 10000;
	FLASH_0_read_eeprom_block(EEPROM_Sleep_2,(void*)&buffer,2);
	sscanf(buffer,"%2lu",&cal.Sleep_2);
	cal.Sleep_2 *= 10000;
	FLASH_0_read_eeprom_block(EEPROM_Sleep_3,(void*)&buffer,2);
	sscanf(buffer,"%2lu",&cal.Sleep_3);
	cal.Sleep_3 *= 10000;
}

/*
bool SendSYBTflavors(){
	char string[50];
	memset(string,0,50*sizeof(char)); // number of items: 8
// number of items: 8
	sprintf(string,"Updt:%3s,%5lu,%3s,%5lu,%3s,%5lu,%3s,%5lu eep\n",cal.FlavrName1,cal.FlavrVal1,cal.FlavrName2,cal.FlavrVal2,cal.FlavrName3,cal.FlavrVal3,cal.FlavrName4,cal.FlavrVal4); 
	//sprintf(string2,"F1%3s,%5lu,F2%3s",calibration.FlavrName1,calibration.FlavrVal1,calibration.FlavrName2); 
	//sprintf(string,"%5lu,F3%3s,%5lu\n",calibration.FlavrVal2,calibration.FlavrName3,calibration.FlavrVal3);
	//strcat(string,string2); 
	USART0_sendString(string);
	return true;					
}
//-----------------------------------------------------------------------------------------------------------------

bool SendThermistorCal(){
	char string[300];

	memset(string,0,300*sizeof(char));
	// number of items: 27
	sprintf(string,"TCal:%lf,%lf,%lf,%f,%lf,%lf,%lf,%f,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i eof\n",cal.Thermist1_cal,cal.Thermist2_cal,cal.Thermist3_cal,
	cal.Thermist_Offset,cal.Int_Thermist1_cal,cal.Int_Thermist2_cal,cal.Int_Thermist3_cal,cal.Int_Thermist_Offset,
	cal.Thermist1_Exp,cal.Thermist2_Exp,cal.Thermist3_Exp,cal.Int_Thermist1_Exp,cal.Int_Thermist2_Exp,cal.Int_Thermist3_Exp,
	cal.SYBT_ad_cal,cal.Load_ad_cal,cal.Nine_V_ad_cal,cal.ResistA_cal,cal.ResistB_cal,
	cal.Therm_ResA_cal,cal.Therm_ResB_cal,cal.Int_Therm_ResA_cal,cal.Int_Therm_ResB_cal,cal.Six_V_ad_cal,cal.LoadTestTolerance_A,cal.LoadTestTolerance_B,cal.SYBT_Idnt_Tolrnc);

	USART0_sendString(string);
	return true;
}
//--------------------------------------------------------------------------------------
bool SendPreferences(){
	char string[50];
	memset(string,0,50*sizeof(char));
	// number  of items:3
	//calibration.Preference_Sleep = 1234;
	sprintf(string,"Pref:%i,%lu,%s,%s eop\n",cal.Preferences,cal.Sleep_3,Bluetooth_Name,Firmware_Version);  //version is defined at top of this file
	USART0_sendString(string);
	return true;
}
*/
//FLASH_0_write_eeprom_block(EEPROM_c1,buffer+5,20);// this works
	
//FLASH_0_write_eeprom_byte(1, 57);