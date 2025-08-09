/*
 * eeprom_functions.h
 *
 * Created: 7/29/2024 9:13:07 AM
 *  Author: Robert F
 */ 

#include <compiler.h>

#ifndef EEPROM_FUNCTIONS_H_
#define EEPROM_FUNCTIONS_H_


#define EEPROM_STATUS_ADDR 1				// 2 bytes
#define EEPROM_PREFERENCES_ADDR 3			// single byte
#define EEPROM_SYBT_AD_Cal 10				// 4 
#define EEPROM_9VOLT_AD_Cal 15				// 4
#define EEPROM_LOAD_AD_Cal 20				// 4
#define EEPROM_SYBT_Resistor_A 25			// 4
#define EEPROM_SYBT_Resistor_B 30			// 4
#define EEPROM_Bluetooth_Name 35			// 32
#define EEPROM_New_Bluetooth_Name 68		// 2
#define EEPROM_SYBT_FlavName1 70			// 4
#define EEPROM_SYBT_FlavResis1 75			// 4
#define EEPROM_SYBT_FlavName2 81			// 5
#define EEPROM_SYBT_FlavResis2 86			// 6
#define EEPROM_SYBT_FlavName3 92			// 5
#define EEPROM_SYBT_FlavResis3 97			// 6
#define EEPROM_SYBT_FlavName4 103			// 5
#define EEPROM_SYBT_FlavResis4 108			// 6
#define EEPROM_SYBT_Thermistor_Offset 114   // 10
#define EEPROM_Int_Thermistor_Offset 124    // 10
#define EEPROM_Int_Thermistor1 134			// 20
#define EEPROM_Int_Thermistor2 154			// 20
#define EEPROM_Int_Thermistor3 174			// 20
#define EEPROM_SYBT_Thermistor1 194			// 20
#define EEPROM_SYBT_Thermistor2 214			// 20
#define EEPROM_SYBT_Thermistor3 234			// 20
#define EEPROM_Int_Exponent1 254			// 4
#define EEPROM_Int_Exponent2 258			// 4
#define EEPROM_Int_Exponent3 262			// 4
#define EEPROM_SYBT_Exponent1 266			// 4
#define EEPROM_SYBT_Exponent2 270			// 4
#define EEPROM_SYBT_Exponent3 274			// 4
#define EEPROM_Preference_Bits 278			// 2
#define EEPROM_Sleep_3 280					// 5
#define EEPROM_SYBT_Therm_Res_A 285			// 4
#define EEPROM_SYBT_Therm_Res_B 289			// 4
#define EEPROM_Int_Therm_Res_A 293			// 4
#define EEPROM_Int_Therm_Res_B 297			// 4
#define EEPROM_Pref_Bluetooth_Sleep 301		// 4
#define EEPROM_6Volt_Battery_Cal 305		// 4
#define EEPROM_Sleep_1 308					// 3
#define EEPROM_Sleep_2 311					// 3
#define EEPROM_SYBT_Ident_Tolerance 314		// 3
#define EEPROM_LoadTestTolerance_A 317		// 3
#define EEPROM_LoadTestTolerance_B 320		// 3
#define EEPROM_HS_Thermistor_Offset 323    // 10
#define EEPROM_HS_Thermistor1 333			// 20
#define EEPROM_HS_Thermistor2 353			// 20
#define EEPROM_HS_Thermistor3 373			// 20
#define EEPROM_HS_Exponent1 377				// 4
#define EEPROM_HS_Exponent2 381				// 4
#define EEPROM_HS_Exponent3 385				// 4
#define EEPROM_HS_Therm_Res_A 389			// 4
#define EEPROM_HS_Therm_Res_B 393			// 4

#define EEPROM_c1 400						// to 429
#define EEPROM_c2 430						// to 459
#define EEPROM_c3 460						// to 489

#define EEPROM_LXbattery_AD_Cal 490			// 4

#define EEPROM_C1C2C3_STAT 1			
#define EEPROM_SYBT_AD_STAT 2				
#define EEPROM_9VOLT_AD_STAT 4			
#define EEPROM_LOAD_AD_STAT 8			
#define EEPROM_SYBT_ResistA_STAT 16	
#define EEPROM_SYBT_ResistB_STAT 32	
#define EEPROM_SYBT_Therm1_STAT 64	
#define EEPROM_SYBT_Therm2_STAT 128	
#define EEPROM_SYBT_Therm3_STAT 256		

#define EEPROM_SYBT_Therm_Res_A_STAT 1
#define EEPROM_SYBT_Therm_Res_B_STAT 2
#define EEPROM_6VOLT_AD_STAT 4
#define EEPROM_Int_Therm_Res_A_STAT 8
#define EEPROM_Int_Therm_Res_B_STAT 16
#define EEPROM_HS_Therm_Res_A_STAT 32
#define EEPROM_HS_Therm_Res_B_STAT 64

bool CheckEEPROMvalues(void);
void RestoreSleepSettings(void);


#endif /* EEPROM_FUNCTIONS_H_ */