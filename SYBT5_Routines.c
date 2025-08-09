/*
 * SYBT5_Routines.c
 *
 * Created: 7/29/2024 8:12:25 AM
 *  Author: Robert F
 */ 

#include "bobs_functions.h"
#include "SYBT5_Routines.h"

void SendHeartbeatRX(void){
	USART_0_write(0x7E);   // sends "~" as heartbeat
	USART_0_write(0x0D);
}

void SendHeartbeatTX(void){
	USART_0_write(0x5E);   // sends "^" as heartbeat
	USART_0_write(0x0D);
	//LED4R_Toggle();
}


//------------------------------------------------------------------------------------------------------------------------------

uint16_t CalculateErrorCheckBits(bool CalculateOrCheck,bool Save){		// This works well to verify that incoming data is not corrupted.
	uint8_t i;															// Slightly modified from original PIC MPLABX code
	uint16_t RecordedErrorBit;
	uint16_t CalculatedErrorBit;
	uint8_t CalculatedErrorBitH;
	uint8_t CalculatedErrorBitL;
	char tempstring[70];
	//USART0_sendString("ChecksumCheck");

	for(i = 0,CalculatedErrorBit = 0; i < 80; i++){								
		CalculatedErrorBit = CalculatedErrorBit + compiledBatteryString[i];
	}
	//CalculatedErrorBit = compiledDataString[28];
	CalculatedErrorBitL = CalculatedErrorBit & 0xff;
	CalculatedErrorBitH = CalculatedErrorBit >> 8;
	if(CalculateOrCheck){
		RecordedErrorBit =  compiledBatteryString[GotoErrorCheckBitLoAddress] | compiledBatteryString[GotoErrorCheckBitHiAddress] <<8;
		if(RecordedErrorBit == CalculatedErrorBit){
			sprintf(tempstring,"CHKOK\n");
			USART3_sendString(tempstring);	
			USART0_sendString(tempstring);
			USART3_sendString(tempstring);	
			return 1;
		}
		else{
			USART0_sendString("CHKNOK");
			sprintf(tempstring,"CH%02x CL%02x EH%02x EL%02x\n SUMC%x SUMR%x\n",CalculatedErrorBitH,CalculatedErrorBitL,compiledBatteryString[GotoErrorCheckBitLoAddress],compiledBatteryString[GotoErrorCheckBitHiAddress],CalculatedErrorBit,RecordedErrorBit);
			//USART3_sendString("CHKNOK\n");
			USART0_sendString(tempstring);
			
			return 0;
		}
		}else{
		if(Save){
			_delay_ms(400);
			GoToAddress(GotoErrorCheckBitLoAddress);				//GotoErrorCheckBitLoAddress);  // Don't need high address because it increments automatically.
			_delay_ms(400);
			WriteToSYBT_EEPROM(CalculatedErrorBitL);
			_delay_ms(400);
			WriteToSYBT_EEPROM(CalculatedErrorBitH);
			_delay_ms(400);
		}
		return CalculatedErrorBit  ;
	}
}
//-------------------------------------------------------------------------------------------------------------
/*     From standalone SYBT5 program. This uses the more compact "logf" function.  Was necessary with PIC chips
int16_t ConvertToTemperature(adc_result_t RawTempData){
	
	float LogFloatSize;
	double logR2;
	double T;
	
	logR2 = logf(RawTempData);
	T = 1/(c1+(c2*logR2)+(c3*logR2*logR2*logR2));
	T = T - 273;
	T = ((T * 9.0)/ 5.0 + 32.0);
	return T;
}
*/
//------------------------------------------------------------------------------------------------------------------------------------
uint8_t ConvertShuntBitsToValue(uint8_t H,uint8_t L ){
    uint8_t ShuntValue = 255;
 
    if(H == 0x3a){ShuntValue = (L-255)*85/100+195;}
    //if(H == 0x3a){ShuntValue = (L-255)*8/10+195;}  Possibly change to this 
    if(H == 0x3b){ShuntValue = L*1643/1000+195;}
    return ShuntValue;    
}
//-------------------------------------------------------------------------------------------------------------------------------------
void ConvertValueToShuntBits(uint8_t ShuntVal,uint8_t *H,uint8_t *L ){
         
  if(ShuntVal <= 196){
      *H = 0x3a;
      *L = (ShuntVal-195)*100/85 + 255;}
  if(ShuntVal > 196){
      *H = 0x3b;
      *L = (ShuntVal-195)*1000/1643;}                  

} 
//-------------------------------------------------------------------------------------------------------------------------------------
void WriteToSYBT(uint8_t Command){				//writes but does not listen for response
	
	USART_1_disable_rx();
	USART_1_write(Command);
	_delay_us(5500);
	USART_1_write(0x00);
	_delay_us(100);		// was 1000
	USART_1_enable_rx();
}
//----------------------------------------------------------------------------------------------------------------
uint8_t ReadSYBT(uint8_t readback){		//  This is used to read the SYBT5 after a command is sent
	
	bool ReveivedOK;
	uint8_t character;
	//uint8_t readback;
	
	readback = USART_1_read_with_timeout();
	USART_0_write(readback);
	character = USART_1_read_with_timeout();
	USART_0_write(character);	
}
//----------------------------------------------------------------------------------------------------------------
bool GetDataFromSYBT(uint8_t Command){		//Sends and receives.  Data stored in global SYBTreturnedData  
	
	uint8_t readback;
	
	WriteToSYBT(Command);
	//if(USART_1_read_with_timeout();)
	readback = USART_1_read_with_timeout();
	//readback = USART_1_read();
	//USART_0_write(readback);	
	//USART_0_write(SYBTreturnedData);
	if(readback == Command){
		SYBTreturnedData = USART_1_read_with_timeout();
		LED4_set_level(false);
		return true;
		}
	else{
		SYBTreturnedData = 0x2A;
		LED4_set_level(true);
		return false;
		}
	//USART0_sendString("Send");
	//return 1;
}
//----------------------------------------------------------------------------------------------------------------
bool GoToAddress(uint8_t Address){			//sends go to EEPROM address command and then looks for verification. Returns TRUE or FALSE
	
	uint8_t readback;
	
	USART_1_disable_rx();
	USART_1_write(0x03);
	_delay_us(5500);
	USART_1_write(Address);
	_delay_us(1000);
	USART_1_enable_rx();
	
	readback = USART_1_read_with_timeout();
	//USART_0_write(readback);
	//SYBTreturnedData = USART_1_read();				// Using this makes it hang
	//USART_0_write(SYBTreturnedData);
	if(readback == Address){return true;}
	else{return false;}
}
//----------------------------------------------------------------------------------------------------------------
void WriteToSYBT_EEPROM(uint8_t Data){		// The SYBT5 code automatically increments EEPROM address. Must select start address first.
	
	USART_1_disable_rx();
	USART_1_write(0x05);				// Write To Eeprom Command
	_delay_us(5500);
	USART_1_write(Data);				// Data to be written
	_delay_us(1000);
	USART_1_enable_rx();
	return;								// Add verification to code?
}
//----------------------------------------------------------------------------------------------------------------
bool GetSYBT5serialNumber(void){
	
	bool ReceivedOK;
	uint8_t count;
	char SerialNumber[15];
	
	memset(SerialNumber,0,15*sizeof(char));
	ReceivedOK = GoToAddress(GotoSerialNumberAddress);//GotoSYBTdateAddress
	//USART_0_write(ReceivedOK);
	//USART_0_write(SYBTreturnedData);
	_delay_ms(CommDelay);
	for(count =0;count <12;count++){
		ReceivedOK = GetDataFromSYBT(ReadEEPROM);
		//	if(!ReceivedOK)break;
		//SerialNumber[count] = SYBTreturnedData;
		savedBatteryString[count + savedBatteryStringBitPosition] = SYBTreturnedData;		//Next is GetSYBT5switchPosition
		wdt_reset();
		_delay_ms(CommDelay);
		wdt_reset();
	}
	savedBatteryString[savedBatteryStringBitPosition + 12] = 0x2c;  //Hex for ASCII comma
	//savedBatteryStringBitPosition = savedBatteryStringBitPosition + 16;
	//USART0_sendString("\n");
	//USART0_sendString(SerialNumber);
	//USART0_sendString("\n");
	return ReceivedOK;
}
//----------------------------------------------------------------------------------------------------------------
bool GetSYBTdate(void){
	
	bool ReceivedOK;
	uint8_t count;
	char SYBT5date[9];
	
	memset(SYBT5date,0,9*sizeof(char));
	ReceivedOK = GoToAddress(GotoSYBTdateAddress);
	//USART_0_write(ReceivedOK);
	//USART_0_write(SYBTreturnedData);
	_delay_ms(CommDelay);
	
	for(count=0;count<8;count++){
		ReceivedOK = GetDataFromSYBT(ReadEEPROM);
		SYBT5date[count] = SYBTreturnedData;
		savedBatteryString[count+savedBatteryStringBitPosition] = SYBTreturnedData;		//Next is GetSYBT5serialNumber
		_delay_ms(CommDelay);
		wdt_reset();
	}
	savedBatteryString[savedBatteryStringBitPosition + 8] = 0x2c;  //Hex for ASCII comma
	savedBatteryStringBitPosition = savedBatteryStringBitPosition + 9;
	//USART0_sendString("\n");
	//USART0_sendString(SYBT5date);
	//USART0_sendString("\n");

	return ReceivedOK;
	
}
//----------------------------------------------------------------------------------------------------------------
bool GetSYBTname(void){
	
	bool ReceivedOK;
	uint8_t count;
	uint8_t SYBT5name[7];
	memset(SYBT5name,0,7*sizeof(uint8_t));
	memset(savedBatteryString,0,80*sizeof(uint8_t));
	savedBatteryStringBitPosition = 0;
	
	ReceivedOK = GoToAddress(GotoSYBTnameAddress);
	//USART_0_write(ReceivedOK);
	//USART_0_write(SYBTreturnedData);
	_delay_ms(CommDelay);					//Lines below adds these characters to the start of the string: 
	savedBatteryString[0] = 0x73;			//			"s"
	savedBatteryString[1] = 0x79;			//			"y"	
	savedBatteryString[2] = 0x62;			//			"b"												
	savedBatteryString[3] = 0x74;			//			"t"
	savedBatteryString[4] = 0x76;			//			"v"
	
	for(count=5;count<10;count++){
		ReceivedOK = GetDataFromSYBT(ReadEEPROM);
		SYBT5name[count] = SYBTreturnedData;
		savedBatteryString[count] = SYBTreturnedData;
		_delay_ms(CommDelay);
		wdt_reset();
	}
	savedBatteryString[10] = 0x2c;  //Hex for ASCII comma
	savedBatteryStringBitPosition = 11;   //Next is GetSYBTdate
	//USART0_sendString("SYBT5\n");
	//USART0_sendString(SYBT5name);
	//USART0_sendString("\n");
	return ReceivedOK;
}
//----------------------------------------------------------------------------------------------------------------
bool GetPrimaryEEPROM(void){   // This replaces the much larger code of the PIC MPLABX version.
	
	bool ReceivedOK;
	uint8_t count;
	uint8_t maxcount = 84;
	memset(compiledBatteryString,0,90*sizeof(uint8_t));
	ReceivedOK = GoToAddress(GotoStartAddress);
	USART_0_write(ReceivedOK);
	USART_0_write(SYBTreturnedData);
	_delay_ms(125);
	USART3_sendString("CHKNOK\n");
	USART0_sendString("SYBT5_EEPROM\n");
	for(count=0;count<maxcount;count++){
		if(IsSYBT5orE3SadapterConnected())return;
		ReceivedOK = GetDataFromSYBT(ReadEEPROM);
		compiledBatteryString[count] = SYBTreturnedData;
		_delay_ms(CommDelay);
		wdt_reset();
		LED4_toggle_level();
		SYBT5_RLY_set_level(true);
		PWR_ON_set_level(true);	
		}
	USART0_sendHEX(compiledBatteryString,maxcount);
	USART0_sendString("\nENDeeprom\n");	
	CalculateErrorCheckBits(true,false);	
	
	//LED4_set_level(true);
	PWR_ON_set_level(true);
	SYBT5_RLY_set_level(true);
}
//----------------------------------------------------------------------------------------------------------------
bool GetDataEEPROM(void){
	
	bool ReceivedOK;
	uint8_t count;
	uint8_t maxcount = 22;
	//uint8_t SYBT5data[30];
	//memset(SYBT5data,0,30*sizeof(uint8_t));
	memset(compiledDataString,0,40*sizeof(uint8_t));
	ReceivedOK = GoToAddress(GotoEEPROMaddress64);
	//USART_0_write(ReceivedOK);
	//USART_0_write(SYBTreturnedData);
	_delay_ms(125);
	USART0_sendString("SYBT5_Data\n");
	for(count=0;count<maxcount;count++){
		if(IsSYBT5orE3SadapterConnected())return;
		ReceivedOK = GetDataFromSYBT(ReadEEPROM);
		compiledDataString[count] = SYBTreturnedData;
		_delay_ms(CommDelay);
		USART_0_write(SYBTreturnedData);
		SYBT5_RLY_set_level(true);
	PWR_ON_set_level(true);
	wdt_reset();
	YellowLED_toggle_level();
	}
	USART0_sendHEX(compiledDataString,maxcount);
	USART0_sendString(" ENdata\n");

}
//----------------------------------------------------------------------------------------------------------------
bool GetSYBT5emperature(void){  //Reads directly from PIC, not from EEPROM, but needs 2 cycles because one cycle is used to calculate the temperature from raw data. 
	
	bool ReceivedOK;

	char SYBT5temperatureLocal[8];
	char SYBT5temperatureApp[4];
	memset(SYBT5temperatureLocal,0,8*sizeof(char));
	memset(SYBT5temperatureApp,0,4*sizeof(char));
	//ReceivedOK = GetDataFromSYBT(ReadTemperature);
	if(GetDataFromSYBT(ReadTemperature)){
			if(SYBTreturnedData < 100){
			//	SYBTreturnedData = SYBTreturnedData *10;
				sprintf(SYBT5temperatureLocal,"TMP%03i\n",SYBTreturnedData);
				SYBT5temperatureLocal[3] = SYBT5temperatureLocal[4];
				SYBT5temperatureLocal[4] = SYBT5temperatureLocal[5] - AddDecimalToASCII;
				SYBT5temperatureLocal[5] = ASCII_0;
			}
			else{
				sprintf(SYBT5temperatureLocal,"TMP%03i\n",SYBTreturnedData);			
			}		
		sprintf(SYBT5temperatureApp,"%03i,",SYBTreturnedData);
	}
	else{
		sprintf(SYBT5temperatureLocal,"TMP---\n");
		sprintf(SYBT5temperatureApp,"***,");
	}
	//USART_0_write(ReceivedOK);
	//_delay_ms(50);
	wdt_reset();
	strcat(savedBatteryString,SYBT5temperatureApp);						//Next is GetSYBT5shuntValue
	USART3_sendString(SYBT5temperatureLocal);
	//USART0_sendString(SYBT5temperatureLocal);
	
	return ReceivedOK;
}
//----------------------------------------------------------------------------------------------------------------
bool GetSYBT5highTemperature(void){		//Go to high temperature address. 
	
	bool ReceivedOK;
	uint8_t temperature;
	char SYBT5highTemperature[12];
	
	memset(SYBT5highTemperature,0,12*sizeof(char));
	GoToAddress(GotoHighTemperatureAddress);
		_delay_ms(50);
	if(GetDataFromSYBT(ReadEEPROM)){
		//USART_0_write(ReceivedOK);
		_delay_ms(CommDelay);
		//what is really needed is f to C conversion
		temperature = (SYBTreturnedData * 9)/5 + 32;		//Current temperature is reported in F but hi temperature is recorded in C. 
		sprintf(SYBT5highTemperature,"%03i,",temperature);		
	}
	else{
		sprintf(SYBT5highTemperature,"***,",temperature);
	}
	//USART_0_write(ReceivedOK);
	//USART_0_write(SYBTreturnedData);	
	//USART0_sendString("SYBT5_");
	strcat(savedBatteryString,SYBT5highTemperature);						//Next is GetSYBT5Temperature
	//USART0_sendString(SYBT5highTemperature);
	//USART0_sendString("\n");
	return ReceivedOK;
}
//----------------------------------------------------------------------------------------------------------------

