#include "ioat91sam7x256.h"
#include "lcd.h"
#include "bmp.h"
#include "bits.h"

AT91PS_PIO    l_pPioA   = AT91C_BASE_PIOA;
AT91PS_PIO    l_pPioB   = AT91C_BASE_PIOB;
AT91PS_SPI    l_pSpi    = AT91C_BASE_SPI0;
AT91PS_PMC    l_pPMC    = AT91C_BASE_PMC;
AT91PS_PDC    l_pPDC    = AT91C_BASE_PDC_SPI0;


#define SPI_SR_TXEMPTY

#define LCD_RESET_LOW     l_pPioA->PIO_CODR   = BIT2
#define LCD_RESET_HIGH    l_pPioA->PIO_SODR   = BIT2

void Delaya (unsigned long a) { while (--a!=0); }

void Delay_ (unsigned long a) {

  volatile unsigned long d;
  d=a;
  while (--d!=0);
}

unsigned int i,j;

void InitLCD(void) {

  // Pin for backlight
  //l_pPioB->PIO_CODR   = BIT20;    // Set PB20 to LOW
  l_pPioB->PIO_SODR   = BIT20;    // Set PB20 to HIGH
  l_pPioB->PIO_OER    = BIT20;    // Configure PB20 as output

  // Reset pin
  l_pPioA->PIO_SODR   = BIT2;     // Set PA2 to HIGH
  l_pPioA->PIO_OER    = BIT2;     // Configure PA2 as output

  // CS pin
  //l_pPioA->PIO_SODR   = BIT12;     // Set PA2 to HIGH
  //l_pPioA->PIO_OER    = BIT12;     // Configure PA2 as output


  // Init SPI0
  //set functionality to pins:
  //port0.12 -> NPCS0
  //port0.16 -> MISO
  //port0.17 -> MOSI
  //port0.18 -> SPCK
  l_pPioA->PIO_PDR = BIT12 | BIT16 | BIT17 | BIT18 | BIT13;
  l_pPioA->PIO_ASR = BIT12 | BIT16 | BIT17 | BIT18 | BIT13;
  l_pPioA->PIO_BSR = 0;


  //enable the clock of SPI
  l_pPMC->PMC_PCER = 1 << AT91C_ID_SPI0;
  //l_pPMC->PMC_PCER = 1 << 5;

  // Fixed mode
  l_pSpi->SPI_CR      = 0x80;               //SPI Enable, Sowtware reset
  l_pSpi->SPI_CR      = 0x01;               //SPI Enable


  //l_pSpi->SPI_MR      = 0xE0099;           //Master mode, fixed select, disable decoder, FDIV=1 (NxMCK), PCS=1110, loopback
  //l_pSpi->SPI_MR      = 0xE0019;            //Master mode, fixed select, disable decoder, FDIV=1 (NxMCK), PCS=1110,
  //l_pSpi->SPI_MR      = 0xE0011;            //Master mode, fixed select, disable decoder, FDIV=0 (MCK), PCS=1110
  l_pSpi->SPI_MR      = 0x100E0011;            //Master mode, fixed select, disable decoder, FDIV=1 (MCK), PCS=1110

  //l_pSpi->SPI_CSR[0]  = 0x4A02;             //8bit, CPOL=0, ClockPhase=1, SCLK = 200kHz
  //l_pSpi->SPI_CSR[0]  = 0x4A13;             //9bit, CPOL=1, ClockPhase=1, SCLK = 200kHz
  //l_pSpi->SPI_CSR[0]  = 0x4A12;             //9bit, CPOL=0, ClockPhase=1, SCLK = 200kHz
  //l_pSpi->SPI_CSR[0]  = 0x4A11;             //9bit, CPOL=1, ClockPhase=0, SCLK = 200kHz
  //l_pSpi->SPI_CSR[0]  = 0x01011F11;           //9bit, CPOL=1, ClockPhase=0, SCLK = 48Mhz/32*31 = 48kHz
  // work l_pSpi->SPI_CSR[0]  = 0x01010F11;           //9bit, CPOL=1, ClockPhase=0, SCLK = 48Mhz/32*15 = 96kHz
  l_pSpi->SPI_CSR[0]  = 0x01010C11;           //9bit, CPOL=1, ClockPhase=0, SCLK = 48Mhz/32*12 = 125kHz
  l_pSpi->SPI_CSR[1]  = 0x01010502;

}


