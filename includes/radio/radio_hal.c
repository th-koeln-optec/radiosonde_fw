/// @file radio_hal.c
/*
 * Filename:            radio_hal.c
 * Description:         This file contains RADIO HAL.
********************************************************************************
 * CHANGE LOG
 * Version  Author
 * v1.00    SiLabs
 *      Copyright 2011 Silicon Laboratories, Inc.
 * vX.XX    M. Malyska
 *      Massive changes where implemented by the author, the original file can be found in the same directory: radio_hal.c.bak
 */
#include "radio_hal.h"

/**
 * Sets the radio-ic shutdown pin to logic high in order to enable shutdown state.
 * @param none
 * @return none
 */
void radio_hal_AssertShutdown(void){
  GPIO_RADIO_SHDN->BSRR |= GPIO_BSRR_BS_RADIO_SHDN_Msk;
}

/**
 * Sets the radio ic shutdown pin to logic low in order to disable shutdown state.
 * @param none
 * @return none
 */
void radio_hal_DeassertShutdown(void){
  GPIO_RADIO_SHDN->BSRR |= GPIO_BSRR_BR_RADIO_SHDN_Msk;
}

/**
 * Sets the radio-ic chip select pin to logic low in order to enable the radio spi interface.
 * @param none
 * @return none
 */
void radio_hal_ClearNsel(void){
  GPIO_RADIO_CS->BSRR |= GPIO_BSRR_BR_RADIO_CS_Msk;
}

/**
 * Sets the radio-ic chip select pin to logic high in order to disable the radio spi interface.
 * @param none
 * @return none
 */
void radio_hal_SetNsel(void){
  GPIO_RADIO_CS->BSRR |= GPIO_BSRR_BS_RADIO_CS_Msk;
}

/*
uint8_t radio_hal_NirqLevel(void)
{
    return RF_NIRQ;
}*/

/**
 * Transmits one byte of data over spi.
 * @param byte_to_write The one data byte that shall be tranmsmitted.
 * @return none
 */
void radio_hal_SpiWriteByte(uint8_t byte_to_write){
  spi_trx(byte_to_write);
}

/**
 * Reads one bit of data over spi.
 * @param none
 * @return Returns the one byte of data that was read.
 */
uint8_t radio_hal_SpiReadByte(void){
  return spi_trx(0x00); //Write 0x00 during read operation so that spi clock pulses will still be generated.
}

/**
 * Transmits one or more byte of data over spi.
 * @param byte_cout The number of bytes that shall be transmitted over spi.
 * @param pData Pointer to an array of bytes that sahll be transmitted.
 * @return none
 */
void radio_hal_SpiWriteData(uint8_t byte_count, uint8_t* pData){
  for(uint8_t i = 0; i < byte_count; i++){
    spi_trx(pData[i]); 
  }
}

/**
 * Reads one or more byte of data over spi.
 * @param byte_cout The number of bytes that shall be read over spi.
 * @param pData Pointer to an array of bytes where read data shall be written.
 * @return none
 */
void radio_hal_SpiReadData(uint8_t byte_count, uint8_t* pData){
  for(uint8_t i = 0; i < byte_count; i++){
    pData[i] = spi_trx(0x00); //Write 0x00 during read operation so that spi clock pulses will still be generated.
  }
}

#ifdef RADIO_DRIVER_EXTENDED_SUPPORT

#ifdef GPIO_RADIO_GPIO0_USED
/**
 * Reads radio-ic gpio pin 0.
 * @param none
 * @return Returns the value of the gpio pin.
 */
uint8_t radio_hal_Gpio0Level(void){
  return (GPIO_RADIO_GPIO0->IDR & GPIO_IDR_RADIO_GPIO0_Msk);
}
#endif //#ifdef GPIO_RADIO_GPIO0_USED

#ifdef GPIO_RADIO_GPIO1_USED
/**
 * Reads radio-ic gpio pin 1.
 * @param none
 * @return Returns the value of the gpio pin.
 */
uint8_t radio_hal_Gpio1Level(void){
  return (GPIO_RADIO_GPIO1->IDR & GPIO_IDR_RADIO_GPIO1_Msk);
}
#endif //#ifdef GPIO_RADIO_GPIO1_USED

#ifdef GPIO_RADIO_GPIO2_USED
/**
 * Reads radio-ic gpio pin 2.
 * @param none
 * @return Returns the value of the gpio pin.
 */
uint8_t radio_hal_Gpio2Level(void){
  return (GPIO_RADIO_GPIO2->IDR & GPIO_IDR_RADIO_GPIO2_Msk);
}
#endif //#ifdef GPIO_RADIO_GPIO2_USED

#ifdef GPIO_RADIO_GPIO3_USED
/**
 * Reads radio-ic gpio pin 3.
 * @param none
 * @return Returns the value of the gpio pin.
 */
uint8_t radio_hal_Gpio3Level(void){
  return (GPIO_RADIO_GPIO3->IDR & GPIO_IDR_RADIO_GPIO3_Msk);
}
#endif //#ifdef GPIO_RADIO_GPIO3_USED

#endif //#ifdef RADIO_DRIVER_EXTENDED_SUPPORT