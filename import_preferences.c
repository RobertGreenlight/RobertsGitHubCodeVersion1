/*
 * import_preferences.c
 *
 * Created: 7/29/2024 9:04:20 AM
 *  Author: Robert F
 */ 

//-------------------------GETS STRING FROM BLUETOOTH---Not called unless there are characters to be read---------------------
#include "import_preferences.h"

#include "bobs_functions.h"
#include "Seven_Seg_LED.h"
#include <math.h>
#include <float.h>
#include "atmel_start.h"
#include <string.h>
#include <stdio.h>
#include "usart_basic.h"
#include <util/delay.h>
#include <eeprom_functions.h>
#include "wdt.h"
#include "ccp.h"
#include <nvmctrl_basic_example.h> 


void USART0_getString_OLD(void)  //
{
//char string [30];
//uint8_t numberChar = 0;
uint8_t buffer[35];
	
memset(buffer,0, 30*sizeof(uint8_t));		
for (int i = 0; i < 30; i++)
	{
	
	buffer[i] = USART_0_read();

	//if (USART_0_is_rx_ready()){
	//	buffer[i] = USART0.RXDATAL;
		if(buffer[i] == 0x0D || buffer[i] == 0x0A){       //  <- could be chR < ' '
		break;
		//numberChar = 0;
		}
	//}
	//else    i = i-1;


		
				//buffer[1] = USART_0_read();
				//while (!(USART1.STATUS & USART_RXCIF_bm)){
		
				//	if(buffer[i] == 0x0D || buffer[i] == 0x0A){       //  <- could be chR < ' '
				//	return;
				//numberChar = 0;
				//	}
				//	else if(numberChar < 20 && buffer[i] >= ' ' && buffer[i] < 0x7F){		//covers range of valid characters and length of string.
				//buffer[i] = USART1.RXDATAL;//}
				//    <- or use char* to make this faster with less code
				//	else{
				//i = 21;
				//	break;

				//}
	}
//	USART_0_disable_RX();
//	WDT_0_reset();
//	sprintf(string,"%s",buffer);
	//USART_0_enable_rx();
//	USART_1_enable_tx();
//	USART0_sendString(string);
//numberChar = 0;
identify_commands(buffer);

return;			
}

//-----------------------------GETS STRING FROM INTERNAL UART ------------------------------------
		// Check that "hello" was received on loopback RX.
		// Initialize rx buffer so strncmp() check will work
		//memset(rx, 0, sizeof(rx));
		//for (i = 0; i < strlen("hello"); i++) {
		//	rx[i] = USART_0_read(); // Blocks until character is available
		//}
//----------------------------------------------------------------------------
void USART1_getString(void){  // this works because I am disabling RX after read

	uint8_t buffer[20];
		
	for (int i = 1; i < 4; i++){
		if (USART_1_is_rx_ready()){
			buffer[i] = USART1.RXDATAL;
		}
		else    i = i-1;
		
			//buffer[1] = USART_0_read();
			//while (!(USART1.STATUS & USART_RXCIF_bm)){
		
			//	if(buffer[i] == 0x0D || buffer[i] == 0x0A){       //  <- could be chR < ' '
			//	return;
			//numberChar = 0;
			//	}
			//	else if(numberChar < 20 && buffer[i] >= ' ' && buffer[i] < 0x7F){		//covers range of valid characters and length of string.
			//buffer[i] = USART1.RXDATAL;//}
			//    <- or use char* to make this faster with less code
			//	else{
			//i = 21;
			//	break;

			//}
	}
	USART_1_disable_RX();
	
	if (buffer[1] == ASCII_C && buffer[2] == ASCII_P && buffer[3] == ASCII_Y){  //Looks for "CPY"
		LED4_set_level(false);
		//Transmit_Battery_Data();		
		USART0_sendString("TO BLUETOOTH ");
	}
	else LED4_set_level(true);
	if (buffer[1] == ASCII_R && buffer[2] == ASCII_L && buffer[3] == ASCII_Y){  //Looks for "RLY"
		Load_Relay_set_level(true);
	}
	else
	if (buffer[1] == ASCII_O && buffer[2] == ASCII_F && buffer[3] == ASCII_F){  //Looks for "OFF"
		Load_Relay_set_level(false);
	}
	else
	if (buffer[1] == ASCII_O && buffer[2] == ASCII_F && buffer[3] == ASCII_F){  //Looks for "OFF"
		Take_Measurements = true;
	}
	else
	if (buffer[1] == ASCII_O && buffer[2] == ASCII_F && buffer[3] == ASCII_F){  //Looks for "OFF"
		Take_Measurements = false;
	}
		
	volts_noload.ones = ASCII_charactermap(buffer[3]);
	volts_noload.tens = ASCII_charactermap(buffer[2]);
	volts_noload.hundreds = ASCII_charactermap(buffer[1]);
	
	
	USART_1_enable_rx(); // THIS HAS TO BE A FEW CLOCK CYCLES AWAY FROM DISABLE RX
	
	return;
}
//--------------------------------------------------------------------------------------------------------------------------------------
