/*
 * bobs_functions.h
 *
 * Created: 7/29/2024 9:13:57 AM
 *  Author: Robert F
 */ 

#ifndef BOBS_FUNCTIONS_H_
#define BOBS_FUNCTIONS_H_
/*
* Originally also in driver_init.c, atmel_start_pins_h,  and  main.c
*/

#define SmallChips  
#define DemoMode								//	Use only to test boards connected to the programmer.
//#define LargeChips								//be sure to change firmware version to match the define. Add "s" for small chip, "L" for large  Located in "EepromFunctions.c"

//#define QualityControlTesting					//disables SYBT battery sensing code and LED timeout   Using this also disables the battery test function.  Pressing test button will display "dly"
//  REMINDER______________________________QualityControlTesting also disables TR relay when connected to a SYBTU battery,
//#define LowBatteryAlert						// enables the functions that control the Bluetooth module when the 6 volt battery is weak.
#define Measure6voltBattery						// enables the 6 volt battery sensing A-D converter and functions.

#ifdef LowBatteryAlert		
#define Measure6voltBattery						// if using the Low battery alert, you need the measure battery functions enabled
#endif

#define WithSixVoltBatteryTest
//#define WithOptionalExtras
//#define FifthGen   							//The experimental dual load version   Is OK to use the 4th gen program for 5th gen board. The light load IGBT will remain off.
//#define HeatSinkThermistorInstalled
#define LXbatteryCompatible   
//#define Relay2AlwaysOn
#define ATTINY

#include "ccp.h"
#include "port.h"
#include "seven_seg_led.h"
#include "import_preferences.h"
#include "SYBT5_Routines.h"
#include <compiler.h>
#include <avr/eeprom.h>
//#include <avr/io.h>

#define Cycle_1 500
#define Cycle_2 1000
#define Cycle_3 1500
#define Cycle_4 1400



#define RedTemperatureLED 8
#define RedResistanceLED 16
#define GreenTemperatureLED 32
#define GreenResistanceLED 64
#define RedGreenResistanceLED 80
#define GreenVoltageLED 128
#define AllGreenLEDs 224
#define RedTempAndResistLEDs 24

#define SYBT_Type_V66 66
#define SYBT_Type_C10 100
#define SYBT_Type_B20 200
#define SYBT_Type_E3S 256
#define SYBT_Type_C69 690
#define SYBT_Type_E85 850
#define SYBT_Type_Error 1000
#define SYBT_Type_Short 0
#define SYBT_DemoR 999
#define SYBT_DemoT 2999

#define Unfinished_SYBT_Flavor_TX 1  //unfinished business bits  0
#define Unfinished_Therm_Cal_TX 2						//1
#define Unfinished_Preferences_TX 4						//2
#define Unfinished_Beep 8								//3
#define Unfinished_Demo 16								//4
#define Unfinished_Sleep 32								//5
#define Unfinished_AutoCalResStart 64					//6
#define Unfinished_AutoCal66 128						//7
#define Unfinished_AutoCal10 256						//8
#define Unfinished_AutoCal20 512						//9
#define Unfinished_AutoCal69 1024						//10
#define Clear_AutoCalResistance 1920					//		Clears Auto Cal 66, 10, 20 and 69
#define Unfinished_AutoCalResRun 2048					//11
#define Unfinished_AutoCalTempStart 4096				//12
#define Unfinished_AutoCalTempRun 8192					//13
#define Unfinished_AutoCalRunning 16384					//14
#define Finished_AutoCAl 32704							//		Clears all Auto Cal flags
#define UnfSpare1 32768									//15
#define Unfinished_AutoPrefRetransmit 65536				//16
#define Unfinished_BatDataRetransmit 131072				//17
#define Unfinished_BatResultRetransmit 262144			//18
#define Unfinished_AutoCalRetransmit 524288				//19
#define Unfinished_ManualCalRetransmit 1048576			//20
#define Unfinished_BatResultRetrans3 2097152			//21
#define Unfinished_BatResultRetrans4 4194304			//22
#define Unfinished_BatResultRetrans5 8388608			//23
#define Unfinished_BatResultRetrans6 16777216			//24
#define Unfinished_BatResultRetrans7 33554432			//25
#define Unfinished_BatResultRetrans9 67108864			//26
#define Unfinished_BatResultRetransA 134217728			//27
#define Unfinished_BatResultRetransB 268435456			//28
#define Unfinished_BatResultRetransC 536870912			//29
#define Unfinished_BatResultRetransE 1073741824			//30
#define UNfSpare2 2147483648							//31

