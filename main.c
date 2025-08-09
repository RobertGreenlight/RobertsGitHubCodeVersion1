
/*
updated 5/17/2023   All #defines must be in bobs_functions.h file

#define SmallChips						must not use on 5th gen boards. 
#define LargeChips						be sure to change firmware version to match the define. Add "s" for small chip, "L" for large  Located in "BluetoothDataOut.c"
#define QualityControlTesting			disables SYBT battery sensing code and LED timeout
#define LowBatteryAlert					enables the functions that control the Bluetooth module when the 6 volt battery is weak. 
#define Measure6voltBattery				enables the 6 volt battery sensing A-D converter and functions.
										also originally in bobs_functions.h    atmel_start_pins.h   and driver_init.c
										change firmware version "L" for large and "s" for small in BluetoothDataOut.c
#define DemoMode						Use only to test boards connected to the programmer.
#define FifthGen						The experimental dual load version   Is OK to use the 4th gen program for 5th gen board. The light load IGBT will remain off.
#define HeatSinkThermistorInstalled
#define LXbatteryCompatible				Uses the port originally created for the 2 level design to control relay 2
										remember if making an entirely new project, the AVR must be configured to handle floating point operations.
										
										I fixed the watchdog. Apparently lost when I copied everything into a new project.  WDT is now set in the wdt.c file
										Update:  I'm back to setting fuses for the watchdog. Not sure why the wdt.c file isn't working.
										Long watchdog time is essential for when renaming Bluetooth modules. 
										 
										I added a delay to the Bluetooth functions to allow time for the Bluetooth modules to boot.  This was causing a freeze in some models.  
*/
//watchdog added 12 29 2021				Remember that the fuses need to be set for each new chip to be programmed. 

// Change firmware to reflect "s" for small chip and "L" for large chips. Located in Bluetooth data out.

//set baud rate in USART basic.c   Use either 19200 or 38400.  9600 works but slow.   Pre-program and label Bluetooth module to match.  
//Define ATTINY for boards with ATTINY

// ---------------------------------------Don't forget to set watchdog fuses when programming new chips-------Using 8kclk setting-------------------------------------------.


#include <atmel_start.h>
#include "bobs_functions.h"
#include "import_preferences.h"
#include "eeprom_functions.h"
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <nvmctrl_basic_example.h> 
#include <driver_init.h>
#include <compiler.h>
#include <wdt.h>

#define DisplayVoltageDropTime 15

uint16_t send_data_delay;
uint16_t number_of_loops;
uint16_t startup_delay;
//int SYBT_adc_cal = 1497;
//int NineV_adc_cal = 650;
//int SixV_adc_cal = 650;


int BluetoothDelay;

long double ThermistorA_adc_cal = 1096;
long double ThermistorB_adc_cal = 1027;
long double ThermistorC_adc_cal = 1027;
uint32_t Unfinished_Business;
uint8_t Business;
uint32_t SleepTimer;
uint32_t SleepTimeRemaining;
uint8_t DemoResistance = 1;
uint32_t ATTINYTimer;

bool SYBTisProvidingPower = false;
bool SYBTconnectedButNoBattery = false;
bool SYBTalreadyConnectedButNoBattery = false;
bool DisplayVoltageDropTimerExpired = false;
int New_Bluetooth_Name;
bool IsLXbattery = true;

//int GlobalTemperature;
//float tempTemperature;

struct cal;	// this and extern struct cal are here to satisfy compiler complaints about the struct pointers

