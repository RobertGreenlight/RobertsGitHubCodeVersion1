/*
 * BluetoothDataIn.c
 *
 * Created: 7/29/2024 9:08:21 AM
 *  Author: Robert F
 */ 

#include <stdio.h>
#include <string.h>
#include <driver_init.h>
#include <util/delay.h>
#include <nvmctrl_basic_example.h>
#include <wdt.h>
#include "bluetooth_data_in.h"
#include "Seven_Seg_LED.h"
#include "eeprom_functions.h"
#include "import_preferences.h"
#include "bobs_functions.h"

extern uint32_t Unfinished_Business;
char Bluetooth_Name [25];
extern bool OverrideSafeties;

void USART0_getString(void)  //
{
//BlueLED_set_level(false);	
uint8_t blue_buffer[30];
memset(blue_buffer,0, 30*sizeof(uint8_t));
for (int i = 0; i < 30; i++){		
	blue_buffer[i] = USART_0_read();
	
	
	if(blue_buffer[i] == 0x0D || blue_buffer[i] == 0x0A){       //  <- could be chR < ' '
		break;
		}
	}
identify_commands(blue_buffer);	//see if the received characters match one of the 3 letter commands below
//BlueLED_set_level(true);
return;
}
//-----------------------------------------------------------------------------------------------------------------------------
void identify_commands(uint8_t buffer[30]){
	char string[30];
	memset(string,0,30*sizeof(char));
//.1.........................................LED test on--------------------------------------------Looks for "RLY"-------------
	if (buffer[0] == ASCII_R && buffer[1] == ASCII_L && buffer[2] == ASCII_Y) ATTINY_PA6_set_level(true);
//.1a.........................................LED test on--------------------------------------------Looks for "RLN"-------------
if (buffer[0] == ASCII_R && buffer[1] == ASCII_L && buffer[2] == ASCII_N) ATTINY_PA6_set_level(false);	
//.2.........................................Blank Display On----------------------------------------Looks for "CPY"-------------
else if (buffer[0] == ASCII_C && buffer[1] == ASCII_P && buffer[2] == ASCII_Y){
	MarchingAnts_set_level(true);
	ReverseMarchingAnts_set_level(true);
}
//.2.........................................Blank Display Off---------------------------------------Looks for "CPN"-------------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_P && buffer[2] == ASCII_N){
	MarchingAnts_set_level(false);
	ReverseMarchingAnts_set_level(false);	
}
//.3.........................................Sets SYBT voltage AD calibration------------------------Looks for "CA1"-------------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_A && buffer[2] == ASCII_1) SYBTvoltsADcal(buffer);	
//.4.........................................Sets Load voltage AD calibration------------------------Looks for "CA2"-------------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_A && buffer[2] == ASCII_2) LoadvoltsADcal(buffer);			
//.5..........................................Sets 9 Volt battery calibration------------------------Looks for "CA3"-------------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_A && buffer[2] == ASCII_3) NineVoltBatteryCal(buffer);		
//.6..........................................Sets 6 Volt battery calibration------------------------Looks for "CA4"-------------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_A && buffer[2] == ASCII_4) SixVoltBatteryCal(buffer);		
//.7..........................................Sets ohmmeter calibration A----------------------------Looks for "RA1"-------------
	else if (buffer[0] == ASCII_R && buffer[1] == ASCII_A && buffer[2] == ASCII_1) OhmmeterCalA(buffer);		
//.8..........................................Sets ohmmeter calibration B----------------------------Looks for "RA2"-------------
	else if (buffer[0] == ASCII_R && buffer[1] == ASCII_A && buffer[2] == ASCII_2) OhmmeterCalB(buffer);
//.9..........................................Sets SYBT thermistor resistor calibration A------------Looks for "TR1"-------------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_R && buffer[2] == ASCII_1) ThermistorCalA(buffer);	
//.10..........................................Sets SYBT thermistor resistor calibration B------------Looks for "TR2"-------------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_R && buffer[2] == ASCII_2) ThermistorCalB(buffer);
//.11..........................................Sets SYBT thermistor calibration 1---------------------Looks for "TH1"-------------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_H && buffer[2] == ASCII_1) ThermistorCalOne(buffer);	
//.12..........................................Sets SYBT thermistor calibration 2.........Looks for "TH2".......These need work if they are ever implemented
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_H && buffer[2] == ASCII_2) ThermistorCal2(buffer);
//.13..........................................Sets SYBT thermistor calibration 3.......Looks for "TH3".........Currently none of the thermistor cal factors are used
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_H && buffer[2] == ASCII_3) ThermistorCal3(buffer);		
//.14..........................................Sets SYBT thermistor offset.........Looks for "TH4"..this should be changed to be consistent with other code formats for saving data
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_H && buffer[2] == ASCII_4) ThermistorOffset(buffer);			
//.15..........................................Sets Int thermistor calibration 1-----------------------Looks for "TH5"--------------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_H && buffer[2] == ASCII_5) IntThermistorCalOne(buffer);					
//.16..........................................Sets Int thermistor calibration 2-----------------------Looks for "TH6"--------------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_H && buffer[2] == ASCII_6) IntThermistorCal2(buffer);	
//.17..........................................Sets Int thermistor calibration 3------Looks for "TH7"..Currently none of the thermistor cal factors are used
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_H && buffer[2] == ASCII_7) IntThermistorCal3(buffer);				
//.18..........................................Sets Int thermistor offset...........Looks for "TH8"..should be changed to be consistent with other data saving formats.but works ok
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_H && buffer[2] == ASCII_8) IntThermistorOffset(buffer);	
//.19..........................................Sets internal thermistor resistor calibration A------------Looks for "TN1"-----------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_N && buffer[2] == ASCII_1) IntThermistorCalA(buffer);
//.20..........................................Sets internal thermistor resistor calibration B------------Looks for "TN2"-----------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_N && buffer[2] == ASCII_2) IntThermistorCalB(buffer);					
//.21..........................................Sets F or C preference-------------------------------------Looks for "FOC"-----------
	else if (buffer[0] == ASCII_F && buffer[1] == ASCII_O && buffer[2] == ASCII_C) FarenheitOrCentigrade(buffer,string);