void WriteSpiCommand(unsigned int data){

  data = (data & 0xFEFF);

  // Wait for the transfer to complete
  while((l_pSpi->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

  l_pSpi->SPI_TDR = data;
}

void WriteSpiData(unsigned int data){

  data = (data | 0x0100);

  // Wait for the transfer to complete
  while((l_pSpi->SPI_SR & AT91C_SPI_TXEMPTY) == 0);

  l_pSpi->SPI_TDR = data;
}


void Backlight(unsigned char state) {

  if(state == BKLGHT_LCD_ON)
    l_pPioB->PIO_SODR   = BIT20;    // Set PB20 to HIGH
  else
    l_pPioB->PIO_CODR   = BIT20;    // Set PB20 to LOW

}

void SetContrast(unsigned char contrast) {

#ifdef GE12
  WriteSpiCommand(CONTRAST);
  WriteSpiData(0x20+contrast);    // contrast
#else
  WriteSpiCommand(VOLCTR);
  WriteSpiData(32+contrast); // contrast
  WriteSpiData(3); // contrast

#endif

}

void LCDSettings(void) {

#ifdef GE12
// Hardware reset 
  LCD_RESET_LOW; 
  Delay(10000);
  LCD_RESET_HIGH; 
  Delay(10000);
  
  // Sleep out (command0x11) 
  WriteSpiCommand(SLEEPOUT); 
  
  // Inversion on(command 0x20) 
  WriteSpiCommand(INVERSIONOFF);    // seems to be required for this controller 
  
  // Color Interface Pixel Format(command 0x3A) 
  WriteSpiCommand(PIXELFORMAT); 
  WriteSpiData(0x03);     // 0x03 =12bits-per-pixel 
    
  // Write contrast  (command 0x25) 
  WriteSpiCommand(CONTRAST); 
  WriteSpiData(0x40);     // contrast 0x38
  Delay(10000);
  
  // DisplayOn  (command 0x29) 
  WriteSpiCommand(DISPLAYON); 

  // 6. Set Normal mode (my)
  WriteSpiCommand(NORMALMODE);

  // 7. Inversion off
  WriteSpiCommand(INVERSIONOFF); // OFF?

  // 8. Column address set
  WriteSpiCommand(COLADDRSET);
  WriteSpiData(0);
  WriteSpiData(131);

  // 9. Page address set
  WriteSpiCommand(PAGEADDRSET);
  WriteSpiData(0);
  WriteSpiData(131);

  // 10. Memory access controler
  WriteSpiCommand(ACCESSCTRL);
  WriteSpiData(0x08);

  ///// Display setting 1 end  /////


  ///// Power supply  ///////

  // 1. Power control
  WriteSpiCommand(PWRCTRL);
  WriteSpiData(4);     // Internal resistance, V1OUT -> high power mode, oscilator devision rate

  // 2. Sleep out
  WriteSpiCommand(SLEEPOUT);


  // 3. Voltage control - voltage control and write contrast define LCD electronic volume
  WriteSpiCommand(VOLTCTRL);
  //WriteSpiData(0x7f);    //  full voltage control
  //WriteSpiData(0x03);    //  must be "1"

  Delaya(2000);

  // 5. Temperature gradient
  WriteSpiCommand(TEMPGRADIENT);
  for(i=0; i<14; i++) {
    WriteSpiData(0);
  }

  // 6. Booster voltage ON
  WriteSpiCommand(BOOSTVON);


  // Finally - Display On
  WriteSpiCommand(DISPLAYON);	

#else


  // Hardware reset
  LCD_RESET_LOW;
  Delay_(1000);
  LCD_RESET_HIGH;
  Delay_(1000);

  // Display vontrol
  WriteSpiCommand(DISCTL);
//  WriteSpiData(0x03); // no division
//  WriteSpiData(0x23); // 160 line
//  WriteSpiData(0x02); // 2 highlighte line
  WriteSpiData(0x00); // default
  WriteSpiData(0x20); // (32 + 1) * 4 = 132 lines (of which 130 are visible)
  WriteSpiData(0x0a); // default

  // COM scan
  WriteSpiCommand(COMSCN);
  WriteSpiData(0x00);  // Scan 1-80

  // Internal oscilator ON
  WriteSpiCommand(OSCON);

  // wait aproximetly 100ms
  Delay_(10000);

  // Sleep out
  WriteSpiCommand(SLPOUT);

  // Voltage control
  WriteSpiCommand(VOLCTR);
  WriteSpiData(43); // middle value of V1
  WriteSpiData(0x03); // middle value of resistance value

  // Temperature gradient
  WriteSpiCommand(TMPGRD);
  WriteSpiData(0x00); // default

  // Power control
  WriteSpiCommand(PWRCTR);
  WriteSpiData(0x0f);   // referance voltage regulator on, circuit voltage follower on, BOOST ON

  // Normal display
  WriteSpiCommand(DISNOR);

  // Inverse display
  WriteSpiCommand(DISINV);

  // Partial area off
  WriteSpiCommand(PTLOUT);

//  // Scroll area set
//  WriteSpiCommand(ASCSET);
//  WriteSpiData(0);
//  WriteSpiData(0);
//  WriteSpiData(40);
//  WriteSpiData(3);

//  // Vertical scrool address start
//  WriteSpiCommand(SCSTART);
//  WriteSpiData(0);


  // Data control
  WriteSpiCommand(DATCTL);
  WriteSpiData(0x00); // all inversions off, column direction
  WriteSpiData(0x03); // RGB sequence
  WriteSpiData(0x02); // Grayscale -> 16

  // Page Address set
  WriteSpiCommand(PASET);
  WriteSpiData(0);
  WriteSpiData(131);

  // Page Column set
  WriteSpiCommand(CASET);
  WriteSpiData(0);
  WriteSpiData(131);

#endif



}


void LCDWrite130x130bmp(void) {

#ifdef GE12

  // Display OFF
  WriteSpiCommand(DISPLAYOFF);

    // WRITE MEMORY
  WriteSpiCommand(MEMWRITE);


  for(j=0; j<sizeof(bmp); j++) {
    WriteSpiData(bmp[j]);
  }


  // Display On
  WriteSpiCommand(DISPLAYON);	

#else

  // Display OFF
  // WriteSpiCommand(DISOFF);

  // WRITE MEMORY
  WriteSpiCommand(RAMWR);

  //for(j=0; j<sizeof(bmp)-396; j++) {
  for(j=0; j<25740; j++) {
    WriteSpiData(bmp[j]);
    //WriteSpiData(0xFF);
  }

  // wait aproximetly 100ms
  Delay_(10000);

  // Display On
  WriteSpiCommand(DISON);	

#endif

}
