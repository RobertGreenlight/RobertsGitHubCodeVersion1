/*
This is corrected for the latest generation of board with large chips.  11-15-2021
Now corrected for ATTINY chip and SYBT5 I/O functions  03-10-2025

1:	Serial Port Out:			PortA 0		To Bluetooth
2:	Serial Port In:				PortA 1		From Bluetooth
3:  ATTINY PB0					PortA 3	    ATTINY PB0
4:	ATTINY PB1					PortA 4		ATTINY PB1		MarchingAnts
5:	ATTINY PA5					PortA 5		ATTINY PA5		Reverse MarchingAnts
6:	ATTINY PA6					PortA 6		ATTINY PA6
7:	Seven Segment LT Pin:		PortA 7		
8:	PWM							PortB 0		ATTINY TX		formerly not used
9:	MR Pin						PortB 1		ATTINY RX		formerly not used
10:	PWR_ON:						PortB 4     Either on-board switch U8 or external via ON1
11:	FT2:						PortB 5		Relay control for SYBT5 compatible versions
12:	PC0:						PortC 0		SYBT5 TX		formerly Not used  Secondary Serial Port Out
13:	PC1:						PortC 1		SYBT5 RX		formerly Not used  Secondary Serial Port In
14: Beeper:						PortC 4
15: Large Chip Display:			PortC 5		Controls voltage regulator for LEDs and drivers
16: LED 1:						PortC 6		Legacy LED for troubleshooting
17: Q15_Sybt5Rly:				PortC 7		Now SYBT5 relay control formerly legacy button for troubleshooting
18: LX Volts:					PortD 0	A0	Formerly HS   Optional analog input. R16 inline resistor. Goes to terminal "H"
19: Internal Temperature:		PortD 1	A1	Internal thermistor. Uses 30k resistor connected to VDD, thermistor to GND
20: SYBT5 Connected:			PortD 2	A2	See if useful for E3S batteries.  Connected to output of U8. Has voltage divider R17 R18. Currently used for SYBT5 sense. Need to use for E3S batteries too
21: 24 volt sense:				PortD 3	A3	Goes to terminal "S"  Voltage divider on power supply board.
22: SYBT Resistor sense:		PortD 4	A4	Uses R13 as voltage divider and R11 & C8 as a filter
23: SYBT Temperature sense:		PortD 5	A5	Uses R12 as voltage divider and R28 & C7 as a filter
24: Spare Input:				PortD 6	A6	Now with pullup resistor.  formerly direct line to terminal "J" .
25: LED 4						PortD 7		Red half of bi color LED next to voltage readout.
26: SYBT Volts:					PortE 0	A8	Uses either onboard divider R32 to R35 or external divider.  Be sure at least R34 is not installed if using external divider.
27: Load Test Volts:			PortE 1	A9	Direct line to terminal "LIN"
28: Blue LED					PortE 2		Blue LED 8
29: Six Volt Battery Sense:		PortE 3	A11	Uses onboard divider R14 R15. Goes to terminal "6V" Actually measures 5 volt bus after switch but before power supply isolating diodes
30: SYBT5 and E3S Sense			PortF 0	A16	SYBT5 Sense
31: Yellow LED					PortF 1		All new LED9
32: HS thermistor:				PortF 2	A18	Uses R9 as voltage divider.
33: Main Button sense:			PortF 3		Senses closure of main button
34: Load Relay control:			PortF 4		Controls NPN Q6
35: Load IGBT gate control:		PortF 5		Controls NPN Q7
36: Reset:						PortF 6		Not used
*/

#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <port.h>

//--------------------------------------------------------------------------------------------------
static inline void Bluetooth_TX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(0, pull_mode);
}

static inline void Bluetooth_TX_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(0, dir);
}

static inline void Bluetooth_TX_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(0, isc);
}

static inline void Bluetooth_TX_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(0, inverted);
}

