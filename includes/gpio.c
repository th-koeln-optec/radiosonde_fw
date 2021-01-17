#include "gpio.h"

void gpio_init(void){
  gpio_rcc_init();
  gpio_spi1_init();
  gpio_radio_init();
  gpio_led_init();
}

void gpio_rcc_init(void){
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN_Msk;     //Enable/disable GPIO PORTA clk
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN_Msk;     //Enable/disable GPIO PORTB clk
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN_Msk;     //Enable/disable GPIO PORTC clk
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN_Msk;     //Enable/disable alternate function io clk
}

void gpio_spi1_init(void){
  //AFIO->MAPR &= ~(AFIO_MAPR_SPI1_REMAP_Msk);  //Switch between standard and remapped spi pinout
  
  //Pin output mode setup
  GPIO_SPI->CRL &= ~(GPIO_CRL_MODE_SPISCK_Msk | GPIO_CRL_MODE_SPIMOSI_Msk | GPIO_CRL_MODE_SPIMISO_Msk);
  GPIO_SPI->CRL |=(GPIO_CRL_MODE_OUT10 << GPIO_CRL_MODE_SPISCK_Pos) //Select pin directions
                | (GPIO_CRL_MODE_OUT10 << GPIO_CRL_MODE_SPIMOSI_Pos)
                | (GPIO_CRL_MODE_IN << GPIO_CRL_MODE_SPIMISO_Pos);
  //Pin function mode setup
  GPIO_SPI->CRL &= ~(GPIO_CRL_CNF_SPISCK_Msk | GPIO_CRL_CNF_SPIMOSI_Msk | GPIO_CRL_CNF_SPIMISO_Msk);
  GPIO_SPI->CRL |=(GPIO_CRL_CNF_OUT_AFPP << GPIO_CRL_CNF_SPISCK_Pos) //Select pin functions
                | (GPIO_CRL_CNF_OUT_AFPP << GPIO_CRL_CNF_SPIMOSI_Pos)
                | (GPIO_CRL_CNF_IN_FLOAT << GPIO_CRL_CNF_SPIMISO_Pos);
}

void gpio_radio_init(void){
  /*chip select*/
  GPIO_RADIO_CS->CRL &= ~(GPIO_CRL_MODE_RADIO_CS_Msk); //Clear prior to setup in next line.
  GPIO_RADIO_CS->CRL |= (GPIO_CRL_MODE_OUT10 << GPIO_CRL_MODE_RADIO_CS_Pos);
  GPIO_RADIO_CS->CRL &= ~(GPIO_CRL_CNF_RADIO_CS_Msk);
  GPIO_RADIO_CS->CRL |= (GPIO_CRL_CNF_OUT_PP << GPIO_CRL_CNF_RADIO_CS_Pos);
  GPIO_RADIO_CS->BSRR |= GPIO_BSRR_BS_RADIO_CS_Msk; //Radio chip select pin should be high as initial value, when cpol is idle high.
  /*shutdown*/
  GPIO_RADIO_SHDN->CRL &= ~(GPIO_CRL_MODE_RADIO_SHDN_Msk); //Clear prior to setup in next line.
  GPIO_RADIO_SHDN->CRL |= (GPIO_CRL_MODE_OUT10 << GPIO_CRL_MODE_RADIO_SHDN_Pos);
  GPIO_RADIO_SHDN->CRL &= ~(GPIO_CRL_CNF_RADIO_SHDN_Msk);
  GPIO_RADIO_SHDN->CRL |= (GPIO_CRL_CNF_OUT_PP << GPIO_CRL_CNF_RADIO_SHDN_Pos);
  GPIO_RADIO_SHDN->BSRR |= GPIO_BSRR_BS_RADIO_SHDN_Msk; //Radio shutdown pin should be high as initial value.
}

void gpio_led_init(void){
  GPIO_LED->BSRR |= GPIO_BSRR_BS_LEDGRN_Msk | GPIO_BSRR_BS_LEDRED_Msk; //Start with leds off

  GPIO_LED->CRL &= ~(GPIO_CRL_MODE_LEDGRN_Msk);
  GPIO_LED->CRL |= (GPIO_CRH_MODE_OUT2 << GPIO_CRL_MODE_LEDGRN_Pos);
  GPIO_LED->CRL &= ~(GPIO_CRL_CNF_LEDGRN_Msk);
  GPIO_LED->CRL |= (GPIO_CRL_CNF_OUT_PP << GPIO_CRL_CNF_LEDGRN_Pos);

  GPIO_LED->CRH &= ~(GPIO_CRH_MODE_LEDRED_Msk);
  GPIO_LED->CRH |= (GPIO_CRH_MODE_OUT2 << GPIO_CRH_MODE_LEDRED_Pos);
  GPIO_LED->CRH &= ~(GPIO_CRH_CNF_LEDRED_Msk);
  GPIO_LED->CRH |= (GPIO_CRH_CNF_OUT_PP << GPIO_CRH_CNF_LEDRED_Pos);
}