#ifndef ATTINY
	#define ADC_Internal_Temp 1
	#define ADC_Five_Volts 2
	#define ADC_SYBT_Resistor 4
	#define ADC_SYBT_Temperature 5
	#define ADC_Spare_Input 6
#endif

#ifndef ATTINY
	#ifdef LargeChips							//large chips before ATTINY
		#define ADC_LXvoltage 0					//large chips
		#define ADC_TwentyFourVolts 3
		#define ADC_SYBT_Volts 8				//large chips
		#define ADC_Load_Test_Volts 9			//large chips
		#define ADC_Six_Volts 11
		#define ADC_HS 18						//large chips
	#endif
#endif

#ifdef SmallChips
	#define ADC_SYBT_Volts 0
	#define ADC_Load_Test_Volts 3
#endif

#ifdef ATTINY								// I grouped everything for ATTINY to avoid confusion.
	#define ADC_LXvoltage 0					// PD0: Separate voltage divider for SYBT5 batteries. Voltage to be measured passes through TR relay
	#define ADC_Internal_Temp 1				// PD1:
	#define ADC_SYBT5_Or_E3S 2				// PD2: Combined SYBT5 and E3S sensor. Has voltage divider on main board and accessible through pin 1 LI_HS
	#define ADC_TwentyFourVolts 3			// PD3: I haven't used this yet. For monitoring 24 volt supply for problems. 
	#define ADC_SYBT_Resistor 4				// PD4:
	#define ADC_SYBT_Temperature 5			// PD5:
	#define ADC_Unused 6					// PD6: Has a pullup resistor. Not used.  Current scheme uses PD 2 to do both SYBT5 and E3S by looking at analog voltage.  Low for SYBT5 medium for E3S
	#define ADC_SYBT_Volts 8				// PE0: Direct from SYBT main Anderson connector as has always been.
	#define ADC_Load_Test_Volts 9			// PE1:
	#define ADC_Six_Volts 11				// PE3: can measure battery when load switch is on. 
	#define ADC_SYBT5_Sense 16				// PF0: looks for current on the SYBT5 communication circuit.
	#define ADC_HS 18						// PF2:
	
#endif

int ConvertToTemperature(int Temperature);

bool Take_Measurements;
long double c1;
long double c2;
long double c3;

// In standalone SYBT5 program I used defines instead of variables. 
//#define c1 2.399163584e-03   
//#define c2 2.684971175e-05
//#define c3 8.748793546e-07

struct volts_noloadLX{
	uint8_t hundreds;
	uint8_t tens;
	uint8_t ones;
}volts_noloadLX;

struct volts_wloadLX{
	uint8_t hundreds;
	uint8_t tens;
	uint8_t ones;
}volts_wloadLX;

struct volts_noload{
	uint8_t hundreds;
	uint8_t tens;
	uint8_t ones;
}volts_noload;

struct volts_wload{
	uint8_t hundreds;
	uint8_t tens;
	uint8_t ones;
	uint8_t decimal;
}volts_wload;

struct resistance{
	uint8_t hundreds;
	uint8_t tens;
	uint8_t ones;
	uint8_t decimal;
}resistance;