static inline void Bluetooth_TX_set_level(const bool level)
{
	PORTA_set_pin_level(0, level);
}

static inline void Bluetooth_TX_toggle_level()
{
	PORTA_toggle_pin_level(0);
}

static inline bool Bluetooth_TX_get_level()
{
	return PORTA_get_pin_level(0);
}
//---------------------------------------------------------------------------------------------------
static inline void Bluetooth_RX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(1, pull_mode);
}

static inline void Bluetooth_RX_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(1, dir);
}

static inline void Bluetooth_RX_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(1, isc);
}

static inline void Bluetooth_RX_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(1, inverted);
}

static inline void Bluetooth_RX_set_level(const bool level)
{
	PORTA_set_pin_level(1, level);
}

static inline void Bluetooth_RX_toggle_level()
{
	PORTA_toggle_pin_level(1);
}

static inline bool Bluetooth_RX_get_level()
{
	return PORTA_get_pin_level(1);
}
//--------------------------------------------------------------------------------------------------------
static inline void ATTINY_PB0_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(3, pull_mode);
}

static inline void ATTINY_PB0_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(3, dir);
}
     
static inline void ATTINY_PB0_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(3, isc);
}

static inline void ATTINY_PB0_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(3, inverted);
}

static inline void ATTINY_PB0_set_level(const bool level)
{
	PORTA_set_pin_level(3, level);
}

static inline void ATTINY_PB0_toggle_level()
{
	PORTA_toggle_pin_level(3);
}

static inline bool ATTINY_PB0_get_level()
{
	return PORTA_get_pin_level(3);
}
//------------------------------------------------------------------------------------------------
static inline void MarchingAnts_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(4, pull_mode);
}

static inline void MarchingAnts_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(4, dir);
}

static inline void MarchingAnts_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(4, isc);
}

static inline void MarchingAnts_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(4, inverted);
}

static inline void MarchingAnts_set_level(const bool level)
{
	PORTA_set_pin_level(4, level);
}

static inline void MarchingAnts_toggle_level()
{
	PORTA_toggle_pin_level(4);
}

static inline bool MarchingAnts_get_level()
{
	return PORTA_get_pin_level(4);
}
//------------------------------------------------------------------------------------------------
static inline void ReverseMarchingAnts_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(5, pull_mode);
}

static inline void ReverseMarchingAnts_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(5, dir);
}

static inline void ReverseMarchingAnts_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(5, isc);
}

static inline void ReverseMarchingAnts_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(5, inverted);
}

static inline void ReverseMarchingAnts_set_level(const bool level)
{
	PORTA_set_pin_level(5, level);
}

static inline void ReverseMarchingAnts_toggle_level()
{
	PORTA_toggle_pin_level(5);
}

static inline bool ReverseMarchingAnts_get_level()
{
	return PORTA_get_pin_level(5);
}
//---------------------------------------------------------------------------------------
static inline void ATTINY_PA6_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTA_set_pin_pull_mode(6, pull_mode);
}

static inline void ATTINY_PA6_set_dir(const enum port_dir dir)
{
	PORTA_set_pin_dir(6, dir);
}

static inline void ATTINY_PA6_set_isc(const PORT_ISC_t isc)
{
	PORTA_pin_set_isc(6, isc);
}

static inline void ATTINY_PA6_set_inverted(const bool inverted)
{
	PORTA_pin_set_inverted(6, inverted);
}

static inline void ATTINY_PA6_set_level(const bool level)
{
	PORTA_set_pin_level(6, level);
}

static inline void ATTINY_PA6_toggle_level()
{
	PORTA_toggle_pin_level(6);
}

static inline bool ATTINY_PA6_get_level()
{
	return PORTA_get_pin_level(6);
}
//----------------------------------------------------------------------------------------
static inline void AVR_TX_TINY_RX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(0, pull_mode);
}

static inline void AVR_TX_TINY_RX_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(0, dir);
}

static inline void AVR_TX_TINY_RX_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(0, isc);
}