//.22..........................................Sets Temp or Resistance preference-------------------------Looks for "TOR"-----------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_O && buffer[2] == ASCII_R) TemperatureOrResistance(buffer,string);
//.23..........................................Sets SYBT type or resistance preference--------------------Looks for "SOR"-----------
	else if (buffer[0] == ASCII_S && buffer[1] == ASCII_O && buffer[2] == ASCII_R) TypeOrResistance(buffer,string);		
//.24..........................................Sets AutoMeasaurement On or Off preference-----------------Looks for "AOF"-----------
	else if (buffer[0] == ASCII_A && buffer[1] == ASCII_O && buffer[2] == ASCII_F) AutoOnOff(buffer,string);		
//.25..........................................Adds new SYBT flavor.1----------------Looks for "FL1"..should be changed to be consistent with other data save code, but works OK
	else if (buffer[0] == ASCII_F && buffer[1] == ASCII_L && buffer[2] == ASCII_1) SYBTflavorOne(buffer,string);		
//.26..........................................Adds new SYBT flavor.2-------------------------------------Looks for "FL2"-----------
	else if (buffer[0] == ASCII_F && buffer[1] == ASCII_L && buffer[2] == ASCII_2)  SYBTflavorTwo(buffer,string);	
//.27..........................................Adds new SYBT flavor.3--------------------------------------Looks for "FL3"----------
	else if (buffer[0] == ASCII_F && buffer[1] == ASCII_L && buffer[2] == ASCII_3)  SYBTflavorThree(buffer,string);		
//-28--------------------------------------------Adds new SYBT flavor.4------------------------------------Looks for "FL4"----------
	else if (buffer[0] == ASCII_F && buffer[1] == ASCII_L && buffer[2] == ASCII_4)  SYBTflavorFour(buffer,string);
//-29-----------------------------------------------------Restore safety features--------------------------Looks for "SAF"----------
	else if (buffer[0] == ASCII_S && buffer[1] == ASCII_A && buffer[2] == ASCII_F) RestoreSafeMode();
//-30-----------------------------------------------------Override test safety features--------------------Looks for "OVR"----------
	else if (buffer[0] == ASCII_O && buffer[1] == ASCII_V && buffer[2] == ASCII_R) ExitSafeMode();	
//-31--------------------------------------Sleep Mode Time 1------------------------------------------------SLT---------------------
	else if (buffer[0] == ASCII_S && buffer[1] == ASCII_L && buffer[2] == ASCII_T) SleepTimeOne(buffer);	
//-32--------------------------------------Sleep Mode Time 2------------------------------------------------SLQ---------------------
	else if (buffer[0] == ASCII_S && buffer[1] == ASCII_L && buffer[2] == ASCII_Q) SleepTimeTwo(buffer);
//-33--------------------------------------Sleep Mode Time 3------------------------------------------------SLZ---------------------
	else if (buffer[0] == ASCII_S && buffer[1] == ASCII_L && buffer[2] == ASCII_Z) SleepTimeThree(buffer);												
//-34--------------------------------------Disable Sleep Mode-----------------------------------------------SLP---------------------
	else if (buffer[0] == ASCII_S && buffer[1] == ASCII_L && buffer[2] == ASCII_P) SleepDisable();
//-35--------------------------------------Restore Sleep Mode-----------------------------------------------SLX---------------------
	else if (buffer[0] == ASCII_S && buffer[1] == ASCII_L && buffer[2] == ASCII_X) SleepRestore();
//-36--------------------------------------enter Demo Mode--------------------------------------------------DMO---------------------
	else if (buffer[0] == ASCII_D && buffer[1] == ASCII_M && buffer[2] == ASCII_O) EnterDemoMode();		
//-37--------------------------------------exit Demo Mode---------------------------------------------------DMX---------------------
	else if (buffer[0] == ASCII_D && buffer[1] == ASCII_M && buffer[2] == ASCII_X) ExitDemoMode();	
//-38--------------------------------------send flavor data to app------------------------------------------FLV---------------------
	else if (buffer[0] == ASCII_F && buffer[1] == ASCII_L && buffer[2] == ASCII_V) GatherSYBTflavorData();	
//-39--------------------------------------send thermistor cal data to app----------------------------------TCY---------------------
	else if (buffer[0] == ASCII_T && buffer[1] == ASCII_C && buffer[2] == ASCII_Y) SendCalDataToApp();
//-40--------------------------------------send preferences to app------------------------------------------PRF---------------------
	else if (buffer[0] == ASCII_P && buffer[1] == ASCII_R && buffer[2] == ASCII_F) GatherPreferenceData();
//-41--------------------------------------Receive Beep command---------------------------------------------BEE---------------------
	else if (buffer[0] == ASCII_B && buffer[1] == ASCII_E && buffer[2] == ASCII_E) ReceiveBeepCommand();				
//-42--------------------------------------Set Bluetooth name-----------------------------------------------Looks for "BLU"---------
	else if (buffer[0] == ASCII_B && buffer[1] == ASCII_L && buffer[2] == ASCII_U) SetBluetoothName(buffer,string);
//-43--------------------------------------Read Bluetooth name.---------------------------------------------Looks for "OKs" Does nothing
	else if (buffer[0] == ASCII_O && buffer[1] == ASCII_K && buffer[2] == ASCII_s){}
