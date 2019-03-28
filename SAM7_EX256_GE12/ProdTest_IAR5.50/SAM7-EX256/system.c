#include "system.h"
#include "ioat91sam7x256.h"
#include "bits.h"


AT91PS_PMC pPMC   = AT91C_BASE_PMC;

void Delay_s (unsigned long a) { while (--a!=0); }

//MAIN POINTER
AT91PS_PIO    p_pPioA  = AT91C_BASE_PIOA;
AT91PS_PIO    p_pPioB  = AT91C_BASE_PIOB;
AT91PS_PMC    p_pPMC   = AT91C_BASE_PMC;
AT91PS_USART  p_pUSART = AT91C_BASE_US0;
AT91PS_PDC    p_pPDC   = AT91C_BASE_PDC_US0;
AT91PS_MC     p_pMC    = AT91C_BASE_MC;
AT91PS_AIC    p_pAic   = AT91C_BASE_AIC;
AT91PS_SYS    p_pSys   = AT91C_BASE_SYS;

void InitFrec(void)
{

  // Set Flash Waite sate - Flash Memory Controler
  // if MCK = 47923200 I have 50 Cycle for 1 usecond
  AT91C_BASE_MC->MC_FMR = ((AT91C_MC_FMCN)&(48 <<16)) | AT91C_MC_FWS_1FWS ;

  // Watchdog Disable
  AT91C_BASE_WDTC->WDTC_WDMR= AT91C_WDTC_WDDIS;

  // Set MCK at 47 923 200
  // 1 Enabling the Main Oscillator:
  // SCK = 1/32768 = 30.51 uSecond
  // Start up time = 8 * 6 / SCK = 56 * 30.51 = 1,46484375 ms
  pPMC->PMC_MOR = (( AT91C_CKGR_OSCOUNT & (0x06 <<8) | AT91C_CKGR_MOSCEN ));

  // Wait the startup time
  while(!(pPMC->PMC_SR & AT91C_PMC_MOSCS));

  // 2 Checking the Main Oscillator Frequency (Optional)
  // 3 Setting PLL and divider:
  // - div by 5 Fin = 3,6864 =(18,432 / 5)
  // - Mul 25+1: Fout =	95,8464 =(3,6864 *26)
  // for 96 MHz the erroe is 0.16%
  // Field out NOT USED = 0
  // PLLCOUNT pll startup time estimate at : 0.844 ms
  // PLLCOUNT 28 = 0.000844 /(1/32768)
  pPMC->PMC_PLLR = ((AT91C_CKGR_DIV & 0x05) | (AT91C_CKGR_PLLCOUNT & (28<<8)) | (AT91C_CKGR_MUL & (25<<16)));

  // Wait the startup time
  while(!(pPMC->PMC_SR & AT91C_PMC_LOCK));
  while(!(pPMC->PMC_SR & AT91C_PMC_MCKRDY));

  // 4. Selection of Master Clock and Processor Clock
  // select the PLL clock divided by 2
  pPMC->PMC_MCKR =  AT91C_PMC_PRES_CLK_2 ;
  while(!(pPMC->PMC_SR & AT91C_PMC_MCKRDY));

  pPMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK  ;
  while(!(pPMC->PMC_SR & AT91C_PMC_MCKRDY));

}


void InitPeriphery(void) {


  //enable the clock of the PIO
  p_pPMC->PMC_PCER = 1 << AT91C_ID_PIOA;
  //enable the clock of the PIO
  p_pPMC->PMC_PCER = 1 << AT91C_ID_PIOB;


  // GPIO init
  p_pPioA->PIO_ODR    = 0xffffffff;   // All as input
  p_pPioB->PIO_ODR    = 0xffffffff;   // All as input
  p_pSys->PIOA_PPUDR  = 0xffffffff;   // Disable Pull-up resistor
  p_pSys->PIOB_PPUDR  = 0xffffffff;   // Disable Pull-up resistor


  // BUTTON SW1
  p_pPioB->PIO_ODR |= BIT24; //Configure in Input
  p_pPioB->PIO_PER |= BIT24; //Enable PB24

  // BUTTON SW2
  p_pPioB->PIO_ODR |= BIT25; //Configure in Input
  p_pPioB->PIO_PER |= BIT25; //Enable PB25


  /**** proba ****/
  //m_pPMC->PMC_PCER = 1 << AT91C_ID_PIOA;
  //m_pPio->PIO_PER = BIT0;    //Enable PA0
  //m_pPio->PIO_OER = BIT0;    //Configure in Output

  /**** LED BUTTONS ****/



  /*
  //LED 1
  //configure the PIO Lines corresponding to LED1
  p_pPio->PIO_PER |= BIT17;    //Enable PA17
  p_pPio->PIO_OER |= BIT17;    //Configure in Output
  p_pPio->PIO_SODR |= BIT17;   //set reg to 1

  Delay_s(1000);


  //LED 2
  //configure the PIO Lines corresponding to LED2
  p_pPio->PIO_PER |= BIT18;    //Enable PA18
  p_pPio->PIO_OER |= BIT18;    //Configure in Output
  p_pPio->PIO_SODR |= BIT18;   //set reg to 1

  Delay_s(1000);

  //BUTTON 1
  p_pPio->PIO_ODR |= BIT19; //Configure in Input
  p_pPio->PIO_PER |= BIT19; //Enable PA19

  Delay_s(1000);

  //BUTTON 2
  p_pPio->PIO_ODR |= BIT20; //Configure in Input
  p_pPio->PIO_PER |= BIT20; //Enable PA20
  */


  //LED 1
  //configure the PIO Lines corresponding to LED1
  //p_pPio->PIO_PER |= BIT17;    //Enable PA17
  //p_pPio->PIO_OER |= BIT17;    //Configure in Output
  //p_pPio->PIO_SODR |= BIT17;   //set reg to 1


  //LED 2
  //configure the PIO Lines corresponding to LED2
  //p_pPio->PIO_PER |= BIT18;    //Enable PA18
  //p_pPio->PIO_OER |= BIT18;    //Configure in Output
  //p_pPio->PIO_SODR |= BIT18;   //set reg to 1

  //Delay_s(1000);

  //BUTTON 1
  //p_pPio->PIO_ODR |= BIT19; //Configure in Input
  //p_pPio->PIO_PER |= BIT19; //Enable PA19

  //Delay_s(1000);

  //BUTTON 2
  //p_pPio->PIO_ODR |= BIT20; //Configure in Input
  //p_pPio->PIO_PER |= BIT20; //Enable PA19


/*

  p_pPio->PIO_SODR = BIT17 | BIT18;

  p_pPio->PIO_OER = BIT17 | BIT18;

  p_pPio->PIO_ODR = BIT19 | BIT20;

  p_pPio->PIO_PER = BIT17 | BIT18 | BIT19 | BIT20; //Enable PA20
*/
}