static inline void AVR_TX_TINY_RX_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(0, inverted);
}

static inline void AVR_TX_TINY_RX_set_level(const bool level)
{
	PORTB_set_pin_level(0, level);
}

static inline void AVR_TX_TINY_RX_toggle_level()
{
	PORTB_toggle_pin_level(0);
}

static inline bool AVR_TX_TINY_RX_get_level()
{
	return PORTB_get_pin_level(0);
}
//------------------------------------------------------------------------------------------------------
static inline void AVR_RX_TINY_TX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(1, pull_mode);
}

static inline void AVR_RX_TINY_TX_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(1, dir);
}

static inline void AVR_RX_TINY_TX_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(1, isc);
}

static inline void AVR_RX_TINY_TX_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(1, inverted);
}

static inline void AVR_RX_TINY_TX_set_level(const bool level)
{
	PORTB_set_pin_level(1, level);
}

static inline void AVR_RX_TINY_TX_toggle_level()
{
	PORTB_toggle_pin_level(1);
}

static inline bool AVR_RX_TINY_TX_get_level()
{
	return PORTB_get_pin_level(1);
}
//--------------------------------------------------------------------------------------
static inline void PWR_ON_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(4, pull_mode);
}

static inline void PWR_ON_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(4, dir);
}

static inline void PWR_ON_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(4, isc);
}

static inline void PWR_ON_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(4, inverted);
}

static inline void PWR_ON_set_level(const bool level)
{
	PORTB_set_pin_level(4, level);
}

static inline void PWR_ON_toggle_level()
{
	PORTB_toggle_pin_level(4);
}

static inline bool PWR_ON_get_level()
{
	return PORTB_get_pin_level(4);
}
//------------------------------------------------------------------------------------
static inline void FT2_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTB_set_pin_pull_mode(5, pull_mode);
}

static inline void FT2_set_dir(const enum port_dir dir)
{
	PORTB_set_pin_dir(5, dir);
}

static inline void FT2_set_isc(const PORT_ISC_t isc)
{
	PORTB_pin_set_isc(5, isc);
}

static inline void FT2_set_inverted(const bool inverted)
{
	PORTB_pin_set_inverted(5, inverted);
}

static inline void FT2_set_level(const bool level)
{
	PORTB_set_pin_level(5, level);
}

static inline void FT2_toggle_level()
{
	PORTB_toggle_pin_level(5);
}

static inline bool FT2_get_level()
{
	return PORTB_get_pin_level(5);
}
//-------------------------------------------------------------------------
static inline void SYBT_TX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(0, pull_mode);
}

static inline void SYBT_TX_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(0, dir);
}

static inline void SYBT_TX_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(0, isc);
}

static inline void SYBT_TX_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(0, inverted);
}

static inline void SYBT_TX_set_level(const bool level)
{
	PORTC_set_pin_level(0, level);
}

static inline void SYBT_TX_toggle_level()
{
	PORTC_toggle_pin_level(0);
}

static inline bool SYBT_TX_get_level()
{
	return PORTC_get_pin_level(0);
}
//-----------------------------------------------------------------------------------------------------------------
static inline void SYBT_RX_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(1, pull_mode);
}

static inline void SYBT_RX_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(1, dir);
}

static inline void SYBT_RX_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(1, isc);
}

static inline void SYBT_RX_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(1, inverted);
}

static inline void SYBT_RX_set_level(const bool level)
{
	PORTC_set_pin_level(1, level);
}

static inline void SYBT_RX_toggle_level()
{
	PORTC_toggle_pin_level(1);
}

static inline bool SYBT_RX_get_level()
{
	return PORTC_get_pin_level(1);
}
//--------------------------------------------------------------------------------------------------------------------------------------
static inline void EPO_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(2, pull_mode);
}

static inline void EPO_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(2, dir);
}

static inline void EPO_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(2, isc);
}