bool GetSYBT5switchPosition(void){		//read directly from PIC. no addressing involved. 
	
	bool ReceivedOK;
	uint16_t temperature;
	char SYBT5switchPositionLocal[8];
	char SYBT5switchPositionApp[4];
	memset(SYBT5switchPositionLocal,0,8*sizeof(char));
	memset(SYBT5switchPositionApp,0,4*sizeof(char));
	
	if(GetDataFromSYBT(ReadSwitchPosition)){
		_delay_ms(CommDelay);
		if(SYBTreturnedData){
			//sprintf(SYBT5switchPosition,"SYBT5_SWI.ON\n",SYBTreturnedData);  //------------------this is the one that is causing problems
			sprintf(SYBT5switchPositionLocal,"SWI.ON\n",SYBTreturnedData);
			sprintf(SYBT5switchPositionApp," ON,",SYBTreturnedData);
		}
		else{
			//sprintf(SYBT5switchPosition,"SYBT5_SWIOFF\n",SYBTreturnedData);
			sprintf(SYBT5switchPositionLocal,"SWIOFF\n",SYBTreturnedData);		//must be 3 characters. Either "OFF" or "ON."
			sprintf(SYBT5switchPositionApp,"OFF,",SYBTreturnedData);
		}
	}
	else{
		sprintf(SYBT5switchPositionLocal,"SWI---\n");		//must be 3 characters. Need to fix ATTINY program to work with this.
		sprintf(SYBT5switchPositionApp,"***,");
	}
	
	
	
	strcat(savedBatteryString,SYBT5switchPositionApp);						//Next is Cal Factor
	//USART3_sendString(SYBT5switchPositionLocal);
	//USART0_sendString(SYBT5switchPosition);
	//USART3_sendString(SYBT5switchPosition);
	
	return ReceivedOK;
}
//----------------------------------------------------------------------------------------------------------------
bool GetSYBT5calFactor(void){		//Go directly to address for cal factor.
	
	bool ReceivedOK;
	uint16_t temperature;
	char calFactorLocal[10];
	char calFactorApp[4];
	memset(calFactorLocal,0,10*sizeof(char));
	memset(calFactorApp,0,4*sizeof(char));
	ReceivedOK = GoToAddress(GotoCalFactorAddress);
	//USART_0_write(ReceivedOK);
	//USART_0_write(SYBTreturnedData);
	_delay_ms(CommDelay);
	if(GetDataFromSYBT(ReadEEPROM)){
		_delay_ms(CommDelay);
		sprintf(calFactorLocal,"RES%03i\n",SYBTreturnedData);				//
		calFactorLocal[3] = calFactorLocal[3] - AddDecimalToASCII;
		//sprintf(calFactor,"SYBT5_CAL: %03i\n",SYBTreturnedData);			
		sprintf(calFactorApp,"%03i,",SYBTreturnedData);						//to send to app
		}
		else{
		sprintf(calFactorLocal,"RES***\n");				//
		//sprintf(calFactor,"SYBT5_CAL: %03i\n",SYBTreturnedData);			
		sprintf(calFactorApp,"***,");										//to send to app			
		}	
	USART3_sendString(calFactorLocal);	
	strcat(savedBatteryString,calFactorApp);								//Next is GetSYBT5highTemperature
	//USART0_sendString(calFactorLocal);	
	wdt_reset();
	return ReceivedOK;
}
//----------------------------------------------------------------------------------------------------------------
bool GetSYBT5shuntValue(void){   // need to add shunt conversion formula
	
	bool ReceivedOK;
	uint16_t temperature;
	uint8_t ShuntRaw[2];
	uint8_t ShuntValueString[30];
	uint8_t ShuntValue;
	memset(ShuntValueString,0,20*sizeof(uint8_t));
	_delay_ms(CommDelay);
	ReceivedOK = GoToAddress(GotoShuntValLoAddress);				//GotoShuntValLoAddress
	//USART_0_write(ReceivedOK+1);
	//USART_0_write(SYBTreturnedData);
	_delay_ms(CommDelay);
	if(GetDataFromSYBT(ReadEEPROM)){
		//USART_0_write(ReceivedOK+2);
		//USART_0_write(SYBTreturnedData);
		ShuntRaw[0] = SYBTreturnedData;
		_delay_ms(CommDelay);
		ReceivedOK = GetDataFromSYBT(ReadEEPROM);
		//USART_0_write(ReceivedOK+3);
		//USART_0_write(SYBTreturnedData);
		ShuntRaw[1] = SYBTreturnedData;
		ShuntValue = ConvertShuntBitsToValue(ShuntRaw[1],ShuntRaw[0]);
		//sprintf(ShuntValueString,"SYBT5  Sh:%i",ShuntValue);
		//sprintf(ShuntValueString,"%03i,%3d,V%3d,%d sybend\n\r",ShuntValue,twentyfourvoltsupply,SYBT.LXvoltage,TypeOfBatteryConnected);
		sprintf(ShuntValueString,"%03i,%3d,V%3d,%d,%d sybend\n\r",ShuntValue,twentyfourvoltsupply,SYBT.LXvoltage,TypeOfBatteryConnected,SixVoltBattery);	
	}
	else{
		//sprintf(ShuntValueString,"***,%3d,V%3d,%d sybend\n\r",twentyfourvoltsupply,SYBT.LXvoltage,TypeOfBatteryConnected);
		sprintf(ShuntValueString,"***,%3d,V%3d,%d,%d sybend\n\r",twentyfourvoltsupply,SYBT.LXvoltage,TypeOfBatteryConnected,SixVoltBattery);	
	}
	
	//sprintf(savedBatteryString,"sybtvRubia,051224,GL2024012001,SWION,CAL099,HTMP055,TMP081,Sh198,sybend\n\r");
	strcat(savedBatteryString,ShuntValueString);						//Last in line, so a carriage return is added.
	//USART0_sendHEX(ShuntRaw,2);
	//USART0_sendString(ShuntValueString);
	//USART0_sendString("  \n");
	return ReceivedOK;
}
//------------------------------------------------------------------------------------------------------------------------------------
bool FastSYBT5query(void){
	if(LookForAnyChanges()){						//A good fast query. All data is compiled in a single string that is sent at the end.
		//ATTINY_PB1_set_level(true);
		//USART0_sendString("StartLX\n\r");
		LED4_set_level(true);
		//YellowLED_set_level(false);
		if(!IsConnectedToSYBT5()){return false;}
		GetSYBTname();								//Name
		//USART0_sendString("Name_LX\n\r");
		//YellowLED_set_level(true);
		LED4_set_level(true);
		if(!IsConnectedToSYBT5()){return false;}	
		GetSYBTdate();								//Date
		LED4_set_level(true);
		//USART0_sendString("Date_LX\n\r");	
		//YellowLED_set_level(false);
		if(!IsConnectedToSYBT5()){return false;}
		GetSYBT5serialNumber();						//Serial Number
		LED4_set_level(true);
		//USART0_sendString("Serial_LX\n\r");
		//YellowLED_set_level(true);
		if(!IsConnectedToSYBT5()){return false;}
		GetSYBT5switchPosition();					//Switch Position
		LED4_set_level(true);
		//BlueLED_toggle_level();
		LED4_set_level(true);
		if(!IsConnectedToSYBT5()){return false;}
		GetSYBT5calFactor();						//CalFactor
		LED4_set_level(true);
		if(!IsConnectedToSYBT5()){return false;}
		GetSYBT5highTemperature();
		LED4_set_level(true);
		if(!IsConnectedToSYBT5()){return false;}
		GetSYBT5emperature();						//Temperature  -         -----   string was too small at 8 and corrupting other strings
		LED4_set_level(true);
		//BlueLED_toggle_level();
		LED4_set_level(true);
		if(!IsConnectedToSYBT5()){return false;}
		GetSYBT5shuntValue();						//Shunt Value. For some reason shunt needs to follow temperature for temperature to not corrupt things.
		
		USART0_sendString(savedBatteryString);		//String looks like this:  SYBT5,MM/DD/YY,SERIALNUMBER,SWI.ON,CAL099,HTMP055,TMP084,SH198   62 characters
		//USART0_sendString("End_LX\n\r");
		MarchingAnts_set_level(false);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------------------------------
bool IsConnectedToSYBT5(void){										// Modified for combined program.
	static uint16_t SYBT5connectedDelay;
	
	if(!IsSYBT5orE3SadapterConnected()){								//HIGH it is an SYBTU battery LOW is SYBT5	// digital method.		However, analog could do two things.  Sense SYBT5 and E3S  by looking at voltage levels.				
		if(SYBT5connectedDelay >5000){
			LED1_set_level(false);
			if(!SYBT5_RLY_get_level()){								// I'm using this as a one shot. 
				USART0_sendString("LX_CONNECTED\n");					//  "\r" not needed here because data follows this program path.
				ReverseMarchingAnts_set_level(false);
				MarchingAnts_set_level(true);
				}			
			SYBT5_RLY_set_level(true);
			
			return true;
		}
		else{
			ReverseMarchingAnts_set_level(true);
			SYBT5connectedDelay++;							// && SYBT5connectedDelay <100)		
			return false;	
			}
	}
	else{
		SYBT5connectedDelay = 0;
		//USART0_sendString("LXnotCONNECT\n");
		SYBT5_RLY_set_level(false);
		return false;
	}
}
//------------------------------------------------------------------------------------------------------
bool LookForAnyChanges(void){						//If a new command or the SYBT5 is unplugged, then stop.
	if(!LookForCommands() && IsConnectedToSYBT5()){
		return true;
	}
	else{
		return false;
	}
}
//------------------------------------------------------------------------------------------------------------------------------------
bool LookForCommands(void){
	if (USART_0_is_rx_ready()){					//listens to Bluetooth module
		//		USART_0_enable_rx();
		USART0_getString();
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------	
/*
bool USART_1_flush(void){		//not used
	uint8_t trash;
	while (USART1.STATUS & USART_RXCIF_bm){
		return USART1.RXDATAH;
		return USART1.RXDATAL;
	}
}
bool USART_0_flush(void){	//not used
	uint8_t trash;
	uint8_t trash;
	while (USART0.STATUS & USART_RXCIF_bm){
		return USART0.RXDATAH;
		return USART0.RXDATAL;
	}
}
*/
//--------------------------------------------------------------------------------------------
/*
bool RequestAllInformationFromBattery(bool restart){		//From PIC MPLABX code. Here for reference only 
	
	static bool SerialNumberRequestStart;
	static uint8_t SerialNumberData;
	//static bool CalFactorRequestStart;
	static bool TemperatureOrSwitch;
	static uint8_t CalFactorData;
	static bool Complete;
	char string[10];
	//uint8_t write[2];
	//Write_a_character(LCD_Line1_start,0x59);
	
	if(restart){
		SerialNumberRequestStart = 0;
		SerialnumberOrCalFactor = 0;
		SerialNumberData = 0;
		Complete = 0;
		bytecount = 0;
	}
	//   if(TMR4IF){                     // If Timer 4 timed out, transmit data request and reset timer.
	//       TMR4IF = 0;                 // PIR2bits.TMR4IF = 0;
	//       T4PR = 0x07;
	//       LED0_Toggle();
	//       TMR4_Start();
	
	if(!Complete){
		if(!SerialnumberOrCalFactor){
			if(!SerialNumberRequestStart){
				SerialNumberRequestStart = 1;
				SerialNumberData = 0;
				//CalFactorRequestStart = 1;
				CalFactorData = 0;
				bytecount = 0;
				GoToAddress(0x00);				//address zero
				return 0;
				}else if(SerialNumberData < 83){
				SerialNumberData ++;
				bytecount ++;
				SendHeartbeatRX();
				WriteToSYBT(ReadEEPROM);
				return 0;
				}else{
				SerialNumberData = 0;
				SerialnumberOrCalFactor = 1;
				bytecount = 0;
				linecount = 0;
				bool CollectionComplete = 1;
				GoToAddress(0x64);				//address 0x64
				return 0;
			}
			}else if(CalFactorData < 40){  //was 27
			CalFactorData ++;
			bytecount ++;
			SendHeartbeatRX();
			WriteToSYBT(ReadEEPROM);
			return 0;
			}else if (!TemperatureOrSwitch){
			bytecount ++;
			//write[0] = 0x09;                             // command for Read Temperature
			sprintf(string,"TEMP:%u",bytecount);
			USART0_sendString(string);
			WriteToSYBT(ReadTemperature);                    //readTemperature -
			TemperatureOrSwitch = 1;
			//bool CollectionComplete = 1;
			return 0;
			}else if (TemperatureOrSwitch){
			bytecount++;
			//linecount = 0;
			//CalFactorData = 0;
			//SerialNumberData = 0;
			//SerialNumberRequestStart = 0;
			//TemperatureOrSwitch = 0;
			//SerialnumberOrCalFactor = 0;
			//SendSerialnumberToScreen();
			//bool CollectionComplete = 1;
			//SendSerialnumberToScreen();
			//write[0] = 0x0B;                             // command for Read Switch Position
			sprintf(string,"SWITCH:%u",bytecount);
			USART0_sendString(string);
			WriteToSYBT(ReadSwitchPosition);                        //readSwitchPosition -
			Complete = 1;
			return 0;
		}
		} else{
		
		Complete = 0;
		linecount = 0;
		CalFactorData = 0;
		SerialNumberData = 0;
		SerialNumberRequestStart = 0;
		TemperatureOrSwitch = 0;
		SerialnumberOrCalFactor = 0;
		if(!CollectionComplete){
			// ResetErrorCheckBytes();
			CollectionComplete = true;
		}
		if(restart){
			CalculateErrorCheckBits(true,false);
		}
		//SendSerialnumberToScreen();
		//LED0_SetHigh();
		//LED4R_SetHigh();
		//printf("waitawhile\n");
		return 1;
	}
	
	//}
	
	return 1;
}
*/