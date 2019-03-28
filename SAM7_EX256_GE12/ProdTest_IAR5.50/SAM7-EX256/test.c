#include "ioat91sam7x256.h"
#include "test.h"
#include "string.h"
#include "usart.h"
#include "system.h"
#include "adc.h"

#include "sd_ll_spi1.h"
#include "sd_spi_mode.h"
#include <stdio.h>

AT91PS_PIO      t_pPioA     = AT91C_BASE_PIOA;
AT91PS_PIO      t_pPioB     = AT91C_BASE_PIOB;
AT91PS_SYS      t_pSys      = AT91C_BASE_SYS;

extern char mmc_buffer[512];

unsigned int l = 0;
unsigned int ad4_value = 0;

char can_error  = 0;

char state_ext_vcc_porta = 0;
char state_ext_vcc_portb = 0;
char state_ext_gnd_porta = 0;
char state_ext_gnd_portb = 0;
char state_ext_pin_porta = 100;
char state_ext_pin_portb = 100;
char state_ext_vcc_uext  = 0;
char state_ext_gnd_uext  = 0;
char state_ext_pin_uext  = 100;
char state_ext_adc_pin   = 0;


const unsigned int mask_port_a  = 0x78000018&(~BIT3);   // pullup PA3
const unsigned int mask_port_b  = 0x78E40000;
const unsigned int mask_uext    = 0x01E00C60&(~BIT22);  // pullup PA22

//it's a simple delay
void DelayS (unsigned long a) { while (--a!=0); }

// stop if error
void Stop(void);

