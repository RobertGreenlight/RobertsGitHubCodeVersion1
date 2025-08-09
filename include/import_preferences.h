/*
 * import_preferences.h
 *
 * Created: 7/29/2024 9:12:22 AM
 *  Author: Robert F
 */ 

#include <compiler.h>
#include <avr/eeprom.h>

#ifndef IMPORT_PREFERENCES_H_
#define IMPORT_PREFERENCES_H_

struct cal{
	int Cal_Status1;
	int Cal_Status2;
	int SYBT_ad_cal;
	int Nine_V_ad_cal;
	int Six_V_ad_cal;
	int Load_ad_cal;
	int ResistA_cal;
	int ResistB_cal;
	int Therm_ResA_cal;
	int Therm_ResB_cal;
	double Thermist1_cal;
	double Thermist2_cal;
	double Thermist3_cal;
	int SYBT_Therm_Offst;
	int Thermist1_Exp;
	int Thermist2_Exp;
	int Thermist3_Exp;
	int Int_Therm_ResA_cal;
	int Int_Therm_ResB_cal;
	double Int_Thermist1_cal;
	double Int_Thermist2_cal;
	double Int_Thermist3_cal;
	int Intrnl_Therm_Offst;
	int Int_Thermist1_Exp;
	int Int_Thermist2_Exp;
	int Int_Thermist3_Exp;	
	int HS_Therm_ResA_cal;
	int HS_Therm_ResB_cal;
	double HS_Thermist1_cal;
	double HS_Thermist2_cal;
	double HS_Thermist3_cal;
	int HS_Thermist_Offset;
	int HS_Thermist1_Exp;
	int HS_Thermist2_Exp;
	int HS_Thermist3_Exp;	
	uint16_t Preferences;
	char FlavrName1[4];
	uint32_t FlavrVal1;
	char FlavrName2[4];
	uint32_t FlavrVal2;
	char FlavrName3[4];
	uint32_t FlavrVal3;
	char FlavrName4[4];
	uint32_t FlavrVal4;
	uint32_t Sleep_1;
	uint32_t Sleep_2;
	uint32_t Sleep_3;
	int SYBT_Idnt_Tolrnc;
	int LoadTestTolerance_A;
	int LoadTestTolerance_B;
	int StartandRunCalibration;
	int CalibrationValue66;
	int CalibrationValue10;
	int CalibrationValue20; 
	int CalibrationValue69;
	int LXvoltage_cal;

}cal;

int SYBTcount;

uint8_t Rows;
uint8_t EEpromData1;
uint8_t EEpromData2 [20];
uint8_t EEpromData3;
char Resistance_String [20];
extern char Bluetooth_Name [25];
extern int New_Bluetooth_Name;



#endif /* IMPORT_PREFERENCES_H_ */