//-44-------------------------------------------------------------------------------------------------------Looks for "MEY"---------	
	else if (buffer[0] == ASCII_M && buffer[1] == ASCII_E && buffer[2] == ASCII_Y){Take_Measurements=true;FLASH_0_write_eeprom_byte(1,57);}
//-45-------------------------------------------------------------------------------------------------------Looks for "NOM"----------------------
	else if (buffer[0] == ASCII_N && buffer[1] == ASCII_O && buffer[2] == ASCII_M){Take_Measurements=false;FLASH_0_write_eeprom_byte(1,83);}
//-46-------------------------------------------------------------------------------------------------------Looks for "OFF"-----------------------		
	else if (buffer[0] == ASCII_O && buffer[1] == ASCII_F && buffer[2] == ASCII_F){
			//	V_Regulators_set_level(false);  // turn off 3.3v regulators for logic and LEDs;
	}
//-47--------------------------------------Set SYBT ID tolerance--------------------------------------------Looks for "SIT"---------
	else if (buffer[0] == ASCII_S && buffer[1] == ASCII_I && buffer[2] == ASCII_T) SetSYBTidentTolerance(buffer);
//-48--------------------------------------Erase EPROM------------------------------------------------------Looks for "ERA"---------
	else if (buffer[0] == ASCII_E && buffer[1] == ASCII_R && buffer[2] == ASCII_A) EraseEEPROM();
//-49--------------------------------------Load Test Tolerance A--------------------------------------------Looks for "LDA"---------
	else if (buffer[0] == ASCII_L && buffer[1] == ASCII_D && buffer[2] == ASCII_A) SetLoadTestToleranceA(buffer);
//-50--------------------------------------Load Test Tolerance B--------------------------------------------Looks for "LDB"---------
	else if (buffer[0] == ASCII_L && buffer[1] == ASCII_D && buffer[2] == ASCII_B) SetLoadTestToleranceB(buffer);
//-51--------------------------------------Start Auto Cal---------------------------------------------------Looks for "CL1"---------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_L && buffer[2] == ASCII_1) BeginAutoCalibration(&cal,buffer);
//-52--------------------------------------Enter calibration value 66---------------------------------------Looks for "CL2"---------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_L && buffer[2] == ASCII_2) EnterCalValue66(&cal);
//-53--------------------------------------Enter calibration value 10---------------------------------------Looks for "CL3"---------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_L && buffer[2] == ASCII_3) EnterCalValue69(&cal);
//-54--------------------------------------Enter calibration value 20---------------------------------------Looks for "CL4"---------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_L && buffer[2] == ASCII_4) EnterCalValue20(&cal);
//-55--------------------------------------Run Auto Calibration---------------------------------------------Looks for "CL5"---------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_L && buffer[2] == ASCII_5) RunAutoCalibration(&cal);
//-56--------------------------------------Save Auto Calibration--------------------------------------------Looks for "CL6"---------			
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_L && buffer[2] == ASCII_6) SaveAutoCalibration(&cal);
//-57--------------------------------------Save Auto Calibration--------------------------------------------Looks for "CL7"---------
	else if (buffer[0] == ASCII_C && buffer[1] == ASCII_L && buffer[2] == ASCII_7) EnterCalValue69(&cal);
//-58--------------------------------------Echo 1--------------------------------------------Looks for "EC1"------------------------
	else if (buffer[0] == ASCII_E && buffer[1] == ASCII_C && buffer[2] == ASCII_1) EchoOne(buffer);
//-59--------------------------------------Echo 2--------------------------------------------Looks for "EC2"------------------------
	else if (buffer[0] == ASCII_E && buffer[1] == ASCII_C && buffer[2] == ASCII_2) EchoTwo(buffer);
//-60--------------------------------------echo 3--------------------------------------------Looks for "EC3"------------------------
	else if (buffer[0] == ASCII_E && buffer[1] == ASCII_C && buffer[2] == ASCII_3) EchoThree(buffer);
//-61--------------------------------------Echo 4--------------------------------------------Looks for "EC4"------------------------
	else if (buffer[0] == ASCII_E && buffer[1] == ASCII_C && buffer[2] == ASCII_4) EchoOne(buffer);
//-62--------------------------------------Echo 5--------------------------------------------Looks for "EC5"------------------------
	else if (buffer[0] == ASCII_E && buffer[1] == ASCII_C && buffer[2] == ASCII_5) EchoTwo(buffer);
//-63--------------------------------------echo 6--------------------------------------------Looks for "EC6"------------------------
	else if (buffer[0] == ASCII_E && buffer[1] == ASCII_C && buffer[2] == ASCII_6) EchoThree(buffer);
//-64--------------------------------------echo 6--------------------------------------------Looks for "OVB"------------------------
	else if (buffer[0] == ASCII_O && buffer[1] == ASCII_V && buffer[2] == ASCII_B) OverrideBluetoothLowBattery(buffer);     // Command to override Bluetooth low battery shutdown.

//#ifdef FifthGen
	//-65--------------------------------------LL On--------------------------------------------Turn On Light Load------------------------
		else if (buffer[0] == ASCII_L && buffer[1] == ASCII_L && buffer[2] == ASCII_Y) LightLoadOn(buffer);	
	//-66--------------------------------------LL Off-------------------------------------------Turn Off Light Load------------------------
		else if (buffer[0] == ASCII_L && buffer[1] == ASCII_L && buffer[2] == ASCII_N) LightLoadOff(buffer);
//#endif	

//-67--------------------------------------Ry On--------------------------------------------Turn On Main Load---------RYY---------------
	else if (buffer[0] == ASCII_R && buffer[1] == ASCII_Y && buffer[2] == ASCII_Y) Load_Relay_set_level(false);
