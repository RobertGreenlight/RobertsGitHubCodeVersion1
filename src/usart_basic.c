/**
 * \file
 *
 * \brief USART basic driver.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \defgroup doc_driver_usart_basic USART Basic
 * \ingroup doc_driver_usart
 *
 * \section doc_driver_usart_basic_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <compiler.h>
#include <clock_config.h>
#include <usart_basic.h>
#include <atomic.h>

#include <stdio.h>

#if defined(__GNUC__)

int USART_0_printCHAR(char character, FILE *stream)
{
	USART_0_write(character);
	return 0;
}

FILE USART_0_stream = FDEV_SETUP_STREAM(USART_0_printCHAR, NULL, _FDEV_SETUP_WRITE);

#elif defined(__ICCAVR__)

int putchar(int outChar)
{
	USART_0_write(outChar);
	return outChar;
}
#endif

/**
 * \brief Initialize USART interface
 * If module is configured to disabled state, the clock to the USART is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 * \retval 0 the USART init was successful
 * \retval 1 the USART init was not successful
 */
int8_t USART_0_init()
{

	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(38400); /* set baud rate register 38400  a few legacy models are at 9600*/

	// USART0.CTRLA = 0 << USART_ABEIE_bp /* Auto-baud Error Interrupt Enable: disabled */
	//		 | 0 << USART_DREIE_bp /* Data Register Empty Interrupt Enable: disabled */
	//		 | 0 << USART_LBME_bp /* Loop-back Mode Enable: disabled */
	//		 | USART_RS485_DISABLE_gc /* RS485 Mode disabled */
	//		 | 0 << USART_RXCIE_bp /* Receive Complete Interrupt Enable: disabled */
	//		 | 0 << USART_RXSIE_bp /* Receiver Start Frame Interrupt Enable: disabled */
	//		 | 0 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */

	USART0.CTRLB = 0 << USART_MPCM_bp       /* Multi-processor Communication Mode: disabled */
	               | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
	               | 1 << USART_RXEN_bp     /* Receiver Enable: enabled */
	               | USART_RXMODE_NORMAL_gc /* Normal mode */
	               | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
	               | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

	// USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc /* Asynchronous Mode */
	//		 | USART_CHSIZE_8BIT_gc /* Character size: 8 bit */
	//		 | USART_PMODE_DISABLED_gc /* No Parity */
	//		 | USART_SBMODE_1BIT_gc; /* 1 stop bit */

	// USART0.DBGCTRL = 0 << USART_DBGRUN_bp; /* Debug Run: disabled */

	// USART0.EVCTRL = 0 << USART_IREI_bp; /* IrDA Event Input Enable: disabled */

	// USART0.RXPLCTRL = 0x0 << USART_RXPL_gp; /* Receiver Pulse Length: 0x0 */

	// USART0.TXPLCTRL = 0x0 << USART_TXPL_gp; /* Transmit pulse length: 0x0 */

#if defined(__GNUC__)
	stdout = &USART_0_stream;
#endif

	return 0;
}

void USART_0_enable()
{
	USART0.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}

void USART_0_enable_rx()
{
	USART0.CTRLB |= USART_RXEN_bm;
}

void USART_0_enable_tx()
{
	USART0.CTRLB |= USART_TXEN_bm;
}

void USART_0_disable()
{
	USART0.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
}

uint8_t USART_0_get_data()
{
	return USART0.RXDATAL;
}

bool USART_0_is_tx_ready()
{
	return (USART0.STATUS & USART_DREIF_bm);
}

bool USART_0_is_rx_ready()
{
	return (USART0.STATUS & USART_RXCIF_bm);
}

bool USART_0_is_tx_busy()
{
	return (!(USART0.STATUS & USART_TXCIF_bm));
}

uint8_t USART_0_read()
{
	while (!(USART0.STATUS & USART_RXCIF_bm))
		;
	return USART0.RXDATAL;
}

void USART_0_write(const uint8_t data)
{
	while (!(USART0.STATUS & USART_DREIF_bm))
		;
	USART0.TXDATAL = data;
}

int8_t USART_1_init()
{

	USART1.BAUD = (uint16_t)USART1_BAUD_RATE(2400); /* set baud rate register */

	// USART1.CTRLA = 0 << USART_ABEIE_bp /* Auto-baud Error Interrupt Enable: disabled */
	//		 | 0 << USART_DREIE_bp /* Data Register Empty Interrupt Enable: disabled */
	//		 | 0 << USART_LBME_bp /* Loop-back Mode Enable: disabled */
	//		 | USART_RS485_DISABLE_gc /* RS485 Mode disabled */
	//		 | 0 << USART_RXCIE_bp /* Receive Complete Interrupt Enable: disabled */
	//		 | 0 << USART_RXSIE_bp /* Receiver Start Frame Interrupt Enable: disabled */
	//		 | 0 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */

	USART1.CTRLB = 0 << USART_MPCM_bp       /* Multi-processor Communication Mode: disabled */
	               | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
	               | 1 << USART_RXEN_bp     /* Receiver Enable: enabled */
	               | USART_RXMODE_NORMAL_gc /* Normal mode */
	               | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
	               | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

	// USART1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc /* Asynchronous Mode */
	//		 | USART_CHSIZE_8BIT_gc /* Character size: 8 bit */
	//		 | USART_PMODE_DISABLED_gc /* No Parity */
	//		 | USART_SBMODE_1BIT_gc; /* 1 stop bit */

	// USART1.DBGCTRL = 0 << USART_DBGRUN_bp; /* Debug Run: disabled */

	// USART1.EVCTRL = 0 << USART_IREI_bp; /* IrDA Event Input Enable: disabled */

	// USART1.RXPLCTRL = 0x0 << USART_RXPL_gp; /* Receiver Pulse Length: 0x0 */

	// USART1.TXPLCTRL = 0x0 << USART_TXPL_gp; /* Transmit pulse length: 0x0 */

	return 0;
}

