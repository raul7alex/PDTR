//adc.c
#include "adc.h"
#include "at91sam7x256.h"

void InitADC(void) {

  // Enable clock for interface
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_ADC;

  // Reset
  AT91C_BASE_ADC->ADC_CR = 0x1;
  AT91C_BASE_ADC->ADC_CR = 0x0;

  // Set maximum startup time and hold time
  AT91C_BASE_ADC->ADC_MR = 0x0F1F0F00;

}


unsigned int GetAdcChannel(unsigned char channel) {

  // variable
  unsigned int result;

  // Disable all but desired chanel
  AT91C_BASE_ADC->ADC_CHDR = ~channel;

  // Enable desired chanel
  AT91C_BASE_ADC->ADC_CHER = channel;

  // Start conversion
  AT91C_BASE_ADC->ADC_CR = 0x2;

  // wait for end of convertion
  while(!(AT91C_BASE_ADC->ADC_SR&channel));

  switch (channel) {

    case ADC_CHN_1: result = AT91C_BASE_ADC->ADC_CDR0; break;
    case ADC_CHN_2: result = AT91C_BASE_ADC->ADC_CDR1; break;
    case ADC_CHN_3: result = AT91C_BASE_ADC->ADC_CDR2; break;
    case ADC_CHN_4: result = AT91C_BASE_ADC->ADC_CDR3; break;
    case ADC_CHN_5: result = AT91C_BASE_ADC->ADC_CDR4; break;
    case ADC_CHN_6: result = AT91C_BASE_ADC->ADC_CDR5; break;
    case ADC_CHN_7: result = AT91C_BASE_ADC->ADC_CDR6; break;
    case ADC_CHN_8: result = AT91C_BASE_ADC->ADC_CDR7; break;
  }

  return result;

}