void TestBoard(void) {


  // Init Uart
  InitUSART0();

  write_str_USART0("\n\r\n\r\n\r\0");
  write_str_USART0("\n\r**** TEST EXTENSION BEGIN*****\n\r\0");

  write_str_USART0("\n\rPlease put extensions (EXT and UEXT). Press any key to continue\0");
  read_char_USART0();

  // just simple delay
  DelayS(500000);

  // GPIO init
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input
  t_pSys->PIOA_PPUDR  = 0xffffffff;   // Disable Pull-up resistor
  t_pSys->PIOB_PPUDR  = 0xffffffff;   // Disable Pull-up resistor

  // all as GPIO
  t_pPioA->PIO_PER = 0xffffffff;    //Disables the PIO from controlling the corresponding pin (enables peripheral control of the pin).
  t_pPioA->PIO_ASR = 0;             //Assigns the I/O line to the peripheral B function.
  t_pPioA->PIO_BSR = 0;
  t_pPioB->PIO_PER = 0xffffffff;    //Disables the PIO from controlling the corresponding pin (enables peripheral control of the pin).
  t_pPioB->PIO_ASR = 0;             //Assigns the I/O line to the peripheral B function.
  t_pPioB->PIO_BSR = 0;


  //===========================================================
  //***** TEST EXTENSIONS PORT *****
  ////// TEST EXT ////////
  // pull up is PA3

  //// Test for VCC /////---------------------------------------

  // all as input
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input

  // just simple delay
  DelayS(1000);

  // pull up
  t_pPioA->PIO_CODR   = BIT3; // low
  t_pPioA->PIO_OER    = BIT3; // output


  // just simple delay
  DelayS(1000);


  // check for high on port A
  if((t_pPioA->PIO_PDSR&mask_port_a) != 0x0) {
    state_ext_vcc_porta = 1;
    Stop();
  }

  // check for high on port B
  if((t_pPioB->PIO_PDSR&mask_port_b) != 0x0) {
    state_ext_vcc_portb = 1;
    Stop();
  }

  //// End test for VCC /////---------------------------------------

  //// Test for GND /////-----------------------------------

  // all as input
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input

  // just simple delay
  DelayS(1000);

  // pull up
  t_pPioA->PIO_SODR   = BIT3; // high
  t_pPioA->PIO_OER    = BIT3; // output

  // just simple delay
  DelayS(1000);

  // check for low on port A
  if((t_pPioA->PIO_PDSR|(~mask_port_a)) != 0xFFFFFFFF) {
    state_ext_gnd_porta = 1;
    Stop();
  }

  // check for low on port b
  if((t_pPioB->PIO_PDSR|(~mask_port_b)) != 0xFFFFFFFF) {
    state_ext_gnd_portb = 1;
    Stop();
  }

  //// End test for GND /////-----------------------------------

  ///// Running zero ///////-----------------------------------

  ///// for Port0

  // all as input
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input

  // just simple delay
  DelayS(1000);

  // pull up
  t_pPioA->PIO_SODR   = BIT3; // high
  t_pPioA->PIO_OER    = BIT3; // output

  // just simple delay
  DelayS(1000);

  // loop for port a
  for(l=0; l<32; l++) {

    // this port is not tested
    if(!((mask_port_a)&(1<<l))) continue;

    t_pPioA->PIO_SODR   = 0xffffffff;
    t_pPioA->PIO_CODR   = (1<<l);

    t_pPioA->PIO_ODR    = 0xffffffff;
    t_pPioA->PIO_OER    = (1<<l) | BIT3;

    // just simple delay
    DelayS(1000);

    // check for other zero at port A
    if((t_pPioA->PIO_PDSR|(~mask_port_a)) != ((0xFFFFFFFF)&(~(1<<l)))) {
      state_ext_pin_porta = l;
      Stop();
    }

    // check for other zero at port B
    if((t_pPioB->PIO_PDSR|(~mask_port_b)) != ((0xFFFFFFFF))) {
      state_ext_pin_portb = l;
      Stop();
    }

  }

  // all as input
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input

  // just simple delay
  DelayS(1000);

  // pull up
  t_pPioA->PIO_SODR   = BIT3; // high
  t_pPioA->PIO_OER    = BIT3; // output

  // just simple delay
  DelayS(1000);

  // loop for port a
  for(l=0; l<32; l++) {

    // this port is not tested
    if(!((mask_port_b)&(1<<l))) continue;

    t_pPioB->PIO_SODR   = 0xffffffff;
    t_pPioB->PIO_CODR   = (1<<l);

    t_pPioB->PIO_ODR    = 0xffffffff;
    t_pPioB->PIO_OER    = (1<<l);

    // just simple delay
    DelayS(1000);

    // check for other zero at port A
    if((t_pPioA->PIO_PDSR|(~mask_port_a)) != ((0xFFFFFFFF))) {
      state_ext_pin_porta = l;
      Stop();
    }

    // check for other zero at port B
    if((t_pPioB->PIO_PDSR|(~mask_port_b)) != ((0xFFFFFFFF)&(~(1<<l)))) {
      state_ext_pin_portb = l;
      Stop();
    }

  }


  ///// Running zero ///////-----------------------------------


  ////// TEST UEXT ////////
  // pull up is PA22

  //// Test for VCC /////---------------------------------------

  // all as input
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input

  // just simple delay
  DelayS(1000);

  // pull up
  t_pPioA->PIO_CODR   = BIT22; // low
  t_pPioA->PIO_OER    = BIT22; // output


  // just simple delay
  DelayS(1000);


  // check for high on port A
  if((t_pPioA->PIO_PDSR&mask_uext) != 0x0) {
    state_ext_vcc_uext = 1;
    Stop();
  }


  //// End test for VCC /////---------------------------------------

  //// Test for GND /////-----------------------------------

  // all as input
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input

  // just simple delay
  DelayS(1000);

  // pull up
  t_pPioA->PIO_SODR   = BIT22; // high
  t_pPioA->PIO_OER    = BIT22; // output

  // just simple delay
  DelayS(1000);

  // check for low on port A
  if((t_pPioA->PIO_PDSR|(~mask_uext)) != 0xFFFFFFFF) {
    state_ext_gnd_uext = 1;
    Stop();
  }


  //// End test for GND /////-----------------------------------

  ///// Running zero ///////-----------------------------------

  ///// for PortA

  // all as input
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input

  // just simple delay
  DelayS(1000);

  // pull up
  t_pPioA->PIO_SODR   = BIT22; // high
  t_pPioA->PIO_OER    = BIT22; // output

  // just simple delay
  DelayS(1000);

  // loop for port a
  for(l=0; l<32; l++) {

    // this port is not tested
    if(!((mask_uext)&(1<<l))) continue;

    t_pPioA->PIO_SODR   = 0xffffffff;
    t_pPioA->PIO_CODR   = (1<<l);

    t_pPioA->PIO_ODR    = 0xffffffff;
    t_pPioA->PIO_OER    = (1<<l) | BIT22;

    // just simple delay
    DelayS(1000);

    // check for other zero at port A
    if((t_pPioA->PIO_PDSR|(~mask_uext)) != ((0xFFFFFFFF)&(~(1<<l)))) {
      state_ext_pin_uext = l;
      Stop();
    }

  }

  // check ADC pin on extension

  // Init ADC
  InitADC();

  // just simple delay
  DelayS(1000);

  // get AD4 value
  ad4_value = GetAdcChanel(ADC_CHN_5);

  // voltage must be half from power suplay
  if((ad4_value<400)||(ad4_value>650)) { state_ext_adc_pin = 1; Stop(); }

  // Init Uart
  InitUSART0();

  write_str_USART0("\n\r**** TEST EXTENSION OK*****\n\r\0");
  write_str_USART0("\n\r\0");
  write_str_USART0("\n\r\0");
  write_str_USART0("\n\r**** TEST CAN BEGIN*****\n\r\0");
  write_str_USART0("\n\rPlease remove extension and put STNB/HS jumper at HS position! Press any key to continue\0");
  read_char_USART0();

  // all as input
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input

  // Set CANTX
  t_pPioA->PIO_SODR   = BIT20; // high
  t_pPioA->PIO_OER    = BIT20; // output

  if((t_pPioA->PIO_PDSR&BIT19)!=BIT19) { can_error=1; Stop(); }

  // Set CANTX
  t_pPioA->PIO_CODR   = BIT20; // high
  t_pPioA->PIO_OER    = BIT20; // output

  if((t_pPioA->PIO_PDSR&BIT19)!=0) { can_error=1; Stop(); }

  write_str_USART0("\n\rPlease put STNB/HS jumper at STNB position. Press any key to continue\0");
  read_char_USART0();

  // Set CANTX
  t_pPioA->PIO_SODR   = BIT20; // high
  t_pPioA->PIO_OER    = BIT20; // output

  if((t_pPioA->PIO_PDSR&BIT19)!=BIT19) { can_error=1; Stop(); }

  // Set CANTX
  t_pPioA->PIO_CODR   = BIT20; // high
  t_pPioA->PIO_OER    = BIT20; // output

  if((t_pPioA->PIO_PDSR&BIT19)==0) { can_error=1; Stop(); }


  write_str_USART0("\n\r**** CAN TEST OK *****\n\r\0");

  write_str_USART0("\n\r\0");
  write_str_USART0("\n\r\0");

  write_str_USART0("\n\r**** TEST CTS and DTR BEGIN *****\n\r\0");

  // PA3
  t_pPioA->PIO_SODR   = BIT3; // high
  t_pPioA->PIO_OER    = BIT3; // output

  write_str_USART0("\n\rConnect Terminal \0");
  write_str_USART0("\n\rToggle RTS, LCD light and CTS must toggle too. Try without CTS_E.\0");
  read_char_USART0();

  // PA4
  t_pPioA->PIO_ODR    = BIT4; // intput

  // LCD LIGHT
  t_pPioB->PIO_SODR  = BIT20; // high
  t_pPioB->PIO_OER   = BIT20; // output

  while(1) {

    if((t_pPioA->PIO_PDSR&BIT4)==0) {

      t_pPioB->PIO_SODR   = BIT20;  // high
      t_pPioA->PIO_SODR   = BIT3;   // high
    }
    else {

      t_pPioB->PIO_CODR   = BIT20;  // low
      t_pPioA->PIO_CODR   = BIT3;   // low
    }

  }



//*  write_str_USART0("\n\r**** PHY INTERFACE TEST CAN *****\n\r\0");
//*  write_str_USART0("\n\rPlease put PHY_PD jumper at 2-3 position. Press any key to continue\0");
//*  read_char_USART0();
//*
//*  write_str_USART0("\n\r\0");
//*  write_str_USART0("\n\r\0");
//*
//*  // all as input
//*  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
//*  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input
//*
//*  // Set PHY_PD
//*  t_pPioB->PIO_SODR   = BIT18; // high
//*  t_pPioB->PIO_OER    = BIT18; // output
//*
//*  // just simple delay
//*  DelayS(1000);
//*
//*  //read from lan chip
//*
//*
//*  // Clear PHY_PD
//*  t_pPioB->PIO_CODR   = BIT18; // low
//*
//*  // just simple delay
//*  DelayS(1000);
//*
//*  //read from lan chip
//*
//*  write_str_USART0("\n\r**** PHY INTERFACE TEST OK *****\n\r\0");
//*
//*  // just simple delay
//*  DelayS(1000);

}

