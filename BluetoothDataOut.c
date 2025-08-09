/*
 * BluetoothDataOut.c
 *
 * Created: 7/29/2024 9:07:31 AM
 *  Author: Robert F
 */ 
//Last modified 06172025

#include <stdio.h>
#include <string.h>
#include <driver_init.h>
#include <util/delay.h>
#include <nvmctrl_basic_example.h>
#include "bluetooth_data_in.h"
#include "import_preferences.h"
#include "bobs_functions.h"
#include <math.h>
#include <float.h>

extern char Bluetooth_Name [25];

#ifdef FifthGen
	#define Firmware_Version "5.10504HS"   // Change firmware to reflect "s" for small chip and "L" for large chips. Set large or small chip in bobs_functions.h
#endif

#ifdef LXbatteryCompatible
	#define Firmware_Version "5.32924LX"   // Change firmware to reflect "s" for small chip and "L" for large chips. Set large or small chip in bobs_functions.h
#endif

char RepeatBuffer[90];
extern uint32_t Unfinished_Business;
//----------------------------------------------------------------------------------------------------------
void TransmitBatteryData(char buffer[100],bool retransmit){
	static char BatteryDataBuffer[100];
//	static char BufferLength[10];
	static uint8_t NumberAttempts;
//	int StringLength;
	if(retransmit){
		strcpy(BatteryDataBuffer,buffer);
		NumberAttempts = 0;	
	}
	else{NumberAttempts++;
		if(NumberAttempts>0)NewClearUnfinishedBusiness(Unfinished_BatDataRetransmit,&Unfinished_Business);
		NumberAttempts = 0;
	}
		USART0_sendString(BatteryDataBuffer);
}
//----------------------------------------------------------------------------------------------------------
void TransmitBatteryTestData(char buffer[70],bool retransmit){
	static char TestDataBuffer[70];
	static uint8_t NumberAttempts;
	if(retransmit){
		strcpy(TestDataBuffer,buffer);
		NumberAttempts = 0;
	}
	else NumberAttempts++;
	
	USART0_sendString(TestDataBuffer);
}
//-----------------------------------Makes up to 3 attempts to send cal data------------------------------------------------------------------
void TransmitADcalData(char buffer[30],bool retransmit){
	static char ADcalDataBuffer[30];
	static uint8_t NumADcalAttmpt;
	if(retransmit){
		strcpy(ADcalDataBuffer,buffer);
		NumADcalAttmpt = 1;
	}
	else{
		NumADcalAttmpt++;
		if(NumADcalAttmpt>2){
			NewClearUnfinishedBusiness(Unfinished_AutoCalRetransmit,&Unfinished_Business);
			NumADcalAttmpt = 0;
		}
	}
	USART0_sendString(ADcalDataBuffer);
}
//-----------------------------------Makes up to 3 attempts to send cal data------------------------------------------------------------------
void TransmitManualCalData(char buffer[300],bool retransmit){
	static char ManualCalDataBuffer[300];
	static uint8_t NumManCalAttmpt;
	if(retransmit){
		strcpy(ManualCalDataBuffer,buffer);
		NumManCalAttmpt = 1;
	}
	else{
		NumManCalAttmpt++;
		if(NumManCalAttmpt>2){
			NewClearUnfinishedBusiness(Unfinished_ManualCalRetransmit,&Unfinished_Business);
			NumManCalAttmpt = 0;
		}
	}
	USART0_sendString(ManualCalDataBuffer);
}
//-----------------------------------------------------------------------------------------------------------------
void TransmitFlavorData(char buffer[50],bool retransmit){
	static char FlavorDataBuffer[50];
	static uint8_t NumFlavorAttmpt;
	if(retransmit){
		strcpy(FlavorDataBuffer,buffer);
		NumFlavorAttmpt = 1;
	}
	else{
		NumFlavorAttmpt++;
		if(NumFlavorAttmpt>2){
			NewClearUnfinishedBusiness(Unfinished_SYBT_Flavor_TX,&Unfinished_Business);
			NumFlavorAttmpt = 0;
		}
	}
	USART0_sendString(FlavorDataBuffer);
}
//-----------------------------------------------------------------------------------------------------------------
/*
bool SendSYBTflavors(){
	char string[50];
	memset(string,0,50*sizeof(char)); // number of items: 8
	// number of items: 8
	sprintf(string,"Updt:%3s,%5lu,%3s,%5lu,%3s,%5lu,%3s,%5lu eep\r\n",cal.FlavrName1,cal.FlavrVal1,cal.FlavrName2,cal.FlavrVal2,cal.FlavrName3,cal.FlavrVal3,cal.FlavrName4,cal.FlavrVal4);
	//sprintf(string2,"F1%3s,%5lu,F2%3s",calibration.FlavrName1,calibration.FlavrVal1,calibration.FlavrName2);
	//sprintf(string,"%5lu,F3%3s,%5lu\r\n",calibration.FlavrVal2,calibration.FlavrName3,calibration.FlavrVal3);
	//strcat(string,string2);
	USART0_sendString(string);
	return true;
}
*/
//-----------------------------------------------------------------------------------------------------------------
bool SendThermistorCal(){
	char string[360];
	char string2[60];
cal.Thermist1_cal = 0;
cal.Thermist2_cal = 0;
cal.Thermist3_cal = 0;

cal.Int_Thermist1_cal = 0;
cal.Int_Thermist2_cal = 0;
cal.Int_Thermist3_cal = 0;


//	sprintf(string,"TCal:%lf,%lf,%lf,%f,%lf,%lf,%lf,%f,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i eof\r\n",cal.Thermist1_cal,cal.Thermist2_cal,cal.Thermist3_cal,

	memset(string,0,360*sizeof(char));
	// number of items: 30
	/*
		sprintf(string,"TCal:%lf,%lf,%lf,%f,%lf,%lf,%lf,%f,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,&i,&i,&lf eof\r\n",cal.Thermist1_cal,cal.Thermist2_cal,cal.Thermist3_cal,cal.Thermist_Offset,
		cal.Int_Thermist1_cal,cal.Int_Thermist2_cal,cal.Int_Thermist3_cal,cal.Int_Thermist_Offset,cal.Thermist1_Exp,cal.Thermist2_Exp,cal.Thermist3_Exp,cal.Int_Thermist1_Exp,cal.Int_Thermist2_Exp,
		cal.Int_Thermist3_Exp,cal.SYBT_ad_cal,cal.Load_ad_cal,cal.Nine_V_ad_cal,cal.ResistA_cal,cal.ResistB_cal,cal.Therm_ResA_cal,cal.Therm_ResB_cal,cal.Int_Therm_ResA_cal,cal.Int_Therm_ResB_cal,
		cal.Six_V_ad_cal,cal.LoadTestTolerance_A,cal.LoadTestTolerance_B,cal.SYBT_Idnt_Tolrnc,cal.HS_Therm_ResA_cal,cal.HS_Therm_ResB_cal,cal.HS_Thermist_Offset);
	*/
	// the max number of items appears to be 27. I used strcat to add 3 more.
	sprintf(string,"TCal:%lf,%lf,%lf,%i,%lf,%lf,%lf,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,",cal.Thermist1_cal,cal.Thermist2_cal,cal.Thermist3_cal,cal.SYBT_Therm_Offst,
	cal.Int_Thermist1_cal,cal.Int_Thermist2_cal,cal.Int_Thermist3_cal,cal.Intrnl_Therm_Offst,cal.Thermist1_Exp,cal.Thermist2_Exp,cal.Thermist3_Exp,cal.Int_Thermist1_Exp,cal.Int_Thermist2_Exp,
	cal.Int_Thermist3_Exp,cal.SYBT_ad_cal,cal.Load_ad_cal,cal.Nine_V_ad_cal,cal.ResistA_cal,cal.ResistB_cal,cal.Therm_ResA_cal,cal.Therm_ResB_cal,cal.Int_Therm_ResA_cal,cal.Int_Therm_ResB_cal,
	cal.Six_V_ad_cal,cal.LoadTestTolerance_A,cal.LoadTestTolerance_B,cal.SYBT_Idnt_Tolrnc);
	#ifdef LXbatteryCompatible
	sprintf(string2,"%i,%i,%i,%i eof\r\n",cal.HS_Therm_ResA_cal,cal.HS_Therm_ResB_cal,cal.HS_Thermist_Offset,cal.LXvoltage_cal);
	#endif
	#ifndef LXbatteryCompatible
	sprintf(string2,"%i,%i,%i eof\r\n",cal.HS_Therm_ResA_cal,cal.HS_Therm_ResB_cal,cal.HS_Thermist_Offset);
	#endif
	
	strcat(string,string2);

	NewSetUnfinishedBusiness(Unfinished_ManualCalRetransmit,&Unfinished_Business);			//The output is buffered and retransmitted if the app doesn't respond.
	TransmitManualCalData(string,true);
//	USART0_sendString(string);
	return true;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void GatherPreferenceData(){
	char string[60];
	memset(string,0,60*sizeof(char));
	sprintf(string,"Pref:%i,%lu,%s,%s eop\r\n",cal.Preferences,cal.Sleep_3,Bluetooth_Name,Firmware_Version);  //version is defined at top of this file
	NewSetUnfinishedBusiness(Unfinished_Preferences_TX,&Unfinished_Business);
	TransmitPreferenceData(string,true);
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------
void TransmitPreferenceData(char buffer[60],bool retransmit ){
	static char PreferenceDataBuffer[60];
	static uint8_t NumPreferenceAttmpt;
	if(retransmit){
		strcpy(PreferenceDataBuffer,buffer);
		NumPreferenceAttmpt = 1;
	}
	else{
		NumPreferenceAttmpt++;
		if(NumPreferenceAttmpt>2){
			NewClearUnfinishedBusiness(Unfinished_Preferences_TX,&Unfinished_Business);
			NumPreferenceAttmpt = 0;
		}
	}
	USART0_sendString(PreferenceDataBuffer);		
}

//---------------------------using Sleep timer to time transmissions to avoid collisions------------------------------------------
void UnfinishedTXbusiness(void){

	if(Unfinished_Business & Unfinished_BatDataRetransmit){
		TransmitBatteryData("",false);
		Unfinished_Business ^= Unfinished_BatDataRetransmit;				//here to prevent retransmissions for now
		return;
	}
	if(Unfinished_Business & Unfinished_AutoCalRetransmit){
		TransmitADcalData("",false);										// The function resets the unfinished bis flag
		return;
	}
	if(Unfinished_Business & Unfinished_ManualCalRetransmit){
		TransmitManualCalData("",false);									// The function resets the unfinished bis flag
		return;
	}
	if(Unfinished_Business & Unfinished_SYBT_Flavor_TX){
		TransmitFlavorData("",false);										// The function resets the unfinished bis flag
		return;
	}
	if(Unfinished_Business & Unfinished_Therm_Cal_TX){
		SendThermistorCal();
		Unfinished_Business ^= Unfinished_Therm_Cal_TX;
	}
	if(Unfinished_Business & Unfinished_Preferences_TX){
		TransmitPreferenceData("",false);									// The function resets the unfinished bis flag
	}
	return;
}