static inline void EPO_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(2, inverted);
}

static inline void EPO_set_level(const bool level)
{
	PORTC_set_pin_level(2, level);
}

static inline void EPO_toggle_level()
{
	PORTC_toggle_pin_level(2);
}

static inline bool EPO_get_level()
{
	return PORTC_get_pin_level(2);
}
//---------------------------------------------------------------------------------------------------------------------------------------
static inline void Beeper_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(4, pull_mode);
}

static inline void Beeper_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(4, dir);
}

static inline void Beeper_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(4, isc);
}

static inline void Beeper_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(4, inverted);
}

static inline void Beeper_set_level(const bool level)
{
	PORTC_set_pin_level(4, level);
}

static inline void Beeper_toggle_level()
{
	PORTC_toggle_pin_level(4);
}

static inline bool Beeper_get_level()
{
	return PORTC_get_pin_level(4);
}
//-------------------------------------------------------------------------------------------------------
static inline void LargeChip_Display_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(5, pull_mode);
}

static inline void LargeChip_Display_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(5, dir);
}

static inline void LargeChip_Display_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(5, isc);
}

static inline void LargeChip_Display_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(5, inverted);
}

static inline void LargeChip_Display_set_level(const bool level)
{
	PORTC_set_pin_level(5, level);
}

static inline void LargeChip_Display_toggle_level()
{
	PORTC_toggle_pin_level(5);
}

static inline bool LargeChip_Display_get_level()
{
	return PORTC_get_pin_level(5);
}
//--------------------------------------------------------------------------------------------------
static inline void LED1_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(6, pull_mode);
}

static inline void LED1_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(6, dir);
}

static inline void LED1_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(6, isc);
}

static inline void LED1_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(6, inverted);
}

static inline void LED1_set_level(const bool level)
{
	PORTC_set_pin_level(6, level);
}

static inline void LED1_toggle_level()
{
	PORTC_toggle_pin_level(6);
}

static inline bool LED1_get_level()
{
	return PORTC_get_pin_level(6);
}
//--------------------------------------------------------------------------------------------------
static inline void SYBT5_RLY_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTC_set_pin_pull_mode(7, pull_mode);
}

static inline void SYBT5_RLY_set_dir(const enum port_dir dir)
{
	PORTC_set_pin_dir(7, dir);
}

static inline void SYBT5_RLY_set_isc(const PORT_ISC_t isc)
{
	PORTC_pin_set_isc(7, isc);
}

static inline void SYBT5_RLY_set_inverted(const bool inverted)
{
	PORTC_pin_set_inverted(7, inverted);
}

static inline void SYBT5_RLY_set_level(const bool level)
{
	PORTC_set_pin_level(7, level);
}

static inline void SYBT5_RLY_toggle_level()
{
	PORTC_toggle_pin_level(7);
}

static inline bool SYBT5_RLY_get_level()
{
	return PORTC_get_pin_level(7);
}
//-------------------------------------------------------------------------------------------------
static inline void LXvolts_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(0, pull_mode);
}

static inline void LXvolts_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(0, dir);
}

static inline void LXvolts_set_isc(const PORT_ISC_t isc)
{
	PORTD_pin_set_isc(0, isc);
}

static inline void LXvolts_set_inverted(const bool inverted)
{
	PORTD_pin_set_inverted(0, inverted);
}

static inline void LXvolts_set_level(const bool level)
{
	PORTD_set_pin_level(0, level);
}

static inline void LXvolts_toggle_level()
{
	PORTD_toggle_pin_level(0);
}

static inline bool LXvolts_get_level()
{
	return PORTD_get_pin_level(0);
}

static inline void Int_Temp_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(1, pull_mode);
}

static inline void Int_Temp_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(1, dir);
}

static inline void Int_Temp_set_isc(const PORT_ISC_t isc)
{
	PORTD_pin_set_isc(1, isc);
}

