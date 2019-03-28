/*----------------------------------------------------------------------------
*         ATMEL Microcontroller Software Support  -  ROUSSET  -
*----------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*----------------------------------------------------------------------------
* File Name           : Board.h
* Object              : AT91SAM7X Evaluation Board Features Definition File.
*
* Creation            : JG   20/Jun/2005
*----------------------------------------------------------------------------
*/
#ifndef Board_h
#define Board_h

#include "AT91SAM7X256.h"
//#include "ioat91sam7x256.h"

#define true	-1
#define false	0

/*-------------------------------*/
/* SAM7Board Memories Definition */
/*-------------------------------*/
// The AT91SAM7X128 embeds a 32-Kbyte SRAM bank, and 128K-Byte Flash

#define  FLASH_PAGE_NB		512
#define  FLASH_PAGE_SIZE	256

/*-----------------*/
/* Leds Definition */
/*-----------------*/
#define LED1            (1<<27)	// PB27
#define LED2            (1<<28)	// PB28
#define LED3            (1<<29)	// PB29
#define LED4            (1<<30)	// PB30
#define NB_LED			4

#define LED_MASK        (LED1|LED2|LED3|LED4)

/*---------------------*/
/* Joystick Definition */
/*---------------------*/

#define SW1_MASK        (1<<9)	// PA9 = UP
#define SW2_MASK        (1<<8)	// PA8 = DOWN
#define SW3_MASK        (1<<7)	// PA7 = LEFT
#define SW4_MASK        (1<<14)	// PA14 = RIGHT
#define SW_MASK         (SW1_MASK|SW2_MASK|SW3_MASK|SW4_MASK)


#define SW1 	(1<<9)	// PA9 = UP
#define SW2 	(1<<8)	// PA8 = DOWN
#define SW3 	(1<<7)	// PA7 = LEFT
#define SW4 	(1<<14)	// PA14 = RIGHT
#define SWPUSH	(1<<15) // PA15 = CENTER

/*-------------------------*/
/* Push Buttons Definition */
/*-------------------------*/

#define PB1_MASK        (1<<24)	// PB24 = LEFT
#define PB2_MASK        (1<<25)	// PB25 = RIGHT
#define PB_MASK         (PB1_MASK|PB2_MASK)

/*--------------*/
/* Master Clock */
/*--------------*/

#define EXT_OC          18432000   // Exetrnal ocilator MAINCK
#define MCK             47923200   // MCK (PLLRC div by 2)
#define MCKKHz          (MCK/1000) //

#endif /* Board_h */
