/*
 * seven_seg_led.c
 *
 * Created: 7/29/2024 9:00:29 AM
 *  Author: Robert F
 */ 

#include <driver_init.h>
#include <atmel_start_pins.h>
#include <seven_seg_led.h>
/*

*/
#ifndef ATTINY
/*
	void THREE_DIGIT_SPI(uint8_t resistance,uint8_t temperature,uint8_t voltage,uint8_t row,bool BatteryOK)
	{
		if(!BatteryOK){
			resistance = 0;
			temperature = 0;
		}
		while (!SPI_0_open(SPI_0_DEFAULT));
		PORTA_set_pin_level(7,false);
		SPI_0_exchange_byte(row);
		SPI_0_exchange_byte(voltage);
		SPI_0_exchange_byte(temperature);
		SPI_0_exchange_byte(resistance);
		PORTA_set_pin_level(7,true);
		SPI_0_close();
	}
*/	
#endif
//-----------------------------------------------------------------------------------------------------------------------
#ifndef ATTINY				// this is not needed ifdef ATTINY because all LED handling is done by the ATTINY
	uint8_t LED_charactermap(uint8_t from)
	{
		switch (from){
			case 1:
			return LED_number_1;
			case 2:
			return LED_number_2;
			case 3:
			return LED_number_3;
			case 4:
			return LED_number_4;
			case 5:
			return LED_number_5;
			case 6:
			return LED_number_6;
			case 7:
			return LED_number_7;
			case 8:
			return LED_number_8;
			case 9:
			return LED_number_9;
			case 0:
			return LED_number_0;
			case DP:
			return DP;
			case C_10:
			return 57;
			case DASH:
			return DASH;
			case SPACE:
			return 0;
			case Letter_c:
			return Letter_c;
		}
		return DASH;
	}
#endif
//-------------------------------------------------------------------------------------------------------
#ifndef ATTINY				// this is not needed ifdef ATTINY because all LED handling is done by the ATTINY
	uint8_t ASCII_charactermap(uint8_t from)
	{
		switch (from){
			case ASCII_0:
			return LED_number_0;
			case ASCII_1:
			return LED_number_1;
			case ASCII_2:
			return LED_number_2;
			case ASCII_3:
			return LED_number_3;
			case ASCII_4:
			return LED_number_4;
			case ASCII_5:
			return LED_number_5;
			case ASCII_6:
			return LED_number_6;
			case ASCII_7:
			return LED_number_7;
			case ASCII_8:
			return LED_number_8;
			case ASCII_9:
			return LED_number_9;
			case  ASCII_a:
			return Letter_a;
			case  ASCII_b:
			return Letter_b;
			case  ASCII_c:
			return Letter_c;
			case  ASCII_d:
			return Letter_d;
			case  ASCII_e:
			return Letter_h;
			case  ASCII_f:
			return Letter_h;
			case  ASCII_g:
			return Letter_h;
			case  ASCII_h:
			return Letter_h;
			case  ASCII_i:
			return Letter_h;
			case  ASCII_j:
			return Letter_h;
			case  ASCII_k:
			return Letter_h;
			case  ASCII_l:
			return Letter_h;
			case  ASCII_m:
			return Letter_h;
			case  ASCII_n:
			return Letter_h;
			case  ASCII_o:
			return Letter_o;
			case  ASCII_p:
			return Letter_h;
			case  ASCII_q:
			return Letter_h;
			case  ASCII_r:
			return Letter_h;
			case  ASCII_s:
			return Letter_h;
			case  ASCII_t:
			return Letter_h;
			case  ASCII_u:
			return Letter_v;
			case  ASCII_v:
			return Letter_v;
			case  ASCII_w:
			return Letter_h;
			case  ASCII_x:
			return Letter_h;
			case  ASCII_y:
			return Letter_h;
			case  ASCII_z:
			return Letter_h;
		}
		return DASH;
	}
#endif	
//--------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
#ifndef ATTINY				// this is not needed ifndef ATTINY because all LED handling is done by 
uint8_t ConvertToASCII(uint8_t from)
{
	switch (from){
		case 0:
		return ASCII_0;
		case 1:
		return ASCII_1;
		case 2:
		return ASCII_2;
		case 3:
		return ASCII_3;
		case 4:
		return ASCII_4;
		case 5:
		return ASCII_5;
		case 6:
		return ASCII_6;
		case 7:
		return ASCII_7;
		case 8:
		return ASCII_8;
		case 9:
		return ASCII_9;
		case Letter_b:
		return ASCII_b;
		case Letter_c:
		return ASCII_c;
		case Letter_v:
		return ASCII_v;	
		case DASH:
		return ASCII_DASH;
		case DP:
		return ASCII_decimal;		
	}
	return ASCII_DASH;
}
#endif