static inline void Int_Temp_set_inverted(const bool inverted)
{
	PORTD_pin_set_inverted(1, inverted);
}

static inline void Int_Temp_set_level(const bool level)
{
	PORTD_set_pin_level(1, level);
}

static inline void Int_Temp_toggle_level()
{
	PORTD_toggle_pin_level(1);
}

static inline bool Int_Temp_get_level()
{
	return PORTD_get_pin_level(1);
}
//----------------------------------------------------------------------------------------------------
static inline void SYBT5_Connected_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(2, pull_mode);
}

static inline void SYBT5_Connected_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(2, dir);
}

static inline void SYBT5_Connected_set_isc(const PORT_ISC_t isc)
{
	PORTD_pin_set_isc(2, isc);
}

static inline void SYBT5_Connected_set_inverted(const bool inverted)
{
	PORTD_pin_set_inverted(2, inverted);
}

static inline void SYBT5_Connected_set_level(const bool level)
{
	PORTD_set_pin_level(2, level);
}

static inline void SYBT5_Connected_toggle_level()
{
	PORTD_toggle_pin_level(2);
}

static inline bool SYBT5_Connected_get_level()
{
	return PORTD_get_pin_level(2);
}

static inline void TwentyFourVolt_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(3, pull_mode);
}

static inline void TwentyFourVolt_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(3, dir);
}

static inline void TwentyFourVolt_set_isc(const PORT_ISC_t isc)
{
	PORTD_pin_set_isc(3, isc);
}

static inline void TwentyFourVolt_set_inverted(const bool inverted)
{
	PORTD_pin_set_inverted(3, inverted);
}

static inline void TwentyFourVolt_set_level(const bool level)
{
	PORTD_set_pin_level(3, level);
}

static inline void TwentyFourVolt_toggle_level()
{
	PORTD_toggle_pin_level(3);
}

static inline bool TwentyFourVolt_get_level()
{
	return PORTD_get_pin_level(3);
}

static inline void SYBT_Resistor_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(4, pull_mode);
}

static inline void SYBT_Resistor_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(4, dir);
}

static inline void SYBT_Resistor_set_isc(const PORT_ISC_t isc)
{
	PORTD_pin_set_isc(4, isc);
}

static inline void SYBT_Resistor_set_inverted(const bool inverted)
{
	PORTD_pin_set_inverted(4, inverted);
}

static inline void SYBT_Resistor_set_level(const bool level)
{
	PORTD_set_pin_level(4, level);
}

static inline void SYBT_Resistor_toggle_level()
{
	PORTD_toggle_pin_level(4);
}

static inline bool SYBT_Resistor_get_level()
{
	return PORTD_get_pin_level(4);
}

static inline void SYBT_Temp_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(5, pull_mode);
}

static inline void SYBT_Temp_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(5, dir);
}

static inline void SYBT_Temp_set_isc(const PORT_ISC_t isc)
{
	PORTD_pin_set_isc(5, isc);
}

static inline void SYBT_Temp_set_inverted(const bool inverted)
{
	PORTD_pin_set_inverted(5, inverted);
}

static inline void SYBT_Temp_set_level(const bool level)
{
	PORTD_set_pin_level(5, level);
}

static inline void SYBT_Temp_toggle_level()
{
	PORTD_toggle_pin_level(5);
}

static inline bool SYBT_Temp_get_level()
{
	return PORTD_get_pin_level(5);
}

static inline void E3S_Sense_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(6, pull_mode);
}

static inline void E3S_Sense_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(6, dir);
}

static inline void E3S_Sense_set_isc(const PORT_ISC_t isc)
{
	PORTD_pin_set_isc(6, isc);
}

static inline void E3S_Sense_set_inverted(const bool inverted)
{
	PORTD_pin_set_inverted(6, inverted);
}

static inline void E3S_Sense_set_level(const bool level)
{
	PORTD_set_pin_level(6, level);
}

static inline void E3S_Sense_toggle_level()
{
	PORTD_toggle_pin_level(6);
}

