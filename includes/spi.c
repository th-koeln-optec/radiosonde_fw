#include "spi.h"

void spi1_init(void){
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN_Msk; //Enable/disable spi1 peripheral clock
  SPI1->CR1 = 0x0000;                     //Reset the complete sp1 cr register (This is here so I can comment out all bit clear instructions)
  //SPI1->CR1 &= ~(SPI_CR1_BIDIMODE_Msk);   //Switch between one or two data lines mode
  //SPI1->CR1 &= ~(SPI_CR1_BIDIOE_Msk);     //Switch between tx only or rx only, if in one data line mode
  //SPI1->CR1 &= ~(SPI_CR1_CRCEN_Msk);      //Enable/disable hardware crc calculation
  //SPI1->CR1 &= ~(SPI_CR1_CRCNEXT_Msk);    //Switch between data- or crc-phase 
  SPI1->CR1 &= ~(SPI_CR1_DFF_Msk);        //Switch between 8- or 16-bit frame format
  //SPI1->CR1 &= ~(SPI_CR1_RXONLY_Msk);     //Switch between full duplex or rx only, if in two data line mode
  SPI1->CR1 |= SPI_CR1_SSM_Msk;           //Enable/disable spi slave select management by software
  SPI1->CR1 |= SPI_CR1_SSI_Msk;           //Sets/resets the internal slave select bit, independent of the physical state
  //SPI1->CR1 &= ~(SPI_CR1_LSBFIRST_Msk);   //Switch between lsb- or msb-first
  //SPE bit will be set as last instruction
  SPI1->CR1 |= (SPI_CR1_BR_FPCLK4 << SPI_CR1_BR_Pos);  //Set spi peripheral baud rate
  SPI1->CR1 |= SPI_CR1_MSTR_Msk;          //Switch between master and slave mode
  //SPI1->CR1 &= ~(SPI_CR1_CPOL_Msk);       //Switch between clk idle low or high
  //SPI1->CR1 &= ~(SPI_CR1_CPHA_Msk);       //Switch between data capture on first- or second-edge

  //SPI1->CR2 &=  ~(SPI_CR2_SSOE_Msk);      //Enable/disable spi nss output

  gpio_spi1_init();

  SPI1->CR1 |= SPI_CR1_SPE_Msk;           //Enable/disable spi peripheral
}

uint8_t spi_trx(uint8_t data){
  while(!(SPI1->SR & SPI_SR_TXE_Msk));
  SPI1->DR = data;
  while(!(SPI1->SR & SPI_SR_RXNE_Msk));
  return SPI1->DR;  //Read rx buffer every time, even when tx, to clear RXNE flag.
}