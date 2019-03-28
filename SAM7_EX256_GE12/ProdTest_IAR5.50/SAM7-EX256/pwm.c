#include "pwm.h"
#include "bits.h"
#include "ioat91sam7x256.h"


AT91PS_PWMC     w_pPWM      = AT91C_BASE_PWMC;
AT91PS_PMC      w_pPMC      = AT91C_BASE_PMC;
AT91PS_PWMC_CH  w_pPWM_CH0  = AT91C_BASE_PWMC_CH0;
AT91PS_PIO      w_pPioA     = AT91C_BASE_PIOA;
AT91PS_PIO      w_pPioB     = AT91C_BASE_PIOB;

void InitPWM(void) {

  // Enable clock for interface
  w_pPMC->PMC_PCER = 1 << AT91C_ID_PWMC;

  // Set second functionality of pin
  w_pPioB->PIO_PDR = BIT19;
  w_pPioB->PIO_ASR = BIT19;
  w_pPioB->PIO_BSR = 0;



  // Enable clock for PWM
  w_pPWM->PWMC_MR = 0x00010001;

  // Frequency for PWM

  // Period for PWM
  w_pPWM_CH0->PWMC_CPRDR = 1023;

  // Duty for PWM
  w_pPWM_CH0->PWMC_CDTYR = 512;

  // Modify the period at the next period start event.
  w_pPWM_CH0->PWMC_CMR = 0x0400;

  // Enable PWM chanel
  w_pPWM->PWMC_ENA = 1;

}

void SetPWMValue(unsigned int val) {

  // set new value
  w_pPWM_CH0->PWMC_CDTYR = val;
}



