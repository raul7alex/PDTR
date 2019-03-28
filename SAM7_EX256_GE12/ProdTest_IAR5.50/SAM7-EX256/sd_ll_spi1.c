
/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : sd_ll_spi1.h
 *    Description : SD/MMC low level SPI1 driver
 *
 *    History :
 *    1. Date        : April 10, 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 30123 $
 **************************************************************************/
#include "sd_ll_spi1.h"
#include "ioat91sam7x256.h"
#include "system.h"
#include "bits.h"
#include "adc.h"
#include "pwm.h"
#include "usart.h"
#include "test.h"

#define _SSP_FIFO_SIZE 8

/*************************************************************************
 * Function Name: SdPowerOn
 * Parameters: none
 * Return: none
 *
 * Description: Set power off state
 *
 *************************************************************************/
void SdPowerOn (void)
{

}
/*************************************************************************
 * Function Name: SdPowerOff
 * Parameters: none
 * Return: none
 *
 * Description: Set power off state
 *
 *************************************************************************/
void SdPowerOff (void)
{

}
/*************************************************************************
 * Function Name: SdChipSelect
 * Parameters: Boolean Select
 * Return: none
 *
 * Description: SD/MMC Chip select control
 * Select = true  - Chip is enable
 * Select = false - Chip is disable
 *
 *************************************************************************/
void SdChipSelect (Boolean Select)
{

}
/*************************************************************************
 * Function Name: SdPresent
 * Parameters: none
 * Return: Boolean - true cart present
 *                 - false cart no present
 *
 * Description: SD/MMC precent check
 *
 *************************************************************************/
Boolean SdPresent (void)
{
  return(((AT91C_BASE_PIOB->PIO_PDSR) & BIT23) == 0);
}

/*************************************************************************
 * Function Name: SdWriteProtect
 * Parameters: none
 * Return: Boolean - true cart is protected
 *                 - false cart no protected
 *
 * Description: SD/MMC Write protect check
 *
 *************************************************************************/
Boolean SdWriteProtect (void)
{
  return(((AT91C_BASE_PIOB->PIO_PDSR) & BIT22) != 0);
}

/*************************************************************************
 * Function Name: SdSetClockFreq
 * Parameters: Int32U Frequency
 * Return: Int32U
 *
 * Description: Set SPI ckl frequency
 *
 *************************************************************************/
Int32U SdSetClockFreq (Int32U Frequency)
{

  // done in LCD init

  // Return real frequency
  return(Frequency);
}

/*************************************************************************
 * Function Name: SdInit
 * Parameters: none
 * Return: none
 *
 * Description: Init SPI, Cart Present, Write Protect and Chip select pins
 *
 *************************************************************************/
void SdInit (void)
{
  /* Cart present */
  /* Write protect */
  AT91C_BASE_PIOB->PIO_ODR |= BIT22 | BIT23;   // make CP and WP inputs
  AT91C_BASE_PIOB->PIO_PER |= BIT22 | BIT23;

  // Spi init
  // all other settings are done in LCD init, no need to repeat them here

  AT91C_BASE_SPI0->SPI_MR      = 0x100D0011;            //Master mode, fixed select, disable decoder, FDIV=1 (MCK), PCS=1101
}

/*************************************************************************
 * Function Name: SdTranserByte
 * Parameters: Int8U ch
 * Return: Int8U
 *
 * Description: Read byte from SPI
 *
 *************************************************************************/
Int8U SdTranserByte (Int8U ch)
{
    unsigned int spib;

    while((AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_TXEMPTY) == 0);
    AT91C_BASE_SPI0->SPI_TDR = (ch & 0xFFFF);    // Send the data

    while((AT91C_BASE_SPI0->SPI_SR & AT91C_SPI_RDRF) == 0);      // Wait until the character can be sent
    spib = ((AT91C_BASE_SPI0->SPI_RDR) & 0xFFFF);                // Get the data received
    return spib;
}

/*************************************************************************
 * Function Name: SdSendBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: void
 *
 * Description: Read byte from SPI
 *
 *************************************************************************/
void SdSendBlock (pInt8U pData, Int32U Size)
{
  while (Size--)
  {
      SdTranserByte(*pData++);
  }
}

/*************************************************************************
 * Function Name: SdReceiveBlock
 * Parameters: pInt8U pData, Int32U Size
 *
 * Return: void
 *
 * Description: Read byte from SPI
 *
 *************************************************************************/
void SdReceiveBlock (pInt8U pData, Int32U Size)
{
  while (Size--)
  {
      *pData++ = SdTranserByte(0xFF);
  }
}

/*************************************************************************
 * Function Name: SdDly_1ms
 * Parameters: Int32U Delay
 * Return: none
 *
 * Description: Delay [msec]
 *
 *************************************************************************/
void SdDly_1ms (Int32U Delay)
{
volatile Int32U i;
  for(;Delay;--Delay)
  {
    for(i = SD_DLY_1MSEC;i;--i);
  }
}