static inline bool E3S_Sense_get_level()
{
	return PORTD_get_pin_level(6);
}

static inline void LED4_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTD_set_pin_pull_mode(7, pull_mode);
}

static inline void LED4_set_dir(const enum port_dir dir)
{
	PORTD_set_pin_dir(7, dir);
}

static inline void LED4_set_isc(const PORT_ISC_t isc)
{
	PORTD_pin_set_isc(7, isc);
}

static inline void LED4_set_inverted(const bool inverted)
{
	PORTD_pin_set_inverted(7, inverted);
}

static inline void LED4_set_level(const bool level)
{
	PORTD_set_pin_level(7, level);
}

static inline void LED4_toggle_level()
{
	PORTD_toggle_pin_level(7);
}

static inline bool LED4_get_level()
{
	return PORTD_get_pin_level(7);
}

static inline void SYBT_Volts_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTE_set_pin_pull_mode(0, pull_mode);
}

static inline void SYBT_Volts_set_dir(const enum port_dir dir)
{
	PORTE_set_pin_dir(0, dir);
}

static inline void SYBT_Volts_set_isc(const PORT_ISC_t isc)
{
	PORTE_pin_set_isc(0, isc);
}

static inline void SYBT_Volts_set_inverted(const bool inverted)
{
	PORTE_pin_set_inverted(0, inverted);
}

static inline void SYBT_Volts_set_level(const bool level)
{
	PORTE_set_pin_level(0, level);
}

static inline void SYBT_Volts_toggle_level()
{
	PORTE_toggle_pin_level(0);
}

static inline bool SYBT_Volts_get_level()
{
	return PORTE_get_pin_level(0);
}

static inline void Load_Test_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTE_set_pin_pull_mode(1, pull_mode);
}

static inline void Load_Test_set_dir(const enum port_dir dir)
{
	PORTE_set_pin_dir(1, dir);
}

static inline void Load_Test_set_isc(const PORT_ISC_t isc)
{
	PORTE_pin_set_isc(1, isc);
}

static inline void Load_Test_set_inverted(const bool inverted)
{
	PORTE_pin_set_inverted(1, inverted);
}

static inline void Load_Test_set_level(const bool level)
{
	PORTE_set_pin_level(1, level);
}

static inline void Load_Test_toggle_level()
{
	PORTE_toggle_pin_level(1);
}

static inline bool Load_Test_get_level()
{
	return PORTE_get_pin_level(1);
}

static inline void BlueLED_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTE_set_pin_pull_mode(2, pull_mode);
}

static inline void BlueLED_set_dir(const enum port_dir dir)
{
	PORTE_set_pin_dir(2, dir);
}

static inline void BlueLED_set_isc(const PORT_ISC_t isc)
{
	PORTE_pin_set_isc(2, isc);
}

static inline void BlueLED_set_inverted(const bool inverted)
{
	PORTE_pin_set_inverted(2, inverted);
}

static inline void BlueLED_set_level(const bool level)
{
	PORTE_set_pin_level(2, level);
}

static inline void BlueLED_toggle_level()
{
	PORTE_toggle_pin_level(2);
}

static inline bool BlueLED_get_level()
{
	return PORTE_get_pin_level(2);
}

static inline void Six_Volts_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTE_set_pin_pull_mode(3, pull_mode);
}

static inline void Six_Volts_set_dir(const enum port_dir dir)
{
	PORTE_set_pin_dir(3, dir);
}

static inline void Six_Volts_set_isc(const PORT_ISC_t isc)
{
	PORTE_pin_set_isc(3, isc);
}

static inline void Six_Volts_set_inverted(const bool inverted)
{
	PORTE_pin_set_inverted(3, inverted);
}

static inline void Six_Volts_set_level(const bool level)
{
	PORTE_set_pin_level(3, level);
}

static inline void Six_Volts_toggle_level()
{
	PORTE_toggle_pin_level(3);
}