//-68--------------------------------------Ry Off-------------------------------------------Turn Off Main Load--------RYN----------------
	else if (buffer[0] == ASCII_R && buffer[1] == ASCII_Y && buffer[2] == ASCII_N) Load_Relay_set_level(true);
//----------------------------------------------------------------------------------------------------------------------------------

//.69..........................................Sets HS thermistor calibration 1-----------------------Looks for "HS3"--------------
else if (buffer[0] == ASCII_H && buffer[1] == ASCII_S && buffer[2] == ASCII_3) HSThermistorCalOne(buffer);
//.70..........................................Sets HS thermistor calibration 2-----------------------Looks for "HS4"--------------
else if (buffer[0] == ASCII_H && buffer[1] == ASCII_S && buffer[2] == ASCII_4) HSThermistorCal2(buffer);
//.71..........................................Sets HS thermistor calibration 3------Looks for "HS5"..Currently none of the thermistor cal factors are used
else if (buffer[0] == ASCII_H && buffer[1] == ASCII_S && buffer[2] == ASCII_5) HSThermistorCal3(buffer);
//.72..........................................Sets HS thermistor offset...........Looks for "HS6"..should be changed to be consistent with other data saving formats.but works ok
else if (buffer[0] == ASCII_H && buffer[1] == ASCII_S && buffer[2] == ASCII_6) HSThermistorOffset(buffer,string);
//.73..........................................Sets HS thermistor resistor calibration A------------Looks for "HS1"-----------
else if (buffer[0] == ASCII_H && buffer[1] == ASCII_S && buffer[2] == ASCII_1) HSThermistorCalA(buffer);
//.74..........................................Sets HS thermistor resistor calibration B------------Looks for "HS2"-----------
else if (buffer[0] == ASCII_H && buffer[1] == ASCII_S && buffer[2] == ASCII_2) HSThermistorCalB(buffer);
//.75..........................................Sets resistor zero calibration ------------Looks for "CA5"-----------
else if (buffer[0] == ASCII_C && buffer[1] == ASCII_A && buffer[2] == ASCII_5) ResistorZeroCal(buffer);
//.76..........................................Sets LX voltage calibration ------------Looks for "LXB"-----------
else if (buffer[0] == ASCII_L && buffer[1] == ASCII_X && buffer[2] == ASCII_B) LXvoltageCal(buffer);
//.77..........................................Sets LX voltage calibration ------------Looks for "TRY"-----------
else if (buffer[0] == ASCII_T && buffer[1] == ASCII_R && buffer[2] == ASCII_Y) KeepTRelayOn = true;
//.78..........................................Sets LX voltage calibration ------------Looks for "TRN"-----------
else if (buffer[0] == ASCII_T && buffer[1] == ASCII_R && buffer[2] == ASCII_N) KeepTRelayOn = false;

	volts_noload.ones = ASCII_charactermap(buffer[3]);
	volts_noload.tens = ASCII_charactermap(buffer[2]);
	volts_noload.hundreds = ASCII_charactermap(buffer[1]);
						
	USART_0_enable_rx(); // THIS HAS TO BE A FEW CLOCK CYCLES AWAY FROM DISABLE RX			
