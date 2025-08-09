/*
 * Seven_Seg_LED.h
 *
 * Created: 7/29/2024 9:11:33 AM
 *  Author: Robert F
 */ 

#ifndef SEVEN_SEG_LED_H_
#define SEVEN_SEG_LED_H_

#include <compiler.h>

#define Letter_a 95
#define Letter_b 124
#define Letter_b_dp 252
#define Letter_c 88
#define Letter_c_dp 216
#define Letter_d 94
#define Letter_E 121
#define Letter_E_dp 249
#define Letter_F 113
#define Letter_h 116
#define Letter_l 6
#define Letter_v 28
#define Letter_v_dp 156
#define Letter_L 56
#define Letter_o 92
#define Letter_P 115
#define Letter_S 109
#define Letter_t 120
#define Letter_y 110

#define C_10 13
#define DASH 64
#define SPACE 15
#define V_66  16
#define B_20 17
#define DP 128
#define LED_number_1 6
#define LED_number_2 91
#define LED_number_3 79
#define LED_number_4 102
#define LED_number_5 109
#define LED_number_6 125
#define LED_number_7 7
#define LED_number_8 127
#define LED_number_9 103
#define LED_number_0 63

#define LED_number_1_dp 134
#define LED_number_2_dp 219
#define LED_number_3_dp 207
#define LED_number_4_dp 230
#define LED_number_5_dp 109
#define LED_number_6_dp 237
#define LED_number_7_dp 135
#define LED_number_8_dp 255
#define LED_number_9_dp 103
#define LED_number_0_dp 191

#define ASCII_SPACE 32

#define ASCII_0_dp 33		//Actual ASCII_!
#define ASCII_1_dp 34		//Actual ASCII_"
#define ASCII_2_dp 35		//Actual ASCII_#
#define ASCII_3_dp 36		//Actual ASCII_$
#define	ASCII_4_dp 37		//Actual ASCII_%
#define ASCII_5_dp 38		//Actual ASCII_&
#define	ASCII_6_dp 39		//Actual ASCII_'
#define ASCII_7_dp 40		//ActualASCII_(
#define	ASCII_8_dp 41		//Actual ASCII_)
#define ASCII_9_dp 42		//Actual ASCII_*
//#define ASCII_+ 43
//#define ASCII_, 44
#define ASCII_DASH	45
#define ASCII_decimal	46
#define AddDecimalToASCII 15
//#define  ASCII_/	47
#define ASCII_0 48
#define ASCII_1 49
#define ASCII_2 50
#define ASCII_3 51
#define ASCII_4 52
#define ASCII_5 53
#define ASCII_6 54
#define ASCII_7 55
#define ASCII_8 56
#define ASCII_9 57
//#define ASCII_:	58
//#define ASCII_;	59
//#define ASCII_<	60
//#define ASCII_=	61
//#define ASCII_>	62
//#define ASCII_?	63
//#define ASCII_@	64
#define ASCII_A 65
#define ASCII_B 66
#define ASCII_C 67
#define ASCII_D 68
#define ASCII_E 69
#define ASCII_F 70
#define ASCII_G 71
#define ASCII_H 72
#define ASCII_I 73
#define ASCII_J 74
#define ASCII_K 75
#define ASCII_L 76
#define ASCII_M 77
#define ASCII_N 78
#define ASCII_O 79
#define ASCII_P 80
#define ASCII_Q 81
#define ASCII_R 82
#define ASCII_S 83
#define ASCII_T 84
#define ASCII_U 85
#define ASCII_V 86
#define ASCII_W 87
#define ASCII_X 88
#define ASCII_Y 89
#define ASCII_Z 90
#define ASCII_b_dp 91
#define ASCII_c_dp 92
#define ASCII_E_dp 93
#define ASCII_v_dp 94
#define ASCII__ 95
//#define ASCII_` 96
#define ASCII_a 97
#define ASCII_b 98
#define ASCII_c 99
#define ASCII_d 100
#define ASCII_e 101
#define ASCII_f 102
#define ASCII_g 103
#define ASCII_h 104
#define ASCII_i 105
#define ASCII_j 106
#define ASCII_k 107
#define ASCII_l 108
#define ASCII_m 109
#define ASCII_n 110
#define ASCII_o 111
#define ASCII_p 112
#define ASCII_q 113
#define ASCII_r 114
#define ASCII_s 115
#define ASCII_t 116
#define ASCII_u 117
#define ASCII_v 118
#define ASCII_w 119
#define ASCII_x 120
#define ASCII_y 121
#define ASCII_z 122
//#define ASCII_{	123
//#define ASCII_|	124
//#define ASCII_}	125
//#define ASCII_~	126
#define ASCII_DEL	127

// The 7seg displays define their segments as a, b, c, d, e, f, g, and dp  _a_
//                                                                       f|   |b
//                                                                        |_g_|
//                                                                       e|   |c
// Each bit of the uin8_t corresponds to a segment                        |_d_|.dp
// the following were used in early versions.
//void LED7SEGMENT_Write(uint8_t tens, uint8_t ones);
//void DUAL_LED7SEGMENT_Write(uint8_t thousands, uint8_t hundreds,uint8_t tens, uint8_t ones);
//void EFFICIENT_LED7SEGMENT_Write(int adcVal);
//void THREE_DIGIT_LED7SEGMENT_Write(int adcVal, int position);
//void THREE_DIGIT_SPI_Write(uint8_t ones,uint8_t tens,uint8_t hundreds,uint8_t row);
void THREE_DIGIT_SPI(uint8_t resistance,uint8_t temperature,uint8_t voltage,uint8_t row,bool BatteryLow);
uint8_t ASCII_charactermap(uint8_t from);				// converts ASCII characters to LED segments
uint8_t LED_charactermap(uint8_t from);					// converts numbers to LED segments
uint8_t ConvertToASCII(uint8_t from);					// converts numbers to ASCII characters



#endif /* SEVEN_SEG_LED_H_ */