struct temperature{
	uint8_t hundreds;
	uint8_t tens;
	uint8_t ones;
	uint8_t decimal;	
}temperature;

struct SYBT{
	int Volts_noload1;
	int Volts_noload2;
	int Volts_wload1;
	int Volts_wload2;
	int Voltage_Drop;
	//int Voltage_DropLocal;
	int Temperature;
	int Stored_Temperature;
	float InternalTemperature;
	int HeatSinkTemp;
	int Resistance;
	int Nine_Volt_Battery;
	int LXvoltage;
	
}SYBT;
struct SYBT array_SYBT[100];
uint8_t TypeOfBatteryConnected;

uint16_t twentyfourvoltsupply;

int R_Zero_VoltageDrop;
bool IsE3Sbattery;
bool KeepTRelayOn;
int SixVoltBattery;

int TurnEverythingOn(void);
void USART0_sendChar(char c);
bool USART0_sendString(char *str);
void USART1_sendChar(char c);
void USART1_sendString(char *str);
void identify_commands(uint8_t buffer[]);
void old_identify_commands(uint8_t buffer[]);
void USART0_getString(void);
//void USART1_getString(void);
bool oldIdentify_Battery_Type(uint32_t Resistance);
bool Identify_Battery_Type(uint32_t Resistance,uint8_t *Rows);
bool Test_Battery(int batterystatus, uint32_t *DisplayVoltageDropTimer, int *Test_Status,int *SYBT_adc_cal,int *LOAD_adc_cal);
bool Send_Data_UART(void);
bool Gather_Battery_Data(int SixVoltBattery,int FiveVoltBus,int ResistorZeroDrop,bool SYBTgood,uint32_t SleepTimer,uint32_t BluetoothSleepTimer);
bool Old_Gather_Battery_Data(int SixVoltBattery,int FiveVoltBus,bool SYBTgood,uint32_t SleepTimer,uint32_t BluetoothSleepTimer);

int Store_Battery_Data(int SYBTcount,int Volts,int Temperature,uint32_t Resistance);
int Check_SYBT_Status(bool batterypresent,int batterystatus, int SYBTcount,int Volts,int Temperature,uint32_t Resistance);
void UnfinishedTXbusiness(void);
void NewSetUnfinishedBusiness(uint32_t Business, uint32_t *UnfinishedBusiness);
void NewClearUnfinishedBusiness(uint32_t Business, uint32_t *UnfinishedBusiness);
void SetDataTX_Delay(uint16_t* send_data_delay);
int ResetTimer(int16_t number_of_loops);
int MeasureSixVoltBattery(bool *BatteryOK,int NineV_adc_cal,bool SYBTisProvidingPower);
void GatherSYBTflavorData(void);
void GatherPreferenceData(void);

void SendDataToLEDdrivers(int *LEDcolumn,bool DimDisplay);
int MeasureSYBTresistance(struct resistance *);
uint16_t MeasureSYBTtemperature(bool BatteryTestOK,uint32_t *DisplayVoltageDropTimer,int Test_Status,struct temperature *);
void MeasureInternalTemperature(struct SYBT*,struct cal*);
void MeasureHeatSinkTemperature(struct SYBT*,struct cal*);
void SwitchToDemoMode(uint8_t *Rows);
void SwitchToDemoModePtr(uint8_t *Rows, struct volts_noload*, struct temperature*, struct resistance*,struct cal*);
void RenameBluetooth(void);
bool MeasureSYBTvolts(bool *SYBTgood, struct volts_noload*);
void SwitchToAutoCal(uint8_t *Rows);
bool MeasureLXvoltage( struct volts_noload*);
bool OvercurrentShutdown(int ResistorZeroValue);
uint16_t Measure24voltSupply(void);
bool IsSYBT5orE3SadapterConnected(void);

#endif /* BOBS_FUNCTIONS_H_ *//* BOBS_FUNCTIONS_H_ */