int main(void)
{	
	bool FirstRun = false;
	bool MainButtonDown;
	bool SYBTgood;
	uint32_t DisplayVoltageDropTimer;
	int Test_Status;
	uint16_t DelayAfterSYBTisConnected;
	

#ifdef Measure6voltBattery
	bool SixVBatteryOK;
#endif	
	bool BatteryTestOK;
	bool DimDisplay;
	bool DisableBluetooth;
	//int MainButtonCount;						// Not using this and I forget the original idea behind it
	int FiveVoltBus;
	int LXvoltageValue;
	int LEDcolumn;
	//uint32_t Resistance = 0;
	uint32_t BluetoothSleepTimer;
	uint32_t InternalBatteryOffDelay;	
	cal.ResistA_cal = 1096;
	cal.ResistB_cal= 1027;
	cal.SYBT_ad_cal = 1497;
	cal.Load_ad_cal = 1390;	
	Rows = 1;	
	SYBTcount = 1;
	DisplayVoltageDropTimer = 10000;
	
	c1 = 2.399163584e-03;
	c2 = 0.2684971175e-04;
	c3 = 8.748793546e-07;
	//uint8_t string[40];

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	wdt_reset();
	
	LED4_set_level(true); //start with LED4 off
	Load_IGBT_set_level(true); //start with IGBT off
	#ifdef LXbatteryCompatible
		#ifndef Relay2AlwaysOn
		FT2_set_level(false);				//start with relay 2 off		
		BlueLED_set_level(true);			//not really needed here, just a reminder that the two are normally used together.  
		#endif	
	#endif		
	//RESET_set_level(true); //start with light load IGBT off
	if(CheckEEPROMvalues()){
	}
		
if(New_Bluetooth_Name)RenameBluetooth();			//if new name flag is set, run the renaming function.

//	PWR_ON_set_level(true);
		
#ifndef LowBatteryAlert  // if there is no low battery alert, then set display to full brightness. 
	DimDisplay = true;
#endif	

if(startup_delay < 10000){		//This gives the Bluetooth module time to wake up.
	startup_delay += 1;
	wdt_reset();
	USART0.RXDATAL;  
	}
//	else{
//			
//		}
	
FirstRun = true;  // not needed anymore
TurnEverythingOn();					// replaces several lines of code for readability  seeing if this restarts after a watchdog event

	
while (1) {	
	SleepTimer += 1;
	BluetoothSleepTimer += 1;	
	number_of_loops += 1;
	ATTINYTimer ++;
	if(ATTINYTimer > 2147483647)ATTINYTimer = 0;


#ifdef Relay2AlwaysOn
	Load_IGBT_set_level(false);
	FT2_set_level(true);				//Only used for testing the code.  This relay can never be left on indefinitely. It will either drain the AA batteries or overheat the dropping resistor R1.
#endif	

#ifdef QualityControlTesting
	LargeChip_Display_set_level(true);  //only for testing. Leaves display on permanently
	PWR_ON_set_level(true);
	LED1_set_level(false);
	LED4_set_level(false);
	BlueLED_set_level(true);   
#endif	
	
	if (Unfinished_Business & Unfinished_Beep){
		Beeper_set_level(true);
		_delay_ms(100);
		Beeper_set_level(false);
		Unfinished_Business ^= Unfinished_Beep;
	}
		
	if((8191)==(SleepTimer & 8191)){				//It appears that LX battery prevents 8191 from ever happening
		//USART0_sendString("8191\n");  Put here to test when 8191 condition is met. 
		if((!DisableBluetooth) && FirstRun){
			Gather_Battery_Data(SixVoltBattery,FiveVoltBus,LXvoltageValue,SYBTgood,SleepTimer,BluetoothSleepTimer);		
		}
		//Gather_Battery_Data(SixVoltBattery,FiveVoltBus,SYBTgood);
		EEpromData1 = FLASH_0_read_eeprom_byte(1);
	}
	else if((4095)==(SleepTimer & 4095)){   //was 8191
		
		Load_IGBT_set_level(true);		//ensure IGBT is off
		Load_Relay_set_level(false);	//ensure relay is off
		
		#ifdef Measure6voltBattery
			SixVoltBattery = MeasureSixVoltBattery(&SixVBatteryOK,cal.Nine_V_ad_cal,SYBTisProvidingPower);
		#endif		

		#ifndef Measure6voltBattery
			SixVoltBattery = 888;			// this is used to tell the app that there is no 6v battery measurement capability.
		#endif
		
		
		
		if(FirstRun == true)UnfinishedTXbusiness();  //handles timing of Bluetooth transmissions
		MeasureInternalTemperature(&SYBT,&cal);
		#ifdef HeatSinkThermistorInstalled
		MeasureHeatSinkTemperature(&SYBT,&cal);
		#endif		
	}
	else if((2047)==(SleepTimer & 2047)){   //
		
	if(FirstRun == true)UnfinishedTXbusiness();  //handles timing of Bluetooth transmissions
	
	}
	
	#ifdef LowBatteryAlert
		if(!SixVBatteryOK){
			if(SYBTisProvidingPower && (3 ==(SleepTimer & 3)))DimDisplay = true;  //dims display while on SYBT power
			else DimDisplay = false;											//turns off temp and resistance when on 6 volt power
		}
		else DimDisplay = true;
	#endif	
	
	if(!Main_Button_get_level()){	
		if (MainButtonDown){
			MainButtonDown = false;
			}
		//Load_Relay_set_level(false);
		//if(MainButtonCount > 1) MainButtonCount = 0;							// Not using this and I forget the original idea behind it
		}		
	//if(MainButtonDown){;}
	//else {;}
//---------------------------------------------------------------------------------------------------------			
//	if(number_of_loops == Cycle_2){
	if((number_of_loops & 0x1FF)==0x1FF ){
		if(MeasureLXvoltage(&volts_noloadLX)){
			IsLXbattery = true;
		}
		else{
			IsLXbattery = false;
			MeasureSYBTvolts(&SYBTgood,&volts_noload);	
		}
	//	if(MeasureSYBTvolts(&SYBTgood,&volts_noload)){
	//					IsLXbattery = false;
	//	}			
	}
//--------------------------------------------Get temperature. Either SYBT or internal temp----------------
	if(number_of_loops == Cycle_3)MeasureSYBTtemperature(BatteryTestOK,&DisplayVoltageDropTimer,Test_Status,&temperature);   // DisplayVoltageDropTimer =  I changed this
	
//--------------------------------------------------------------------------------------------------										
	//FiveVoltBus = (((float)cal.Six_V_ad_cal/1000)*(ADC_0_get_conversion(ADC_Unused) >> 2));
		twentyfourvoltsupply = Measure24voltSupply();
		//twentyfourvoltsupply = 39;
//-------------------------------------------------------------------------------------------------Following functions are only when LX battery version is required
#ifdef LXbatteryCompatible  	
	SYBT.LXvoltage = (((float)cal.LXvoltage_cal/1000)*(ADC_0_get_conversion(ADC_LXvoltage) >> 2));
//--------------------------------------------------------------------------------------------------
	if(SleepTimer > 3000){ 
		if(!(Unfinished_Business & Finished_AutoCAl)){
			if(!KeepTRelayOn){
				#ifndef Relay2AlwaysOn
				FT2_set_level(false);				//This functions opens Relay 2 after measurement of SYBT temperature and resistance.   TEMPORARY DISABLE
				BlueLED_set_level(true);			//Relay2 needed to be closed during the calibration routine.
				#endif
				
				 
			}
			
		}   				
	}
//--------------------------------------------------------------------------------------------------
#endif
#ifdef LowBatteryAlert
if(((FiveVoltBus > 500) && (!DisableBluetooth)) || SYBTisProvidingPower){				//is there sufficient power for Bluetooth?		Allow Bluetooth to run whenever connected to SYBT								
#endif
		
//if(FirstRun == true){
	if (USART_0_is_rx_ready()){					//listens to Bluetooth module 
//		USART_0_enable_rx();
//		if(FirstRun == true){
			USART0_getString();
//		}				
	//	else if(FirstRun == true){	 
	//		USART0.RXDATAL;   // this clears the USART buffer after startup.
	//		}		
	}
//}

#ifdef LowBatteryAlert	
}
//else
//	if((SleepTimer & 1024)== 1024)LED0_set_level(true);
	
#endif
//----------------------------------------------------------------------------------
if ((number_of_loops == Cycle_1) && !(Unfinished_Business & Unfinished_AutoCalResStart))MeasureSYBTresistance(&resistance); // Don't measure if auto cal is running
//----------------------------------------------------------------------------------
//if(SleepTimer > calibration.Sleep_3 * 100){			// Puts display to sleep  It appears I haven't used this in a long time. 
//	#ifdef SmallChips	
//		LightLoadIGBT_set_level(false);  
//	#endif
//	#ifdef LargeChips	
//		LargeChip_Display_set_level(false);
//	#endif
//}
if((SleepTimer > (cal.Sleep_2)) && !SYBTisProvidingPower){									// Put display to sleep ---- SYBT is not providing power
//if((SleepTimer > 300000) && !SYBTisProvidingPower){									// Put display to sleep ---- SYBT is not providing power
	SleepTimer = 0;
	#ifdef SmallChips
		FT2_set_level(false);    // is voltage regulator control on small chip board
	#endif
	#ifdef LargeChips
		LargeChip_Display_set_level(false);
	#endif
	#ifdef LXbatteryCompatible
		#ifndef Relay2AlwaysOn
		FT2_set_level(false);						// is relay 2 control on large chip board
		BlueLED_set_level(true); 
		#endif
	#endif
}

if((SleepTimer > (cal.Sleep_1)) && (SYBTisProvidingPower || !IsSYBT5orE3SadapterConnected())){									// Put display to sleep ----SYBT is providing power   Sybt5 as analog input
//if((SleepTimer > (cal.Sleep_1)) && (SYBTisProvidingPower || !SYBT5_Connected_get_level())){								// Put display to sleep ----SYBT is providing power  Sybt5 as digital input
//if((SleepTimer > 200000) && SYBTisProvidingPower){									// Put display to sleep ----SYBT is providing power
	if(!(Unfinished_Business & Unfinished_Sleep)){										// keep display on indefinitely
		SleepTimer = 0;
		#ifdef SmallChips
		FT2_set_level(false);						// is voltage regulator control on small chip board
		#endif
		#ifdef LargeChips
		LargeChip_Display_set_level(false);
		#endif	
		#ifdef LXbatteryCompatible
			#ifndef Relay2AlwaysOn
			FT2_set_level(false);						// is relay 2 control on large chip board
			BlueLED_set_level(true);
			#endif
		#endif
	}	
}
if(SleepTimer > 800000)  SleepTimer = 0;			

//if((BluetoothSleepTimer > 600000) && !SYBTisProvidingPower)PWR_ON_set_level(false);  // Put everything to sleep if running on internal battery
if((BluetoothSleepTimer > cal.Sleep_3) && !SYBTisProvidingPower)PWR_ON_set_level(false);  // Put everything to sleep if running on internal battery

//if((SleepTimer > calibration.Sleep_1) && SYBTisProvidingPower){
//if((SleepTimer > 600000) && SYBTisProvidingPower){
//	SleepTimer = 0;
	//Gather_Battery_Data();
//	if(Take_Measurements)Test_Battery(1,&DisplayVoltageDropTimer,&Test_Status);
//}

//if((SleepTimer > calibration.Sleep_2) && SYBTisProvidingPower){
//if((SleepTimer > 300000) && SYBTisProvidingPower){
//	SleepTimer = 0;	
	//Gather_Battery_Data();
//	if(Take_Measurements)Test_Battery(1,&DisplayVoltageDropTimer,&Test_Status,&calibration.SYBT_ad_cal,&calibration.Load_ad_cal);
//}

if(number_of_loops > 20000){
	number_of_loops = 0;
}

if(Main_Button_get_level()){							// Main button turns on logic power, if not already on. It does this by way of a temporary hardware power connection, and then the 
	//SleepTimer = 0;
	BluetoothSleepTimer = 0;
	LargeChip_Display_set_level(true);					// turn on 3.3v regulators for LEDs on large chip board	
	PWR_ON_set_level(true);						// connects 6v battery to voltage divider for measurement also used to control switch on load test board
	LED1_set_level(false);
}
else{
	LED1_set_level(true);
}

if (Main_Button_get_level() && !MainButtonDown) {		// Main button turns on logic power, if not already on.
														// button has both hardware direct power connection, and software input.
	//if (!MainButtonDown){								// appears to be redundant
		//Load_IGBT_set_level(false);}
		MainButtonDown = true;
		// I don't like this below
		//if((ADC_0_get_conversion(ADC_LXvoltage) < 200) || (ADC_0_get_conversion(ADC_SYBT_Volts) < 1000) || !FT2_get_level()){   //If there is no voltage present on the LX input, then close relay-----------------------------------------------------------------------------------------------------------
		//	;
		//}
		if(SYBTgood && (DelayAfterSYBTisConnected == 2000) && (DisplayVoltageDropTimer > DisplayVoltageDropTime)){
			SleepTimer = 0;
			BatteryTestOK = Test_Battery(1,&DisplayVoltageDropTimer,&Test_Status,&cal.SYBT_ad_cal,&cal.Load_ad_cal);
		}
		else
		{
					//
		//} //if(!SYBTconnectedButNoBattery)   // this isn't working  ---------------------------------------------------------
		//{
			BatteryTestOK = false;
			DisplayVoltageDropTimer = 16;
			DisplayVoltageDropTimerExpired = false;
			//if(DisplayVoltageDropTimer <= DisplayVoltageDropTime)
			 Test_Status = 100;  // this is going to be less than time, every time. 
			//else Test_Status = 200;
		}
		//SleepTimer = 0;
		TurnEverythingOn();					// replaces several lines of code for readability 
		//MainButtonCount = MainButtonCount + 1;					// Not using this and I forget the original idea behind it
	//}
}
//-----------------------------Test for disconnect from SYBT----------------------------------------------------------------------------
//																			raw data from AD converter should be about 2457
if((ADC_0_get_conversion(ADC_SYBT_Volts) < 2200) && SYBTisProvidingPower){ //no need for accuracy, just see battery voltage is at a low level.  I didn't use zero so it would have faster response.
	PWR_ON_set_level(true);				//	Turn on 6v battery	
	LargeChip_Display_set_level(true);
	
	#ifdef LXbatteryCompatible
		#ifndef Relay2AlwaysOn
		FT2_set_level(false);						// is relay 2 control on large chip board
		BlueLED_set_level(true);
		#endif
	#endif

	DisableBluetooth = true;
	//_delay_ms(200);
	SleepTimer = 0;
	BluetoothSleepTimer = 0;
	SYBTisProvidingPower = false;
	LED1_set_level(true);
	DelayAfterSYBTisConnected = 0;
	array_SYBT[1].Temperature = 3000;			//open circuit value
}
//-----------------------------Test for connected to SYBT----------------------------------------------------------------------------
#ifndef QualityControlTesting
	if((ADC_0_get_conversion(ADC_SYBT_Volts) > 2200)){				// no need for accuracy, just see if AD converter is reading some battery voltage. 
		if(!SYBTisProvidingPower){
			SleepTimer = 0;
			BluetoothSleepTimer = 0;
			if(!SYBTisProvidingPower)DisableBluetooth = true;
			LargeChip_Display_set_level(true);
			USART0_sendString("PXconnected\n\r");
			//_delay_ms(200);
	//		PWR_ON_set_level(false);						// turn off 6v battery	
			Load_Relay_set_level(false);							//ensure relay is off
			SYBT5_RLY_set_level(false);								//ensure relay is off
			#ifdef LXbatteryCompatible
				FT2_set_level(true);								// Close RY2 so temp and resistance can be read.##############################################disabled for troubleshooting 
				BlueLED_set_level(false);							// Turn on Blue LED 
			#endif		
			}
		SYBTisProvidingPower = true;
		LED1_set_level(false);										// Turn on LED 1
		if(DelayAfterSYBTisConnected < 2000){						// Brief delay to allow time for a reliable connection
			DelayAfterSYBTisConnected++;
		}
		else{
			//DelayAfterSYBTisConnected = 2000;						//not needed but here as a reminder that the counter is supposed to stop at 2000
		//	PWR_ON_set_level(false);						// turn off 6v battery	I moved here to provide overlap
		}
	}
	//-------------------------Test for connected to SYBT5------------------------------------------------------------------------------------------------------------
	if(IsConnectedToSYBT5()){
		MeasureLXvoltage(&volts_noloadLX);
		FastSYBT5query();
		SleepTimer = SleepTimer + 10000;			//This has the effect of skipping past 8191, where SYBT for LX readings are made.
	}

#endif
//-----------------------------Test for connected to SYBT with dead battery----------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------


#ifdef DemoMode
	SwitchToDemoModePtr(&Rows,&volts_noload,&temperature,&resistance,&cal);
	BlueLED_set_level(false);
#endif

#ifndef DemoMode
	if (Unfinished_Business & Unfinished_Demo)SwitchToDemoModePtr(&Rows,&volts_noload,&temperature,&resistance,&cal);
	else if (Unfinished_Business & Unfinished_AutoCalRunning)SwitchToAutoCal(&Rows);
#endif



//-------------------------------------SPI bus signals to LED drivers are sent one column at a time, with the next column sent on the next program loop.----------------------------------
SendDataToLEDdrivers(&LEDcolumn,DimDisplay);
//--------------------------------------------------------------------------------------------------------------
	if(DisableBluetooth){
		BluetoothDelay ++;
		if(BluetoothDelay > 10000){
			DisableBluetooth = false;
			BluetoothDelay = 0;
			}
		}	
	if(ATTINY_PB0_get_level()){						//checks for USART received OK signal from ATTINY
	//	BlueLED_set_level(true);
	YellowLED_set_level(false);
		}
	else{
	//	BlueLED_set_level(false);
	YellowLED_set_level(true);
	}			
	wdt_reset();									//watchdog time is set with fuses in programmer.
	}												// end of while loop
}													
//--------------------------------------------------------------------------------------------------------------------------end of main----------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SendDataToLEDdrivers(int *LEDcolumn,bool DimDisplay){
	#ifndef ATTINY
		switch(*LEDcolumn){
			case 0:
				THREE_DIGIT_SPI(resistance.ones,temperature.ones,volts_noload.ones,4,DimDisplay);					//transmit first digits.
				*LEDcolumn = 1;
				break;
			case 1:
				THREE_DIGIT_SPI(resistance.tens,temperature.tens,volts_noload.tens,2,DimDisplay);					//transmit second digits.
				*LEDcolumn = 2;
				break;
			case 2:
				THREE_DIGIT_SPI(resistance.hundreds,temperature.hundreds,volts_noload.hundreds,Rows,DimDisplay);	//transmit third digits..
				*LEDcolumn = 0;
				break;
			default:
				*LEDcolumn = 0;
				break;
		}
	#endif	
	#ifdef ATTINY
	if(ATTINYTimer > 2000 ){   //)
		char temporaryString[30];
		memset(temporaryString,0,30*sizeof(char));
		
	/*	switch(*LEDcolumn){
			case 0:
				sprintf(temporaryString,"TMP%c%c%c\n",temperature.hundreds,temperature.tens,temperature.ones);
				*LEDcolumn = 1;
			break;
			case 1:
				sprintf(temporaryString,"RES%c%c%c\n",resistance.hundreds,resistance.tens,resistance.ones);
				*LEDcolumn = 2;
				break;
			case 2:	
				sprintf(temporaryString,"VOL%c%c%c\n",volts_noload.hundreds,volts_noload.tens,volts_noload.ones);
				*LEDcolumn = 0;
				break;
			default:
				*LEDcolumn = 0;
				break;
		}
		*/
//sprintf(temporaryString,"VOL123\nTMP456\nRES789\n");
sprintf(temporaryString,"VOL%c%c%c\nTMP%c%c%c\nRES%c%c%c\n",volts_noload.hundreds,volts_noload.tens,volts_noload.ones,temperature.hundreds,temperature.tens,temperature.ones,resistance.hundreds,resistance.tens,resistance.ones);
//sprintf(temporaryString,"VOL%c%c%c\nTMP%c%c%c\nRES%c%c%c\n",volts_noload.hundreds,volts_noload.tens,volts_noload.ones,resistance.hundreds,resistance.tens,resistance.ones,resistance.hundreds,resistance.tens,resistance.ones);			
//sprintf(temporaryString,"VOL%c%c%c\nTMP%c%c%c\nRES%c%c%c\n",volts_noload.hundreds,volts_noload.tens,volts_noload.ones,temperature.hundreds,temperature.tens,temperature.ones,temperature.hundreds,temperature.tens,temperature.ones);		

  //USART0_sendString(temporaryString);							//This is not needed
	memset(temporaryString,0,30*sizeof(char));
	sprintf(temporaryString,"VOL%c%c%c\n",volts_noload.hundreds,volts_noload.tens,volts_noload.ones);
	USART3_sendString(temporaryString);
	memset(temporaryString,0,30*sizeof(char));
	//temperature.tens = temperature.tens - AddDecimalToASCII;
	sprintf(temporaryString,"TMP%c%c%c\n",temperature.hundreds,temperature.tens,temperature.ones);	
	USART3_sendString(temporaryString);
	memset(temporaryString,0,30*sizeof(char));
	sprintf(temporaryString,"RES%c%c%c\n",resistance.hundreds,resistance.tens,resistance.ones);	
	USART3_sendString(temporaryString);	
	
	
	ATTINYTimer = 0;
	}
	
	#endif
}
//---------------------------------------------------------------------------------------------------------------------
bool MeasureSYBTvolts(bool *SYBTgood, struct volts_noload *ptrVolts){// && (!Main_Button_get_level)){	// Get voltmeter reading.  The returned value is true if connected, good battery or not.
	bool ConnectedToSYBT = false;
	
	array_SYBT[1].Volts_noload1 = (((float)cal.SYBT_ad_cal/1000) * (ADC_0_get_conversion(ADC_SYBT_Volts) >> 2))/10;				// corrected for SYBT 2  1.497v 
		//array_SYBT[1].Volts_noload1 = (((float)SYBT_adc_cal/1000) * (ADC_0_get_conversion(ADC_SYBT_Volts) >> 2))/10;			// corrected for SYBT 2  1.497v
	//if(FT2_get_level){																	//If relay is closed, then get new data from ADC
	//	array_SYBT[1].Stored_Volts_noload1 = array_SYBT[1].Volts_noload1;				// NOT NEEDED  It is the temperature that has to be stored
	//	}																				//	 --NEW--  now store volts to be used after relay has opened.
	if (array_SYBT[1].Volts_noload1	> 90){												//  If voltage is > 90, turn on green LED
		LED4_set_level(true);
		*SYBTgood = true;
		Rows |= GreenVoltageLED;
		ConnectedToSYBT = true;	
		//FT2_set_level(true);															//Relay for measuring temperature and resistance of SYBT
		//BlueLED_set_level(false);														// 
	}
	else{																				// If voltage is < 90, turn off green LED
		LED4_set_level(false);
		*SYBTgood = false;
		Rows |= GreenVoltageLED;
		Rows ^= GreenVoltageLED;
		ConnectedToSYBT = false;														//Used to be always True, not not always.   Need to change this -----Maybe change name to ConnectedToGoodSYBT?-----------------------------------------------------------------------
	}
	if ((array_SYBT[1].Volts_noload1 > 2) && (array_SYBT[1].Volts_noload1 < 8)){		//This uses my new design for detecting whether connected to an SYBT1 or 2
		SYBTconnectedButNoBattery = true;
		ConnectedToSYBT = true;															//	
		FT2_set_level(true);															//Relay for measuring temperature and resistance of SYBT ##############################################disabled for troubleshooting 
		BlueLED_set_level(false);  
		if(!SYBTalreadyConnectedButNoBattery){
			SleepTimer = 0;
			SYBTalreadyConnectedButNoBattery = true;
		}
		//array_SYBT[1].Volts_noload1 = 0;												// Set to zero to remove false 5 volt reading
	}
	else{
		SYBTconnectedButNoBattery = false;
		SYBTalreadyConnectedButNoBattery = false;
		//ConnectedToSYBT = false;
	}
	
	if(ConnectedToSYBT || SYBTconnectedButNoBattery){
		#ifndef ATTINY
			ptrVolts->ones = LED_charactermap(array_SYBT[1].Volts_noload1%10);						// If connected to a battery, display results.  Otherwise show dashes?
			ptrVolts->tens = LED_charactermap((array_SYBT[1].Volts_noload1/10)%10);
			ptrVolts->hundreds = LED_charactermap((array_SYBT[1].Volts_noload1/100)%10);																	// is it necessary to look at both LX and PX?			
		#endif
		#ifdef ATTINY
			ptrVolts->ones = ConvertToASCII(array_SYBT[1].Volts_noload1%10);						// If connected to a battery, display results.  Otherwise show dashes?
			ptrVolts->tens = ConvertToASCII((array_SYBT[1].Volts_noload1/10)%10);
			ptrVolts->hundreds = ConvertToASCII((array_SYBT[1].Volts_noload1/100)%10);	
		#endif
	}
	else{
		#ifndef ATTINY										
			ptrVolts->ones = DASH;												// If connected to a battery, display results.  Otherwise show dashes?
			ptrVolts->tens = DASH;
			ptrVolts->hundreds = DASH;
		#endif
		#ifdef ATTINY
			ptrVolts->ones = ASCII_DASH;						// If connected to a battery, display results.  Otherwise show dashes?
			ptrVolts->tens = ASCII_DASH;
			ptrVolts->hundreds = ASCII_DASH;
		#endif
	}
	return ConnectedToSYBT;
}
//-----------------------------------------------------------------------------------------------------------------------
int MeasureSYBTresistance(struct resistance *ptrResist){
	static float temp;
	if(FT2_get_level() && SleepTimer < 3000){//.....................................................................If FT2 is high, relay is closed and it is ok to take reading.  Otherwise use saved reading.
		
		ADC_0_get_conversion(ADC_SYBT_Resistor);			// if preferences are set to identify type, the type is displayed. Otherwise the resistance is displayed
		temp = (ADC_0_get_conversion(ADC_SYBT_Resistor) >> 2);
		if (temp > 1000){   // Over range or open circuit.   Use 999 as default, because actual full scale value of 1023 is too large to use.
			temp = 999;
			array_SYBT[1].Resistance = temp;
		} 
		else{
			array_SYBT[1].Resistance = ((temp * (float)cal.ResistA_cal )/((float)cal.ResistB_cal  - temp))/10;
		}
	}
	else if((!SYBTisProvidingPower)&&(!SYBTconnectedButNoBattery)){			//if SYBT disconnected, set display to show dashes.
		array_SYBT[1].Resistance = 999;
		temp = 999;	
		}	
		if(!Identify_Battery_Type(array_SYBT[1].Resistance, &Rows)){
			#ifndef ATTINY
				ptrResist->ones = LED_charactermap(array_SYBT[1].Resistance%10);
				ptrResist->tens = LED_charactermap((array_SYBT[1].Resistance/10)%10);			
				ptrResist->hundreds = LED_charactermap((array_SYBT[1].Resistance/100)%10);
				ptrResist->tens = (ptrResist->tens | 128);
			#endif
			#ifdef ATTINY
				ptrResist->ones = ConvertToASCII(array_SYBT[1].Resistance%10);
				ptrResist->tens = ConvertToASCII((array_SYBT[1].Resistance/10)%10);				// - AddDecimalToASCII;
				ptrResist->hundreds = ConvertToASCII((array_SYBT[1].Resistance/100)%10);
				//ptrResist->decimal = ASCII_decimal;
			#endif
			
		}
	return temp;
}
//-----------------------------------------------------------------------------------------
uint16_t MeasureSYBTtemperature(bool BatteryTestOK,uint32_t *DisplayVoltageDropTimer,int Test_Status, struct temperature *ptrTemp){
	static int Temperature;
	int TemperatureOffset;
	uint32_t tempTimer;	
	int TemperatureLimit;
	float RawTempData;
	static float tempTemperature;
	number_of_loops = 0;
	tempTimer = *DisplayVoltageDropTimer;    // apparently I can't increment referenced values, but I can increment a local var then make the referenced value equal to it. 
	char string[10];
	if(cal.Preferences & 1){
		TemperatureOffset = ((cal.SYBT_Therm_Offst * 9.0)/ 5.0 );
	}
	else{
		TemperatureOffset = cal.SYBT_Therm_Offst;
	}

	if(FT2_get_level() && SleepTimer < 3000){			//......................................................................If FT2 is high, relay is closed and it is ok to take reading.  Otherwise use saved reading. 	
		RawTempData = (ADC_0_get_conversion(ADC_SYBT_Temperature) >> 2);						// first get raw numbers from AD converter
		sprintf(string,"RawData:%u",(uint16_t)RawTempData);
		USART0_sendString(string);
		if(TypeOfBatteryConnected == 1){					// Is a standard SYBTU battery so no changes
			tempTemperature = ((RawTempData * (float)cal.Therm_ResA_cal)/((float)cal.Therm_ResB_cal - RawTempData)); //then convert to resistor value
			//tempTemperature = (((float)RawTempData * (float)cal.Therm_ResA_cal)/((float)cal.Therm_ResB_cal - (float)RawTempData)); //then convert to resistor value
			//tempTemperature = 2222;
			sprintf(string,"SYBTUresistance:%u",(uint16_t)tempTemperature);
			USART0_sendString(string);
			
		}else{											// Must be an E3S battery.  E3S puts out voltage signal from 0.8 to 3.2 volts
			tempTemperature = RawTempData;
		}
		
		sprintf(string,"RawTemp:%u",(uint16_t)tempTemperature);
		USART0_sendString(string);
		}																								// End of reading while relay is closed-----------------------------------------------------------------------------------	
		if(tempTemperature < 3000){																		// if temp is less than 3000 then calculate the temp or resistance 
			if(cal.Preferences & 4)	Temperature = tempTemperature/10;									// user selected resistance readout
		else{
			if(TypeOfBatteryConnected == 1){	
				Temperature = ConvertToTemperature(tempTemperature)+TemperatureOffset;					// user selected temperature readout	
				//tempTemperature = ConvertToTemperature(tempTemperature)+TemperatureOffset;			// user selected temperature readout		
				//GlobalTemperature = tempTemperature;	
				sprintf(string,"SYBTUtemp:%u",(uint16_t)Temperature);
				USART0_sendString(string);
				}
			else{
				Temperature = tempTemperature/8+10;
				sprintf(string,"E3Stemp:%u",(uint16_t)Temperature);
				USART0_sendString(string);
				}	
			}																							// End of Temperature of Resistance selected.
		array_SYBT[1].Temperature = Temperature;														// Save either temp or resistance to array	
	//	}																								// End of reading while relay is closed-----------------------------------------------------------------------------------	
		if ((*DisplayVoltageDropTimer > DisplayVoltageDropTime)){ 							//--------------------------------------------------------------temporary switch to displaying voltage drop after battery test
//			if(cal.Preferences & 1) TemperatureLimit = 90;										// if temperature set to F, then upper limit is 90. Long way of coding
//			else TemperatureLimit = 40;	
		TemperatureLimit = (cal.Preferences & 1)? 90 : 40;										// if temperature set to C, then upper limit is 40  Shorthand method of writing 
												
			if(Temperature < TemperatureLimit){
				Rows |= GreenTemperatureLED;													// green light on below limit   ----------------
			}
			else{
				Rows |= GreenTemperatureLED;													// green light off
				Rows ^= GreenTemperatureLED;
			}
		//}																						// End of reading while relay is closed-----------------------------------------------------------------------------------
		#ifndef ATTINY
			ptrTemp->ones = LED_charactermap(array_SYBT[1].Temperature%10);								// Copy values into "Struct temperature"
			ptrTemp->tens = LED_charactermap((array_SYBT[1].Temperature/10)%10);						// Copy values into "Struct temperature"		
			ptrTemp->hundreds = LED_charactermap((array_SYBT[1].Temperature/100)%10);					// Copy values into "Struct temperature"
			ptrTemp->tens = (ptrTemp->tens | 128);														// Copy values into "Struct temperature"		
		#endif 
		#ifdef ATTINY
			ptrTemp->ones = ConvertToASCII(array_SYBT[1].Temperature%10);								// Copy values into "Struct temperature"
			if(array_SYBT[1].Temperature > 100){ptrTemp->tens = ConvertToASCII((array_SYBT[1].Temperature/10)%10) - AddDecimalToASCII;}		// add decimal point if less than 100
			else{ptrTemp->tens = ConvertToASCII((array_SYBT[1].Temperature/10)%10);}
			ptrTemp->hundreds = ConvertToASCII((array_SYBT[1].Temperature/100)%10);						// Copy values into "Struct temperature"
		//	ptrTemp->decimal = ASCII_decimal;															// Copy values into "Struct temperature"		
		#endif
		
		/*
		ptrTemp->ones = LED_charactermap(Temperature%10);		// write temperature or resistance to display  old way
		ptrTemp->tens = LED_charactermap((Temperature/10)%10);
		ptrTemp->tens = (ptrTemp->tens | 128);
		ptrTemp->hundreds = LED_charactermap((Temperature/100)%10);
		*/
		Rows |= RedTemperatureLED;
		Rows ^= RedTemperatureLED;																// Turn off red LED ---------------------------- these appear backwards
		if(!DisplayVoltageDropTimerExpired){													// If Display Voltage Drop Timer is expired?
			if(SYBTconnectedButNoBattery || SYBTisProvidingPower){								// If SYBT connected but no battery OR SYBT is providing power
				FT2_set_level(true);															//Turn on relay to read SYBT temperature and resistance.##############################################disabled for troubleshooting 
				BlueLED_set_level(false);
				}																				// End of if SYBT connected but no battery OR SYBT is providing power
			DisplayVoltageDropTimerExpired = true;	
			}																					// End of if Display Voltage Drop Timer is expired?
		
		}//-------------------------------------------------------------------------------------------------end of if voltage drop timer is > 20----------------------------																						
		else{																//display voltage drop instead of temperature until timer runs out
			tempTimer ++;  // I needed to put in this temporary variable because it didn't work using pointers alone
			if(BatteryTestOK){
				if(SYBT.Voltage_Drop < 100){			// all this does is move decimal point.  Perhaps better way to do this.
					#ifndef ATTINY
						ptrTemp->ones = LED_charactermap((SYBT.Voltage_Drop)%10);
						ptrTemp->tens = LED_charactermap((SYBT.Voltage_Drop/10)%10);
						ptrTemp->tens = (ptrTemp->tens | 128);
					#endif
					#ifdef ATTINY
						ptrTemp->ones = ConvertToASCII((SYBT.Voltage_Drop)%10);
						ptrTemp->tens = ConvertToASCII((SYBT.Voltage_Drop/10)%10) - AddDecimalToASCII;
					#endif				
				}
				else{
					#ifndef ATTINY
						ptrTemp->ones = LED_charactermap((SYBT.Voltage_Drop/10)%10);
						ptrTemp->tens = LED_charactermap((SYBT.Voltage_Drop/100)%10);
						ptrTemp->ones = (ptrTemp->ones | 128);
					#endif
					#ifdef ATTINY
						ptrTemp->ones = ConvertToASCII((SYBT.Voltage_Drop)%10) - AddDecimalToASCII;
						ptrTemp->tens = ConvertToASCII((SYBT.Voltage_Drop/10)%10);
					#endif			
				}
				ptrTemp->hundreds = DASH;
			}																// end of if Battery Test OK
			else{															// start of if Battery Test Not OK
										
/*
	if(Test_Status == (Test_Status | 8)){					// IGBT Fail
		ptrTemp->ones = LED_number_1;
		ptrTemp->tens = Letter_F;
		ptrTemp->hundreds = DASH;
	}
	else if(Test_Status == (Test_Status | 16)){				// Relay Open Fail
		ptrTemp->ones = LED_number_2;
		ptrTemp->tens = Letter_F;
		ptrTemp->hundreds = DASH;		
	{							
	else if(Test_Status == (Test_Status | 32)){				// Relay Closed Fail
		ptrTemp->ones = LED_number_3;
		ptrTemp->tens = Letter_F;
		ptrTemp->hundreds = DASH;
	}
	else if(Test_Status == 100){				// Time Delay
		ptrTemp->ones = Letter_y;
		ptrTemp->tens = Letter_L;
		ptrTemp->hundreds = Letter_d;
	}
	else if(Test_Status == 200){				// Battery Bad
		ptrTemp->ones = LED_number_4;
		ptrTemp->tens = Letter_F;
		ptrTemp->hundreds = DASH;
	}
	*/				
				switch(Test_Status){										//Start of Switch
					case 14:
					case 28:
					case 42:
					case 56:
						#ifndef ATTINY
							ptrTemp->ones = LED_number_1;
							ptrTemp->tens = Letter_F;
							ptrTemp->hundreds = DASH;
						#endif
						#ifdef ATTINY
							ptrTemp->ones = ASCII_1;
							ptrTemp->tens = ASCII_F;
							ptrTemp->hundreds = ASCII_DASH;
						#endif
					break;
					case 21:
					case 35:
					case 59:
						#ifndef ATTINY
							ptrTemp->ones = LED_number_2;
							ptrTemp->tens = Letter_F;
							ptrTemp->hundreds = DASH;
						#endif
						#ifdef ATTINY
							ptrTemp->ones = ASCII_2;
							ptrTemp->tens = ASCII_F;
							ptrTemp->hundreds = ASCII_DASH;
						#endif				
					break;
					case 255:
						#ifndef ATTINY
							ptrTemp->ones = Letter_t;
							ptrTemp->tens = Letter_o;
							ptrTemp->hundreds = Letter_h;
						#endif
						#ifdef ATTINY
							ptrTemp->ones = ASCII_t;
							ptrTemp->tens = ASCII_o;
							ptrTemp->hundreds = ASCII_h;
						#endif
					break;
					case 100:
						#ifndef ATTINY												//Display Time Delay.  Disable this for when SYBT with not battery is connected
							ptrTemp->ones = Letter_y;
							ptrTemp->tens = Letter_L;
							ptrTemp->hundreds = Letter_d;
						#endif	
						#ifdef ATTINY
							ptrTemp->ones = ASCII_y;
							ptrTemp->tens = ASCII_L;
							ptrTemp->hundreds = ASCII_d;
						#endif					
					break;
					case 200:
						#ifndef ATTINY				
							ptrTemp->ones = LED_number_3;
							ptrTemp->tens = Letter_F;
							ptrTemp->hundreds = DASH;
						#endif	
						#ifdef ATTINY
							ptrTemp->ones = ASCII_3;
							ptrTemp->tens = ASCII_F;
							ptrTemp->hundreds = ASCII_DASH;
						#endif								
					break;					
				}														// End of Switch						
			}															// End of if Battery Test Not OK
			Rows |= RedTemperatureLED;														// Turn on red LED because the temperature is not OK
		}
	}	//....................................................................... end of if temperature is < 3000  .........................................................................................................
	else{											// temperature over 3000 means open circuit so display dashes in LEDs
		#ifndef ATTINY	
			ptrTemp->hundreds = DASH;
			ptrTemp->tens = DASH;
			ptrTemp->ones = DASH;
		#endif
		#ifdef ATTINY
			ptrTemp->hundreds = ASCII_DASH;
			ptrTemp->tens = ASCII_DASH;
			ptrTemp->ones = ASCII_DASH;		
		#endif
		array_SYBT[1].Temperature = 3000;
	}	
//else{
	//tempTimer++;	
	if((!SYBTisProvidingPower)&&(!SYBTconnectedButNoBattery)){							// If Sybester is disconnected, this changes the reading to dashes
		#ifndef ATTINY			
			ptrTemp->hundreds = DASH;
			ptrTemp->tens = DASH;
			ptrTemp->ones = DASH;
		#endif
		#ifdef ATTINY
			ptrTemp->hundreds = ASCII_DASH;
			ptrTemp->tens = ASCII_DASH;
			ptrTemp->ones = ASCII_DASH;
		#endif	
		array_SYBT[1].Temperature = 3000;
		}
//}
*DisplayVoltageDropTimer = tempTimer;
return RawTempData;	
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
void MeasureInternalTemperature(struct SYBT *ptrSYBT, struct cal *ptrCal){
//	float tempIntTemperature;
	uint16_t RawIntTempData;
	
	RawIntTempData = (ADC_0_get_conversion(ADC_Internal_Temp) >> 2);				// first get raw numbers from AD converter
	
	ptrSYBT->InternalTemperature = ConvertToTemperature((((float)RawIntTempData * (float)ptrCal->Int_Therm_ResA_cal)/((float)ptrCal->Int_Therm_ResB_cal- (float)RawIntTempData))) + cal.Intrnl_Therm_Offst;		
}
//------------------------------------------------------------------------------
void MeasureHeatSinkTemperature(struct SYBT *ptrSYBT, struct cal *ptrCal){
//	float tempHSTemperature;
	uint16_t RawHSTempData;
	
	RawHSTempData = (ADC_0_get_conversion(ADC_HS) >> 2);				// first get raw numbers from AD converter
	//tempIntTemperature = (((float)RawIntTempData * (float)ptrCal->Int_Therm_ResA_cal)/((float)ptrCal->Int_Therm_ResB_cal- (float)RawIntTempData));
	ptrSYBT->HeatSinkTemp = ConvertToTemperature((((float)RawHSTempData * (float)ptrCal->HS_Therm_ResA_cal)/((float)ptrCal->HS_Therm_ResB_cal- (float)RawHSTempData))) + cal.HS_Thermist_Offset;	
}
//-------------------------------This works ok too--------------------------------------------------------------------
/*
void SwitchToDemoMode(uint8_t *Rows){
	volts_noload.ones = LED_number_8;						// display simulated voltage of 108
	volts_noload.tens = LED_number_0;
	volts_noload.hundreds = LED_number_1;					// display simulated temperature of 72.1
	temperature.ones = LED_number_2;
	temperature.tens = 134;									// number one plus decimal
	temperature.hundreds = LED_number_7;
	resistance.ones = LED_number_0;							// display simulated battery c.10
	resistance.tens = LED_number_1;
	resistance.hundreds = Letter_c_dp;
	*Rows |= AllGreenLEDs;										// turns on green LEDs 32,64,128
	*Rows |= RedTempAndResistLEDs;	
	*Rows ^= RedTempAndResistLEDs;										// turns off red LED
	array_SYBT[1].Resistance = SYBT_DemoR;					//Sends "Demo" to app to display in resistance window.
	array_SYBT[1].Temperature = SYBT_DemoT;					//Sends "Demo" to app to display in resistance window.
	array_SYBT[1].Volts_noload1 = SYBT_DemoR;
	cal.Sleep_1 = 800000;
	cal.Sleep_2 = 800000;
}
*/
//------------------------------------The recommended way. No apparent difference than the wrong way without pointers.----------------------------
void SwitchToDemoModePtr(uint8_t *Rows, struct volts_noload *ptrVolts, struct temperature *ptrTemp, struct resistance *ptrResist, struct cal *ptrCal){

	if(number_of_loops == Cycle_4){
		DemoResistance ++;
		if(DemoResistance > 12)DemoResistance = 1;		
	}
	#ifndef ATTINY
		ptrVolts->ones = LED_number_8;							// display simulated voltage of 108
		ptrVolts->tens = LED_number_0;
		ptrVolts->hundreds = LED_number_1;					
		ptrTemp->ones = LED_number_2;							// display simulated temperature of 72.1
		ptrTemp->tens = LED_number_1_dp;						// number one plus decimal
		ptrTemp->hundreds = LED_number_7;
	#endif
	#ifdef ATTINY
		ptrVolts->ones = ASCII_8;								// display simulated voltage of 108
		ptrVolts->tens = ASCII_0;
		ptrVolts->hundreds = ASCII_1;							// display simulated temperature of 72.1
		ptrTemp->ones = ASCII_2;
		ptrTemp->tens = ASCII_1;								// number one 
		ptrTemp->hundreds = ASCII_7;							//Character will be interpreted by ATTINY as a 7 with decimal.  I removed this idea
		//ptrTemp->decimal = ASCII_decimal;						// plus decimal
	#endif
	
	switch (DemoResistance){
		case 1:
		case 2:
		case 3:
		#ifndef ATTINY
			ptrResist->ones = LED_number_6;						// display simulated battery v.66
			ptrResist->tens = LED_number_6;
			ptrResist->hundreds = Letter_v_dp;
		#endif
		#ifdef ATTINY
			ptrResist->ones = ASCII_6;							// display simulated battery c.10
			ptrResist->tens = ASCII_6;
			ptrResist->hundreds = ASCII_v_dp;
			//ptrResist->hundreds = ASCII_V;						//Capital V will be interpreted by ATTINY as a lowercase V with decimal. 
			//ptrResist->decimal = ASCII_decimal;		
		#endif
		break;
		case 4:
		case 5:
		case 6:
		#ifndef ATTINY
			ptrResist->ones = LED_number_0;							// display simulated battery c.10
			ptrResist->tens = LED_number_1;
			ptrResist->hundreds = Letter_c_dp;				
		#endif
		#ifdef ATTINY
			ptrResist->ones = ASCII_0;							// display simulated battery c.10
			ptrResist->tens = ASCII_1;
			ptrResist->hundreds = ASCII_c_dp;						//Capital C will be interpreted by ATTINY as a lowercase c with decimal. 
			//ptrResist->decimal = ASCII_decimal;
		#endif	
		break;
		case 7:
		case 8:
		case 9:
		#ifndef ATTINY		
			ptrResist->ones = LED_number_0;							// display simulated battery b.20
			ptrResist->tens = LED_number_2;
			ptrResist->hundreds = Letter_b_dp;
		#endif
		#ifdef ATTINY
			ptrResist->ones = ASCII_0;							// display simulated battery c.10
			ptrResist->tens = ASCII_2;
			ptrResist->hundreds = ASCII_b_dp;						//Capital B will be interpreted by ATTINY as a lowercase b with decimal. 
			//ptrResist->decimal = ASCII_decimal;		
		#endif		
		break;
		case 10:
		//	ReverseMarchingAnts_set_level(false);
			MarchingAnts_set_level(true);
		case 11:
		case 12:
		#ifndef ATTINY
			ptrResist->ones = LED_number_9;							// display simulated battery c.69
			ptrResist->tens = LED_number_6;
			ptrResist->hundreds = Letter_c_dp;
		#endif
		#ifdef ATTINY
		//	ReverseMarchingAnts_set_level(false);
			MarchingAnts_set_level(false);
			ptrResist->ones = ASCII_9;							// display simulated battery c.10
			ptrResist->tens = ASCII_6;
			ptrResist->hundreds = ASCII_c_dp;						//Capital C will be interpreted by ATTINY as a lowercase c with decimal. 
			//ptrResist->decimal = ASCII_decimal;			
		#endif
		break;
	}	
	#ifndef ATTINY	
		*Rows |= AllGreenLEDs;										// turns on green LEDs 32,64,128
		*Rows |= RedTempAndResistLEDs;
		*Rows ^= RedTempAndResistLEDs;										// turns off red LED
	#endif
	#ifdef ATTINY
		//Need way to convert LEDs to ASCII	
		
	#endif		
		array_SYBT[1].Resistance = SYBT_DemoR;				//Sends "Demo" to app to display in resistance window.
		array_SYBT[1].Temperature = SYBT_DemoT;					//Sends "Demo" to app to display in resistance window.
		array_SYBT[1].Volts_noload1 = SYBT_DemoR;		
	
	
		ptrCal->Sleep_1 = 800000;
		ptrCal->Sleep_2 = 800000;
}
//-------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
bool MeasureLXvoltage( struct volts_noload *ptrVolts){						// Detect and measure if LX battery is connected
			
	SYBT.LXvoltage = (((float)cal.LXvoltage_cal/1000) * (ADC_0_get_conversion(ADC_LXvoltage) >> 2))/10;  // Temporary raised to 2 to get around the E3S adapter adding voltage from thermistor amp to this terminal
		if(SYBT.LXvoltage > 2){
		 IsLXbattery = true;
			#ifndef ATTINY
				ptrVolts->ones = LED_charactermap(SYBT.LXvoltage%10);						// If connected to a battery, display results.  Otherwise show dashes?
				ptrVolts->tens = LED_charactermap((SYBT.LXvoltage/10)%10);
				ptrVolts->hundreds = LED_charactermap((SYBT.LXvoltage/100)%10);
			#endif
			#ifdef ATTINY
			
				ptrVolts->ones = (SYBT.LXvoltage%10);						// If connected to a battery, display results.  Otherwise show dashes
				ptrVolts->tens = ((SYBT.LXvoltage/10)%10);
				ptrVolts->hundreds = ((SYBT.LXvoltage/100)%10);
				
				//ptrVolts->ones = ConvertToASCII(SYBT.LXvoltage%10);						// If connected to a battery, display results.  Otherwise show dashes
				//ptrVolts->tens = ConvertToASCII((SYBT.LXvoltage/10)%10);
				//ptrVolts->hundreds = ConvertToASCII((SYBT.LXvoltage/100)%10);
				#ifndef DemoMode
					SendLXVoltsToLEDs();
				#endif							
			#endif
			return true;
		}
		else{
			#ifndef ATTINY
				ptrVolts->ones = DASH;						// If connected to a battery, display results.  Otherwise show dashes?
				//ptrVolts->tens = LED_charactermap((SYBT.LXvoltage/10)%10);
				ptrVolts->hundreds = DASH;
			#endif	
			#ifdef ATTINY
				//ptrVolts->ones = ASCII_DASH;						// If connected to a battery, display results.  Otherwise show dashes?
				//ptrVolts->tens = ConvertToASCII((SYBT.LXvoltage/10)%10);
				//ptrVolts->hundreds = ASCII_DASH;
				ptrVolts->ones = 0;						// If connected to a battery, display results.  Otherwise show dashes?
				ptrVolts->tens = 0;
				ptrVolts->hundreds = 0;
				//SendLXVoltsToLEDs();				
			#endif
			return false;	
		}
}
//-----------------------------------------------------------------------------------------------------------------------
#ifndef DemoMode
bool SendLXVoltsToLEDs(void){
	char temporaryString[9];
	//sprintf(temporaryString,"VOL%i%i%i\n",4,5,6);
	//sprintf(temporaryString,"VOL123\n",volts_noload.hundreds,volts_noload.tens,volts_noload.ones);
	sprintf(temporaryString,"VOL%u%u%u\n",volts_noloadLX.hundreds,volts_noloadLX.tens,volts_noloadLX.ones);
	//USART0_sendString(temporaryString);
	USART3_sendString(temporaryString);
}
#endif
//------------------------------------------------------------------------------------------------------------------------
bool IsSYBT5orE3SadapterConnected(void){						// Detect and measure if LX battery is connected or if E3S battery is connected

char temporaryString[12];
	adc_result_t E3S;
	//uint8_t E3S;
	E3S = (ADC_0_get_conversion(ADC_SYBT5_Or_E3S) >> 2)/1;  //
	
	//sprintf(temporaryString,"E3S %u\n",E3S);
	//USART0_sendString(temporaryString);

	if(E3S > 200 && E3S < 600 ){				// Is an E3S battery
		//USART0_sendString("E3S_Connected\n");
		IsE3Sbattery = true;
		TypeOfBatteryConnected = 3;				//E3S is battery Type 3
		return true;
	}
	else if(E3S < 100){						// Is an SYBT5 battery
		IsE3Sbattery = false;
		TypeOfBatteryConnected = 2;			//SYBT5 is battery Type 2
		return false;
	}
	else if(E3S > 700){						// Must be PX battery
		IsE3Sbattery = false;
		TypeOfBatteryConnected = 1;			//SYBTU is battery Type 1
		return true;
	}
	else{
		IsE3Sbattery = false;				// Default to PX for safety
		TypeOfBatteryConnected = 0;			//Default is battery Type 0
		return true;
	}
    
}