void TestMMC(void) {

  pDiskCtrlBlk_t sdCardInfo;

  //TEST MMC
  write_str_USART0("\n\r***** TEST MMC BEGIN *****\n\r\0");

  //chek for CP
  while( !SdPresent() ) {
     write_str_USART0("Please put MMC card and press any key... \n\r\0");
     read_char_USART0();
  }
  //chek for WP
  while( SdWriteProtect() ) {
     write_str_USART0("Card is write protected. Please correct card state and press any key... \n\r\0");
     read_char_USART0();
  }


  /* PUT THE TEST HERE */
  SdDiskInit();
  sdCardInfo = SdGetDiskCtrlBkl();

  if(sdCardInfo->DiskStatus == DiskCommandPass) {
    char str[32];
    sprintf(str, " SD BLOCK SIZE: %dB \n\r\0", sdCardInfo->BlockSize);
    write_str_USART0((unsigned char*)str);
    sprintf(str, " SD CARD SIZE: %dB \n\r\0", sdCardInfo->BlockSize * sdCardInfo->BlockNumb);
    write_str_USART0((unsigned char*)str);
    write_str_USART0("**** TEST SD OK ****\n\r\0");
  }
  else {
    write_str_USART0("**** TEST SD FAILED ****\n\r\0");
  }
}

