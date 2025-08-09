/*
 * bluetooth_data_in.h
 *
 * Created: 7/29/2024 9:14:39 AM
 *  Author: Robert F
 */ 

#ifndef BLUETOOTH_DATA_IN_H_
#define BLUETOOTH_DATA_IN_H_

extern struct cal;	// here to satisfy compiler complaints about the struct pointers

void identify_commands(uint8_t buffer30[]);
void USART0_getString(void);

void SYBTvoltsADcal(char buffer[30]);
void LoadvoltsADcal(char buffer[30]);	
void NineVoltBatteryCal(char buffer[30]);
void SixVoltBatteryCal(char buffer[30]);
void OhmmeterCalA(char buffer[30]);
void OhmmeterCalB(char buffer[30]);
void ThermistorCalA(char buffer[30]);
void ThermistorCalB(char buffer[30]);
void ThermistorCalOne(char buffer[30]);
void ThermistorCal2(char buffer[30]);			//10
void ThermistorCal3(char buffer[30]);
void ThermistorOffset(char buffer[30]);
void IntThermistorCalA(char buffer[30]);
void IntThermistorCalB(char buffer[30]);
void IntThermistorCalOne(char buffer[30]);
void IntThermistorCal2(char buffer[30]);
void IntThermistorCal3(char buffer[30]);
void IntThermistorOffset(char buffer[30]);
void HSThermistorCalA(char buffer[30]);
void HSThermistorCalB(char buffer[30]);
void HSThermistorCalOne(char buffer[30]);
void HSThermistorCal2(char buffer[30]);
void HSThermistorCal3(char buffer[30]);
void HSThermistorOffset(char buffer[30],char string[30]);
void FarenheitOrCentigrade(char buffer[30],char string[30]);
void TemperatureOrResistance(char buffer[30],char string[30]); //20
void TypeOrResistance(char buffer[30],char string[30]);
void AutoOnOff(char buffer[30],char string[30]);
void SYBTflavorOne(char buffer[30],char string[30]);
void SYBTflavorTwo(char buffer[30],char string[30]);
void SYBTflavorThree(char buffer[30],char string[30]);
void SYBTflavorFour(char buffer[30],char string[30]);
void RestoreSafeMode(void);
void ExitSafeMode(void);
void SleepTimeOne(char buffer[30]);
void SleepTimeTwo(char buffer[30]);								//30
void SleepDisable(void);
void SleepRestore(void);
void EnterDemoMode(void);
void ExitDemoMode(void);
void SendFlavorsToApp(void);
void SendCalDataToApp(void);
void SendPreferencesToApp(void);
void ReceiveBeepCommand(void);
void SleepTimeThree(char buffer[30]);
void SetBluetoothName(char buffer[30],char string[30]);			//40
void SetSYBTidentTolerance(char buffer[30]);
void EraseEEPROM(void);
void SetLoadTestToleranceA(char buffer[30]);
void SetLoadTestToleranceB(char buffer[30]);
void BeginAutoCalibration(struct cal *,char buffer[30]);
void EnterCalValue66(struct cal *);
//void EnterCalValue10(struct cal *);
void EnterCalValue20(struct cal *);
void EnterCalValue69(struct cal *);
void RunAutoCalibration(struct cal *);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
void SaveAutoCalibration(struct cal *);
void EchoOne(char buffer[30]);
void EchoTwo(char buffer[30]);
void EchoThree(char buffer[30]);
void EchoFour(char buffer[30]);
void EchoFive(char buffer[30]);
void EchoSix(char buffer[30]);
void EchoSix2(char buffer[30]);
void OverrideBluetoothLowBattery(char buffer[30]);
void ResistorZeroCal(char buffer[30]);
void LXvoltageCal(char buffer[30]);


#ifdef FifthGen
void LightLoadOn(char buffer[30]);
void LightLoadOff(char buffer[30]);
#endif


/* Data Out Functions*/
void TransmitBatteryTestData(char buffer[90], bool retransmit);
void TransmitBatteryData(char buffer[90], bool retransmit);
void TransmitADcalData(char buffer[30], bool retransmit);
void TransmitFlavorData(char buffer[50], bool retransmit);
void TransmitPreferenceData(char buffer[60],bool retransmit);
//bool SendSYBTflavors(void);
bool SendThermistorCal(void);


#endif /* BLUETOOTH_DATA_IN_H_ */