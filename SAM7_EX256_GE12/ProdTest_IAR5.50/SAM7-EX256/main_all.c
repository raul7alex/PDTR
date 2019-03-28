#include "ioat91sam7x256.h"
#include "system.h"
#include "lcd.h"
#include "bits.h"
#include "adc.h"
#include "pwm.h"
#include "usart.h"
#include "test.h"


// DISPLAY 195x130

//MAIN POINTER
AT91PS_PIO    m_pPioA  = AT91C_BASE_PIOA;
AT91PS_PIO    m_pPioB  = AT91C_BASE_PIOB;
AT91PS_PMC    m_pPMC   = AT91C_BASE_PMC;
AT91PS_USART  m_pUSART = AT91C_BASE_US0;
AT91PS_PDC    m_pPDC   = AT91C_BASE_PDC_US0;
AT91PS_MC     m_pMC    = AT91C_BASE_MC;
AT91PS_AIC    m_pAic   = AT91C_BASE_AIC;
AT91PS_SYS    m_pSys   = AT91C_BASE_SYS;
AT91PS_ADC    m_pADC   = AT91C_BASE_ADC;
AT91PS_RSTC   m_pRSTC  = AT91C_BASE_RSTC;

// simple measure of temperature
void MeasureTemp(void);

//it's a simple delay
void Delay (unsigned long a) {

  volatile unsigned long d;

  d=a;
  while (--d!=0);
}

// loop variable
unsigned int a = 0;

// variable for echo
unsigned char ch = 0;

char c=0;

// sinus 1khz - variable
int sin[] = { 512, 579, 645, 708, 768, 824, 874, 918, 955, 985, 1007, 1020, 1023,
              1020, 1007, 985, 955, 918, 874, 824, 768, 708, 645, 579, 512, 445,
              379, 316, 256, 200, 150, 106, 69, 39, 17, 4, 0, 4, 17, 39, 69,
              106, 150, 200, 256, 316, 379, 445
};

unsigned int adc5_value = 0;

int main()
{


  //Enable RESET
  m_pRSTC->RSTC_RCR = 0xA5000008;
  m_pRSTC->RSTC_RMR = 0xA5000001;
  Delay(1000);

  // Freq init
  InitFrec();

  // Init periphery
  InitPeriphery();

  Delay(1000);

  // Init LCD
  InitLCD();

  Delay(1000);

  // Set settings
  LCDSettings();

  Delay(1000);

  // Load bitmap
  LCDWrite130x130bmp();

  // Init ADC
  InitADC();

  // Init Uart
  InitUSART0();

  // Init PWM
  InitPWM();


//      TestMMC();


  ////?????? REPOOOORT ??????////////
  // kak se testva!!!
  write_str_USART0("\n\r\tWelcome to Olimex SAM7-EX256 Development board\0");
  write_str_USART0("\n\r\t\tWWW.OLIMEX.COM/DEV\0");
  write_str_USART0("\n\r\0");
  write_str_USART0("\n\r\0");
  write_str_USART0("\n\r\0");
  write_str_USART0("\n\r\t 1. Check buttons \0");
  write_str_USART0("\n\r\t 2. Check joystick \0");
  write_str_USART0("\n\r\t 3. Check trimers TRIM and VOLUME\0");
  write_str_USART0("\n\r\t 4. Check UART - echo \0");
  write_str_USART0("\n\r\t 5. Press T to get temperature (very similar)\0");
  write_str_USART0("\n\r\t 6. Press M to test MMC card\0");
  write_str_USART0("\n\r\t 7. Press X to test extension port (only with extension)\0");
  write_str_USART0("\n\r\0");
  write_str_USART0("\n\r\0");
  write_str_USART0("\n\r\0");


  // loop forewer
  while(1) {

    // joystick
    if(!((m_pPioA->PIO_PDSR) & BIT7))       { if(++c>3) { c = 0; SetPWMValue(sin[a++]); a%=47; } continue; }
    else if(!((m_pPioA->PIO_PDSR) & BIT8))  { if(++c>4) { c = 0; SetPWMValue(sin[a++]); a%=47; } continue; }
    else if(!((m_pPioA->PIO_PDSR) & BIT9))  { if(++c>5) { c = 0; SetPWMValue(sin[a++]); a%=47; } continue; }
    else if(!((m_pPioA->PIO_PDSR) & BIT14)) { if(++c>6) { c = 0; SetPWMValue(sin[a++]); a%=47; } continue; }
    else if(!((m_pPioA->PIO_PDSR) & BIT15)) { if(++c>7) { c = 0; SetPWMValue(sin[a++]); a%=47; } continue; }

    // check button SW1
    if(!((m_pPioB->PIO_PDSR) & BIT24)) {

        // Backlight on
        Backlight(BKLGHT_LCD_ON);

        //Adc trim - set contrast
        SetContrast(GetAdcChanel(ADC_CHN_7)/28);
    }

    // check button SW2
    if(!((m_pPioB->PIO_PDSR) & BIT25)) {

        // Backlight off
        Backlight(BKLGHT_LCD_OFF);
    }

    // echo for uart
    ch = read_char_USART0_nonstop();
    if(ch != 0)
    {
      if(ch== 'X') { TestBoard(); continue; }
      if(ch== 'M') { TestMMC(); continue; }
      if(ch== 'T') { MeasureTemp(); continue; }
      write_char_USART0(ch);
      write_char_USART0('*');
      ch = 0;
    }

    // Get from microphone and set to speaker
    SetPWMValue(GetAdcChanel(ADC_CHN_8));

  }

}

// simple measure of temperature
void MeasureTemp(void) {

   unsigned int t=0;

   t = GetAdcChanel(ADC_CHN_6);

   t*=10;
   t/=19;

   write_str_USART0("\n\rTemperature: ");
   write_char_USART0(t/100+48);
   write_char_USART0((t%100)/10+48);
   write_char_USART0('.');
   write_char_USART0((t%100)%10+48);
   write_char_USART0(' ');
   write_char_USART0('C');

}