void Stop(void) {

  // GPIO init
  t_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  t_pPioB->PIO_ODR    = 0xffffffff;   // All as input
  t_pSys->PIOA_PPUDR  = 0xffffffff;   // Disable Pull-up resistor
  t_pSys->PIOB_PPUDR  = 0xffffffff;   // Disable Pull-up resistor

    // Freq init
  InitFrec();

  // Init Uart
  InitUSART0();

  // just simple delay
  DelayS(1000);

  if(state_ext_vcc_porta) {
    write_str_USART0("\n\rPin to VCC at Port A (EXT)\n\r\0");
  }

  if(state_ext_vcc_portb) {
    write_str_USART0("\n\rPin to VCC at Port B (EXT)\n\r\0");
  }

  if(state_ext_gnd_porta) {
    write_str_USART0("\n\rPin to GND at Port A (EXT)\n\r\0");
  }

  if(state_ext_gnd_portb) {
    write_str_USART0("\n\rPin to GND at Port B (EXT)\n\r\0");
  }

  if(state_ext_pin_porta != 100) {
    write_str_USART0("\n\rShort pin at Port A. Posible pin PA\0");
    write_char_USART0(state_ext_pin_porta/10 + 48);
    write_char_USART0(state_ext_pin_porta%10 + 48);
  }

  if(state_ext_pin_portb != 100) {
    write_str_USART0("\n\rShort pin at Port B. Posible pin PB\0");
    write_char_USART0(state_ext_pin_portb/10 + 48);
    write_char_USART0(state_ext_pin_portb%10 + 48);      }

  if(state_ext_vcc_uext) {
    write_str_USART0("\n\rPin at UEXT is to GND\n\r\0");
  }

  if(state_ext_gnd_uext) {
    write_str_USART0("\n\rPin at UEXT is to VCC\n\r\0");
  }

  if(state_ext_pin_uext!=100) {
    write_str_USART0("\n\rPin at UEXT is to VCC\n\r\0");
    write_char_USART0(state_ext_pin_uext/10 + 48);
    write_char_USART0(state_ext_pin_uext%10 + 48);
  }

  if(can_error) {
    write_str_USART0("\n\r CAN Error!!! \n\r\0");
  }

  if(state_ext_adc_pin) {
    write_str_USART0("\n\r ADC Error!!! \n\r\0");
  }

  // just simple delay
  DelayS(1000);

  while(1);
}