return;
}	// end of Identify Commands							
//----------------------------------------------Below are all the functions called by Identify Commands-----------------------------
void SYBTvoltsADcal(char buffer[30]){
	char YesOrNo;		
	sscanf(buffer,"CA1%d %c",&cal.SYBT_ad_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_AD_Cal,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_SYBT_AD_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_SYBT_AD_STAT;
	}
}								
//------------------------------------------------------------------------------------------------------------------		
void LoadvoltsADcal(char buffer[30]){	
	char YesOrNo;
	sscanf(buffer,"CA2%d %c",&cal.Load_ad_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_LOAD_AD_Cal,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_LOAD_AD_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_LOAD_AD_STAT;
		}
}
//-----------------------------------------------------------------------------------------------------------------		
void NineVoltBatteryCal(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"CA3%d %c",&cal.Nine_V_ad_cal ,&YesOrNo);			
	if(YesOrNo== ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_9VOLT_AD_Cal,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_9VOLT_AD_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_9VOLT_AD_STAT;
		}
}
//-----------------------------------------------------------------------------------------------------------------
void SixVoltBatteryCal(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"CA4%d %c",&cal.Six_V_ad_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_6Volt_Battery_Cal,buffer+3,4);
		cal.Cal_Status2 = cal.Cal_Status2 | EEPROM_6VOLT_AD_STAT;
		cal.Cal_Status2 = cal.Cal_Status2 ^ EEPROM_6VOLT_AD_STAT;
		}
}
//-----------------------------------------------------------------------------------------------------------------
void OhmmeterCalA(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"RA1%d %c",&cal.ResistA_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_Resistor_A,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_SYBT_ResistA_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_SYBT_ResistA_STAT;
		}
}
//-----------------------------------------------------------------------------------------------------------------
void OhmmeterCalB(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"RA2%d %c",&cal.ResistB_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_Resistor_B,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_SYBT_ResistB_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_SYBT_ResistB_STAT;
		}
}
//-----------------------------------------------------------------------------------------------------------------		
void ThermistorCalA(char buffer[30]){	
	char YesOrNo;
	sscanf(buffer,"TR1%d %c",&cal.Therm_ResA_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_Therm_Res_A,buffer+3,4);
		cal.Cal_Status2 = cal.Cal_Status2 | EEPROM_SYBT_Therm_Res_A_STAT;
		cal.Cal_Status2 = cal.Cal_Status2 ^ EEPROM_SYBT_Therm_Res_A_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------		
void ThermistorCalB(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"TR2%d %c",&cal.Therm_ResB_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_Therm_Res_B,buffer+3,4);
		cal.Cal_Status2 = cal.Cal_Status2 | EEPROM_SYBT_Therm_Res_B_STAT;
		cal.Cal_Status2 = cal.Cal_Status2 ^ EEPROM_SYBT_Therm_Res_B_STAT;
	}
}
//------------------------------------------------------------------------------------------------------------------		
void ThermistorCalOne(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"TH1%lf %c",&cal.Thermist1_cal,&YesOrNo);	
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_Thermistor1,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;						
		}
}
//-----------------------------------------------------------------------------------------------------------------		
void ThermistorCal2(char buffer[30]){
	char YesOrNo;
	double Temp_E2;
	sscanf(buffer,"TH2%lf E%lf %c",&cal.Thermist2_cal,&Temp_E2,&YesOrNo);
//	Power = pow(1,Temp_E2);

			//SYBT_Temperature_Cal_2 = TempCal2;
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_Thermistor2,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------		
void ThermistorCal3(char buffer[30]){
	char YesOrNo;
	double Temp_E3;
	LED4_set_level(false);
	sscanf(buffer,"TH3%lf E%lf %c",&cal.Thermist3_cal,&Temp_E3,&YesOrNo);
//	Power = pow(1,Temp_E3);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_Thermistor3,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------		
void ThermistorOffset(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"TH4%d %c",&cal.SYBT_Therm_Offst,&YesOrNo);
	if(YesOrNo  == ASCII_Y){
		FLASH_0_write_eeprom_block(EEPROM_SYBT_Thermistor_Offset,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
 void IntThermistorCalA(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"TN1%d %c",&cal.Int_Therm_ResA_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_Int_Therm_Res_A,buffer+3,4);
		cal.Cal_Status2 = cal.Cal_Status2 | EEPROM_Int_Therm_Res_A_STAT;
		cal.Cal_Status2 = cal.Cal_Status2 ^ EEPROM_Int_Therm_Res_A_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void IntThermistorCalB(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"TN2%d %c",&cal.Int_Therm_ResB_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_Int_Therm_Res_B,buffer+3,4);
		cal.Cal_Status2 = cal.Cal_Status2 | EEPROM_Int_Therm_Res_B_STAT;
		cal.Cal_Status2 = cal.Cal_Status2 ^ EEPROM_Int_Therm_Res_B_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void IntThermistorCalOne(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"TH5%lf %c",&cal.Int_Thermist1_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_Int_Thermistor1,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void IntThermistorCal2(char buffer[30]){
	char YesOrNo;
	double Temp_E2;
	sscanf(buffer,"TH6%lf E%lf %c",&cal.Int_Thermist2_cal,&Temp_E2,&YesOrNo);
//	Power = pow(1,Temp_E2);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_Int_Thermistor2,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void IntThermistorCal3(char buffer[30]){
	char YesOrNo;
	double Temp_E3;
	sscanf(buffer,"TH7%lf E%lf %c",&cal.Int_Thermist3_cal,&Temp_E3,&YesOrNo);
//	Power = pow(1,Temp_E3);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_Int_Thermistor3,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------	
void IntThermistorOffset(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"TH8%d %c",&cal.Intrnl_Therm_Offst,&YesOrNo);
//	sprintf(string,"%f",cal.Int_Thermist_Offset);
	if(YesOrNo  == ASCII_Y){
		FLASH_0_write_eeprom_block(EEPROM_Int_Thermistor_Offset,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------	
//-----------------------------------------------------------------------------------------------------------------
void HSThermistorCalA(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"HS1%d %c",&cal.HS_Therm_ResA_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_HS_Therm_Res_A,buffer+3,4);
		cal.Cal_Status2 = cal.Cal_Status2 | EEPROM_HS_Therm_Res_A_STAT;
		cal.Cal_Status2 = cal.Cal_Status2 ^ EEPROM_HS_Therm_Res_A_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void HSThermistorCalB(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"HS2%d %c",&cal.HS_Therm_ResB_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_HS_Therm_Res_B,buffer+3,4);
		cal.Cal_Status2 = cal.Cal_Status2 | EEPROM_HS_Therm_Res_B_STAT;
		cal.Cal_Status2 = cal.Cal_Status2 ^ EEPROM_HS_Therm_Res_B_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void HSThermistorCalOne(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"HS3%lf %c",&cal.HS_Thermist1_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_HS_Thermistor1,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void HSThermistorCal2(char buffer[30]){
	char YesOrNo;
	double Temp_E2;
	sscanf(buffer,"HS4%lf E%lf %c",&cal.HS_Thermist2_cal,&Temp_E2,&YesOrNo);
	//	Power = pow(1,Temp_E2);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_HS_Thermistor2,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void HSThermistorCal3(char buffer[30]){
	char YesOrNo;
	double Temp_E3;
	sscanf(buffer,"HS5%lf E%lf %c",&cal.HS_Thermist3_cal,&Temp_E3,&YesOrNo);
	//	Power = pow(1,Temp_E3);
	if(YesOrNo == ASCII_Y ){
		FLASH_0_write_eeprom_block(EEPROM_HS_Thermistor3,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void HSThermistorOffset(char buffer[30],char string[30]){
	char YesOrNo;
	sscanf(buffer,"HS6%d %c",&cal.HS_Thermist_Offset,&YesOrNo);
//	sprintf(string,"%f",cal.HS_Thermist_Offset);
	if(YesOrNo  == ASCII_Y){
		FLASH_0_write_eeprom_block(EEPROM_HS_Thermistor_Offset,buffer+3,4);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------



void FarenheitOrCentigrade(char buffer[30],char string[30]){
	char YesOrNo;			
	sscanf(buffer,"FOC%4s %c",string,&YesOrNo);
	if(strncmp(string,"Fare",4)){
	cal.Preferences = cal.Preferences | 1;
	cal.Preferences = cal.Preferences ^ 1;
		}
		else if(strncmp(string,"Celc",4)){
		cal.Preferences = cal.Preferences | 1;	
			}
			if(YesOrNo == ASCII_Y ){			
				LED4_set_level(false);
				FLASH_0_write_eeprom_byte(EEPROM_PREFERENCES_ADDR,cal.Preferences);
				cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
				cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}		
}
//-----------------------------------------------------------------------------------------------------------------
void TemperatureOrResistance(char buffer[30],char string[30]){
	char YesOrNo;
	sscanf(buffer,"TOR%4s %c",string,&YesOrNo);
	if(strncmp(string,"esis",4)){
		cal.Preferences = cal.Preferences | 4;
		cal.Preferences = cal.Preferences ^ 4;
	}
	else if(strncmp(string,"temp",4)){
		cal.Preferences = cal.Preferences | 4;
	}
	if(YesOrNo == ASCII_Y ){
		LED4_set_level(false);
		FLASH_0_write_eeprom_byte(EEPROM_PREFERENCES_ADDR,cal.Preferences);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TypeOrResistance(char buffer[30],char string[30]){
	char YesOrNo;
	sscanf(buffer,"SOR%4s %c",string,&YesOrNo);
	if(strncmp(string,"Sybt",4)){
		cal.Preferences = cal.Preferences | 2;
		cal.Preferences = cal.Preferences ^ 2;
		}
		else if(strncmp(string,"Rist",4)){
			cal.Preferences = cal.Preferences | 2;
			}
	if(YesOrNo == ASCII_Y ){
		LED4_set_level(false);
		FLASH_0_write_eeprom_byte(EEPROM_PREFERENCES_ADDR,cal.Preferences);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void AutoOnOff(char buffer[30],char string[30]){
	char YesOrNo;
	sscanf(buffer,"AOF%4s %c",string,&YesOrNo);
	if(strncmp(string,"Aton",4)){
		cal.Preferences = cal.Preferences | 8;
		cal.Preferences = cal.Preferences ^ 8;
			}
			else if(strncmp(string,"Aoff",4)){
				cal.Preferences = cal.Preferences | 8;
			}
	if(YesOrNo == ASCII_Y ){
		LED4_set_level(false);
		FLASH_0_write_eeprom_byte(EEPROM_PREFERENCES_ADDR,cal.Preferences);
		cal.Cal_Status1 = cal.Cal_Status1 | EEPROM_C1C2C3_STAT;
		cal.Cal_Status1 = cal.Cal_Status1 ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void SYBTflavorOne(char buffer[30],char string[30]){
	char YesOrNo;	
	sscanf(buffer,"FL1%3s Nam%5lu Sv%c",cal.FlavrName1,&cal.FlavrVal1,&YesOrNo);
	sprintf(string,"%5lu",cal.FlavrVal1);  //converts resistor value to string for saving to EEPROM
	//sprintf(string2,"New%lu %3s %c",calibration.FlavrVal1,calibration.FlavrName1,YesOrNo);
	//USART0_sendString(string2);

	if(YesOrNo == ASCII_Y ){
		LED4_set_level(false);
		FLASH_0_write_eeprom_block(EEPROM_SYBT_FlavName1,buffer+3,4);
		FLASH_0_write_eeprom_block(EEPROM_SYBT_FlavResis1,string,5);
		//FLASH_0_write_eeprom_byte(EEPROM_PREFERENCES_ADDR,calibration.Preferences);
		//calibration.Cal_Status = calibration.Cal_Status | EEPROM_C1C2C3_STAT;
		//calibration.Cal_Status = calibration.Cal_Status ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void SYBTflavorTwo(char buffer[30],char string[30]){
	char YesOrNo;
	sscanf(buffer,"FL2%3s Nam%5lu Sv%c",cal.FlavrName2,&cal.FlavrVal2,&YesOrNo);
	sprintf(string,"%5lu",cal.FlavrVal2);
	//sprintf(string2,"New%lu %3s %c",calibration.FlavrVal2,calibration.FlavrName2,YesOrNo);
	//USART0_sendString(string2);

	if(YesOrNo == ASCII_Y ){
		LED4_set_level(false);
		FLASH_0_write_eeprom_block(EEPROM_SYBT_FlavName2,buffer+3,4);
		FLASH_0_write_eeprom_block(EEPROM_SYBT_FlavResis2,string,5);
		//FLASH_0_write_eeprom_byte(EEPROM_PREFERENCES_ADDR,calibration.Preferences);
		//calibration.Cal_Status = calibration.Cal_Status | EEPROM_C1C2C3_STAT;
		//calibration.Cal_Status = calibration.Cal_Status ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void SYBTflavorThree(char buffer[30],char string[30]){
	char YesOrNo;
	sscanf(buffer,"FL3%3s Nam%5lu Sv%c",cal.FlavrName3,&cal.FlavrVal3,&YesOrNo);
	sprintf(string,"%5lu",cal.FlavrVal3);
	//sprintf(string2,"New%lu %3s %c",calibration.FlavrVal3,calibration.FlavrName3,YesOrNo);
	//USART0_sendString(string2);

	if(YesOrNo == ASCII_Y ){
		LED4_set_level(false);
		FLASH_0_write_eeprom_block(EEPROM_SYBT_FlavName3,buffer+3,4);
		FLASH_0_write_eeprom_block(EEPROM_SYBT_FlavResis3,string,5);
		//FLASH_0_write_eeprom_byte(EEPROM_PREFERENCES_ADDR,calibration.Preferences);
		//calibration.Cal_Status = calibration.Cal_Status | EEPROM_C1C2C3_STAT;
		//calibration.Cal_Status = calibration.Cal_Status ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void SYBTflavorFour(char buffer[30],char string[30]){
	char YesOrNo;
	sscanf(buffer,"FL4%3s Nam%5lu Sv%c",cal.FlavrName4,&cal.FlavrVal4,&YesOrNo);
	sprintf(string,"%5lu",cal.FlavrVal4);
	if(YesOrNo == ASCII_Y ){
		LED4_set_level(false);
		FLASH_0_write_eeprom_block(EEPROM_SYBT_FlavName4,buffer+3,4);
		FLASH_0_write_eeprom_block(EEPROM_SYBT_FlavResis4,string,5);
		//FLASH_0_write_eeprom_byte(EEPROM_PREFERENCES_ADDR,calibration.Preferences);
		//calibration.Cal_Status = calibration.Cal_Status | EEPROM_C1C2C3_STAT;
		//calibration.Cal_Status = calibration.Cal_Status ^ EEPROM_C1C2C3_STAT;
	}
}
//-----------------------------------------------------------------------------------------------------------------	
void RestoreSafeMode(){
	OverrideSafeties = false;
	Beeper_set_level(true);
	_delay_ms(80);
	Beeper_set_level(false);
	LED4_set_level(true);
}
//-----------------------------------------------------------------------------------------------------------------
void ExitSafeMode(){
	OverrideSafeties = true;
	Beeper_set_level(true);
	_delay_ms(80);
	Beeper_set_level(false);
	_delay_ms(80);
	Beeper_set_level(true);
	_delay_ms(80);
	Beeper_set_level(false);
}
//-----------------------------------------------------------------------------------------------------------------	
void SleepTimeOne(char buffer[30]){		//Demo mode forces Sleep 1 to 80
	char YesOrNo;	
	sscanf(buffer,"SLT%2lu%c",&cal.Sleep_1,&YesOrNo);
	cal.Sleep_1 *= 10000;
	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_Sleep_1,buffer+3,2);
}
//-----------------------------------------------------------------------------------------------------------------
void SleepTimeTwo(char buffer[30]){		//Demo mode forces Sleep 2 to 80
	char YesOrNo;		
	sscanf(buffer,"SLQ%2lu%c",&cal.Sleep_2,&YesOrNo);
	cal.Sleep_2 *= 10000;	
	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_Sleep_2,buffer+3,2);

}
//---------------------------------------------------------------------------------------------------------------------
void SleepTimeThree(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"SLZ%2lu%c",&cal.Sleep_3,&YesOrNo);
	cal.Sleep_3 *= 10000;
	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_Sleep_3,buffer+3,2);
}
//-----------------------------------------------------------------------------------------------------------------
void SleepDisable(){
	NewSetUnfinishedBusiness(Unfinished_Sleep,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
}	
//-----------------------------------------------------------------------------------------------------------------
void SleepRestore(){
	NewClearUnfinishedBusiness(Unfinished_Sleep,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
}
//-----------------------------------------------------------------------------------------------------------------
void EnterDemoMode(){
	NewSetUnfinishedBusiness(Unfinished_Demo,&Unfinished_Business);
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	PWR_ON_set_level(true);  // turn on 6v battery
	#ifdef SmallChips
	FT2_set_level(true);	// turn on 3.3v regulators for LEDs on small chip board  on 5th Gen it is light load IGBT
	#endif
	#ifdef LargeChips
	LargeChip_Display_set_level(true);// turn on 3.3v regulators for LEDs on large chip board
	#endif
}
//-----------------------------------------------------------------------------------------------------------------
void ExitDemoMode(){
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
	NewClearUnfinishedBusiness(Unfinished_Demo,&Unfinished_Business);
	RestoreSleepSettings();				//copy saved Flash value back into volatile and replace temporary Demo value.		
}
//-----------------------------------------------------------------------------------------------------------------
//void SendFlavorsToApp(){
//	GatherSYBTflavorData();
//}
//-----------------------------------------------------------------------------------------------------------------
void SendCalDataToApp(){
	NewSetUnfinishedBusiness(Unfinished_Therm_Cal_TX,&Unfinished_Business);
}
//-----------------------------------------------------------------------------------------------------------------
//void SendPreferencesToApp(){
	//GatherPreferenceData();
	//NewSetUnfinishedBusiness(Unfinished_Preferences_TX,&Unfinished_Business);	
//}
//-----------------------------------------------------------------------------------------------------------------
void ReceiveBeepCommand(){
	NewSetUnfinishedBusiness(Unfinished_Beep,&Unfinished_Business);
}
//...........................................Sets internal thermistor calibration................................................
/*
		}else if (buffer[0] == ASCII_T && buffer[1] == ASCII_e && buffer[2] == ASCII_m){  //Looks for "Temp1"
			LED4_set_level(true);
			sscanf(buffer,"Temp1%8ld",&tempfloat);
			//tempfloat = 84668;
			sprintf(string,"%ld",tempfloat);
			USART1_sendString(string);
			sscanf(buffer+13,"%8ld",&tempfloat2);
			sprintf(string2,"%ld",tempfloat2);
			strcat(string,string2);
			USART0_sendString(string);
			stringlength = strlen(string);
			FLASH_0_write_eeprom_block(EEPROM_c1,buffer+5,20);// this works
			}
			*/
//---------------------------------------------------------------------------------------------------------------------		
void SetBluetoothName(char buffer[30],char string[30]){
	sscanf(buffer,"BLU%24s",Bluetooth_Name);
	sprintf(string,"%s",Bluetooth_Name);
	FLASH_0_write_eeprom_block(EEPROM_Bluetooth_Name,buffer+3,25);
	FLASH_0_write_eeprom_byte(EEPROM_New_Bluetooth_Name,ASCII_1);
}
//-------------------------------------------------------------------------------------------------------------
void RenameBluetooth(void){			//called from Main
	PWR_ON_set_level(true);
	uint8_t string[40];
	memset(string,0,40*sizeof(char));
	sprintf(string,"AT+NAME%s",Bluetooth_Name);
	wdt_reset();
	_delay_ms(1600);
	wdt_reset();
	_delay_ms(1600);
	wdt_reset();
	_delay_ms(1600);
	wdt_reset();
	USART0_sendString(string);  //only used to rename Bluetooth module.
	FLASH_0_write_eeprom_byte(EEPROM_New_Bluetooth_Name,ASCII_0);
	_delay_ms(1600);
	wdt_reset();
	_delay_ms(1600);
	wdt_reset();
	_delay_ms(1600);
	wdt_reset();
	USART0_getString();
//	New_Bluetooth_Name = 0;
}
//---------------------------------------------------------------------------------------------------------------------
void SetSYBTidentTolerance(char buffer[30]){
	char YesOrNo;
	sscanf(buffer,"SIT%2d %c",&cal.SYBT_Idnt_Tolrnc,&YesOrNo);
	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_SYBT_Ident_Tolerance,buffer+3,2);
}
//---------------------------------------------------------------------------------------------------------------------
void EraseEEPROM(void){
	uint8_t erase[10];
	memset(erase,255,10*sizeof(char));
	FLASH_0_write_eeprom_block(EEPROM_Sleep_1,(uint8_t*)erase,7);
	FLASH_0_write_eeprom_block(EEPROM_Sleep_3,(uint8_t*)erase,4);	
}
//----------------------------------------------------------------------------------------------------------------------
void SetLoadTestToleranceA(char buffer[30]){		//A adjusts to match SYBT voltage
	char YesOrNo;
	sscanf(buffer,"LDA%3d %c",&cal.LoadTestTolerance_A,&YesOrNo);
	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_LoadTestTolerance_A,buffer+3,3);	
}
//----------------------------------------------------------------------------------------------------------------------

void SetLoadTestToleranceB(char buffer[30]){		//B adjusts the acceptable range. 
	char YesOrNo;
	sscanf(buffer,"LDB%2d %c",&cal.LoadTestTolerance_B,&YesOrNo);
	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_LoadTestTolerance_B,buffer+3,2);	
}
//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------

void EchoOne(char buffer[30]){		//Was main mimic read OK? If so don't ask for repeat.
	char YesOrNo;
	sscanf(buffer,"EC1%c",&YesOrNo);
	//TransmitBatteryData(buffer,false);									
	if(YesOrNo == ASCII_Y )NewClearUnfinishedBusiness(Unfinished_BatDataRetransmit,&Unfinished_Business);
}
//----------------------------------------------------------------------------------------------------------------------

void EchoTwo(char buffer[30]){		//Was load mimic read OK? If so don't ask for repeat.
	char YesOrNo;
	sscanf(buffer,"EC2%c",&YesOrNo);
	if(YesOrNo == ASCII_Y )NewClearUnfinishedBusiness(Unfinished_AutoCalRetransmit,&Unfinished_Business);
}
//----------------------------------------------------------------------------------------------------------------------

void EchoThree(char buffer[30]){		//Was automatic cal read OK?
	char YesOrNo;
	sscanf(buffer,"EC3%c",&YesOrNo);
	//	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_LoadTestTolerance_B,buffer+3,2);
}
//----------------------------------------------------------------------------------------------------------------------

void EchoFour(char buffer[30]){		//Was manual cal read OK?
	char YesOrNo;
	sscanf(buffer,"EC4%c",&YesOrNo);
	//	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_LoadTestTolerance_B,buffer+3,2);
}
//----------------------------------------------------------------------------------------------------------------------

void EchoFive(char buffer[30]){		//Were preferences read OK?
	char YesOrNo;
	sscanf(buffer,"EC5%c",&YesOrNo);
	//	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_LoadTestTolerance_B,buffer+3,2);
}
//----------------------------------------------------------------------------------------------------------------------

void EchoSix(char buffer[30]){		//Was custom screen read OK?
	char YesOrNo;
	sscanf(buffer,"EC6%c",&YesOrNo);
	//	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_LoadTestTolerance_B,buffer+3,2);
}
//----------------------------------------------------------------------------------------------------------------------
void OverrideBluetoothLowBattery(char buffer[30]){       //Temporary override of low battery Bluetooth lockout.  Resets on power off.
	
}
//----------------------------------------------------------------------------------------------------------------------

void ResistorZeroCal(char buffer[30]){		//  Not used 
	; 
	//char YesOrNo;
	//sscanf(buffer,"CA5%d %c",&cal.Nine_V_ad_cal,&YesOrNo);
	//if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_LXbattery_AD_Cal,buffer+3,2);
}
//----------------------------------------------------------------------------------------------------------------------
void LXvoltageCal(char buffer[30]){	
	char YesOrNo;
	sscanf(buffer,"LXB%d %c",&cal.LXvoltage_cal,&YesOrNo);
	if(YesOrNo == ASCII_Y )FLASH_0_write_eeprom_block(EEPROM_LXbattery_AD_Cal,buffer+3,4);
}
//----------------------------------------------------------------------------------------------------------------------
//#ifdef FifthGen
	void LightLoadOn(char buffer[30]){		//Turn Light Load On
		FT2_set_level(true);			// I commented these out as a reminder that I'm currently not using the light load design at all
		LED4_set_level(false);
	}
	//----------------------------------------------------------------------------------------------------------------------

	void LightLoadOff(char buffer[30]){		//Turn Light Load Off
		FT2_set_level(false);			// I commented these out as a reminder that I'm currently not using the light load design at all
		LED4_set_level(true);
	}
//#endif
//----------------------------------------------------------------------------------------------------------------------