static inline bool Six_Volts_get_level()
{
	return PORTE_get_pin_level(3);
}

static inline void SYBT5_Sense_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(0, pull_mode);
}

static inline void SYBT5_Sense_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(0, dir);
}

static inline void SYBT5_Sense_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(0, isc);
}

static inline void SYBT5_Sense_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(0, inverted);
}

static inline void SYBT5_Sense_set_level(const bool level)
{
	PORTF_set_pin_level(0, level);
}

static inline void SYBT5_Sense_toggle_level()
{
	PORTF_toggle_pin_level(0);
}

static inline bool SYBT5_Sense_get_level()
{
	return PORTF_get_pin_level(0);
}

static inline void YellowLED_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(1, pull_mode);
}

static inline void YellowLED_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(1, dir);
}

static inline void YellowLED_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(1, isc);
}

static inline void YellowLED_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(1, inverted);
}

static inline void YellowLED_set_level(const bool level)
{
	PORTF_set_pin_level(1, level);
}

static inline void YellowLED_toggle_level()
{
	PORTF_toggle_pin_level(1);
}

static inline bool YellowLED_get_level()
{
	return PORTF_get_pin_level(1);
}

static inline void HS_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(2, pull_mode);
}

static inline void HS_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(2, dir);
}

static inline void HS_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(2, isc);
}

static inline void HS_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(2, inverted);
}

static inline void HS_set_level(const bool level)
{
	PORTF_set_pin_level(2, level);
}

static inline void HS_toggle_level()
{
	PORTF_toggle_pin_level(2);
}

static inline bool HS_get_level()
{
	return PORTF_get_pin_level(2);
}

static inline void Main_Button_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(3, pull_mode);
}

static inline void Main_Button_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(3, dir);
}

static inline void Main_Button_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(3, isc);
}

/**
 * \brief Set Main_Button inverted mode
 *
 * Enable or disable inverted I/O on a pin
 *
 * \param[in] inverted true  = I/O on Main_Button is inverted
 *                     false = I/O on Main_Button is not inverted
 */
static inline void Main_Button_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(3, inverted);
}

static inline void Main_Button_set_level(const bool level)
{
	PORTF_set_pin_level(3, level);
}

static inline void Main_Button_toggle_level()
{
	PORTF_toggle_pin_level(3);
}

static inline bool Main_Button_get_level()
{
	return PORTF_get_pin_level(3);
}

static inline void Load_Relay_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(4, pull_mode);
}

static inline void Load_Relay_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(4, dir);
}

static inline void Load_Relay_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(4, isc);
}

static inline void Load_Relay_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(4, inverted);
}

static inline void Load_Relay_set_level(const bool level)
{
	PORTF_set_pin_level(4, level);
}

static inline void Load_Relay_toggle_level()
{
	PORTF_toggle_pin_level(4);
}

static inline bool Load_Relay_get_level()
{
	return PORTF_get_pin_level(4);
}

static inline void Load_IGBT_set_pull_mode(const enum port_pull_mode pull_mode)
{
	PORTF_set_pin_pull_mode(5, pull_mode);
}

static inline void Load_IGBT_set_dir(const enum port_dir dir)
{
	PORTF_set_pin_dir(5, dir);
}

static inline void Load_IGBT_set_isc(const PORT_ISC_t isc)
{
	PORTF_pin_set_isc(5, isc);
}

static inline void Load_IGBT_set_inverted(const bool inverted)
{
	PORTF_pin_set_inverted(5, inverted);
}

static inline void Load_IGBT_set_level(const bool level)
{
	PORTF_set_pin_level(5, level);
}

static inline void Load_IGBT_toggle_level()
{
	PORTF_toggle_pin_level(5);
}

static inline bool Load_IGBT_get_level()
{
	return PORTF_get_pin_level(5);
}

#endif /* ATMEL_START_PINS_H_INCLUDED */