void USART_1_enable()
{
	USART1.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}

void USART_1_enable_rx()
{
	USART1.CTRLB |= USART_RXEN_bm;
}

void USART_1_enable_tx()
{
	USART1.CTRLB |= USART_TXEN_bm;
}

void USART_1_disable()
{
	USART1.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
}

uint8_t USART_1_get_data()
{
	return USART1.RXDATAL;
}

bool USART_1_is_tx_ready()
{
	return (USART1.STATUS & USART_DREIF_bm);
}

bool USART_1_is_rx_ready()
{
	return (USART1.STATUS & USART_RXCIF_bm);
}

bool USART_1_is_tx_busy()
{
	return (!(USART1.STATUS & USART_TXCIF_bm));
}

uint8_t USART_1_read()
{
	while (!(USART1.STATUS & USART_RXCIF_bm))
		;
	return USART1.RXDATAL;
}

void USART_1_write(const uint8_t data)
{
	while (!(USART1.STATUS & USART_DREIF_bm))
		;
	USART1.TXDATAL = data;
}

int8_t USART_3_init()
{

	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(20000); /* set baud rate register */

	// USART3.CTRLA = 0 << USART_ABEIE_bp /* Auto-baud Error Interrupt Enable: disabled */
	//		 | 0 << USART_DREIE_bp /* Data Register Empty Interrupt Enable: disabled */
	//		 | 0 << USART_LBME_bp /* Loop-back Mode Enable: disabled */
	//		 | USART_RS485_DISABLE_gc /* RS485 Mode disabled */
	//		 | 0 << USART_RXCIE_bp /* Receive Complete Interrupt Enable: disabled */
	//		 | 0 << USART_RXSIE_bp /* Receiver Start Frame Interrupt Enable: disabled */
	//		 | 0 << USART_TXCIE_bp; /* Transmit Complete Interrupt Enable: disabled */

	USART3.CTRLB = 0 << USART_MPCM_bp       /* Multi-processor Communication Mode: disabled */
	               | 0 << USART_ODME_bp     /* Open Drain Mode Enable: disabled */
	               | 1 << USART_RXEN_bp     /* Receiver Enable: enabled */
	               | USART_RXMODE_NORMAL_gc /* Normal mode */
	               | 0 << USART_SFDEN_bp    /* Start Frame Detection Enable: disabled */
	               | 1 << USART_TXEN_bp;    /* Transmitter Enable: enabled */

	// USART3.CTRLC = USART_CMODE_ASYNCHRONOUS_gc /* Asynchronous Mode */
	//		 | USART_CHSIZE_8BIT_gc /* Character size: 8 bit */
	//		 | USART_PMODE_DISABLED_gc /* No Parity */
	//		 | USART_SBMODE_1BIT_gc; /* 1 stop bit */

	// USART3.DBGCTRL = 0 << USART_DBGRUN_bp; /* Debug Run: disabled */

	// USART3.EVCTRL = 0 << USART_IREI_bp; /* IrDA Event Input Enable: disabled */

	// USART3.RXPLCTRL = 0x0 << USART_RXPL_gp; /* Receiver Pulse Length: 0x0 */

	// USART3.TXPLCTRL = 0x0 << USART_TXPL_gp; /* Transmit pulse length: 0x0 */

	return 0;
}

void USART_3_enable()
{
	USART3.CTRLB |= USART_RXEN_bm | USART_TXEN_bm;
}

void USART_3_enable_rx()
{
	USART3.CTRLB |= USART_RXEN_bm;
}

void USART_3_enable_tx()
{
	USART3.CTRLB |= USART_TXEN_bm;
}

void USART_3_disable()
{
	USART3.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
}

uint8_t USART_3_get_data()
{
	return USART3.RXDATAL;
}

bool USART_3_is_tx_ready()
{
	return (USART3.STATUS & USART_DREIF_bm);
}

bool USART_3_is_rx_ready()
{
	return (USART3.STATUS & USART_RXCIF_bm);
}

bool USART_3_is_tx_busy()
{
	return (!(USART3.STATUS & USART_TXCIF_bm));
}

uint8_t USART_3_read()
{
	while (!(USART3.STATUS & USART_RXCIF_bm))
		;
	return USART3.RXDATAL;
}



void USART_3_write(const uint8_t data)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
		;
	USART3.TXDATAL = data;
}
