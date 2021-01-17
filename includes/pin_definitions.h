#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>
    
/****DEFINES*/

/*LED*/
#define GPIO_LED GPIOC //!< This define returns the port base address for the leds.
/*led green*/
#define GPIO_CRL_MODE_LEDGRN_Pos GPIO_CRL_MODE7_Pos //!< This define returns the register position for led green
#define GPIO_CRL_MODE_LEDGRN_Msk GPIO_CRL_MODE7_Msk //!< This define returns the register mask for led green
#define GPIO_CRL_CNF_LEDGRN_Msk GPIO_CRL_CNF7_Msk //!< This define returns the register mask for led green
#define GPIO_CRL_CNF_LEDGRN_Pos GPIO_CRL_CNF7_Pos //!< This define returns the register position for led green
#define GPIO_BSRR_BR_LEDGRN_Pos GPIO_BSRR_BR7_Pos //!< This define returns the register position for led green
#define GPIO_BSRR_BR_LEDGRN_Msk GPIO_BSRR_BR7_Msk //!< This define returns the register mask for led green
#define GPIO_BSRR_BS_LEDGRN_Pos GPIO_BSRR_BS7_Pos //!< This define returns the register position for led green
#define GPIO_BSRR_BS_LEDGRN_Msk GPIO_BSRR_BS7_Msk //!< This define returns the register mask for led green
/*led red*/
#define GPIO_CRH_MODE_LEDRED_Pos GPIO_CRH_MODE13_Pos //!< This define returns the register position for led red
#define GPIO_CRH_MODE_LEDRED_Msk GPIO_CRH_MODE13_Msk //!< This define returns the register mask for led red
#define GPIO_CRH_CNF_LEDRED_Msk GPIO_CRH_CNF13_Msk //!< This define returns the register mask for led red
#define GPIO_CRH_CNF_LEDRED_Pos GPIO_CRH_CNF13_Pos //!< This define returns the register position for led red
#define GPIO_BSRR_BR_LEDRED_Pos GPIO_BSRR_BR13_Pos //!< This define returns the register position for led red
#define GPIO_BSRR_BR_LEDRED_Msk GPIO_BSRR_BR13_Msk //!< This define returns the register mask for led red
#define GPIO_BSRR_BS_LEDRED_Pos GPIO_BSRR_BS13_Pos //!< This define returns the register position for led red
#define GPIO_BSRR_BS_LEDRED_Msk GPIO_BSRR_BS13_Msk //!< This define returns the register mask for led red

/*RADIO*/
/*chip select*/
#define GPIO_RADIO_CS GPIOA
#define GPIO_CRL_MODE_RADIO_CS_Pos GPIO_CRL_MODE2_Pos
#define GPIO_CRL_MODE_RADIO_CS_Msk GPIO_CRL_MODE2_Msk
#define GPIO_CRL_CNF_RADIO_CS_Msk GPIO_CRL_CNF2_Msk
#define GPIO_CRL_CNF_RADIO_CS_Pos GPIO_CRL_CNF2_Pos
#define GPIO_BSRR_BR_RADIO_CS_Msk GPIO_BSRR_BR2_Msk
#define GPIO_BSRR_BS_RADIO_CS_Msk GPIO_BSRR_BS2_Msk
/*shutdown*/
#define GPIO_RADIO_SHDN GPIOA
#define GPIO_CRL_MODE_RADIO_SHDN_Pos GPIO_CRL_MODE3_Pos
#define GPIO_CRL_MODE_RADIO_SHDN_Msk GPIO_CRL_MODE3_Msk
#define GPIO_CRL_CNF_RADIO_SHDN_Msk GPIO_CRL_CNF3_Msk
#define GPIO_CRL_CNF_RADIO_SHDN_Pos GPIO_CRL_CNF3_Pos
#define GPIO_BSRR_BR_RADIO_SHDN_Msk GPIO_BSRR_BR3_Msk
#define GPIO_BSRR_BS_RADIO_SHDN_Msk GPIO_BSRR_BS3_Msk
/*gpio3*/
#define GPIO_RADIO_GPIO3_USED //!< If defined gpio3 can be used
#ifdef GPIO_RADIO_GPIO3_USED
  #define GPIO_RADIO_GPIO3 GPIOA //!< GPIO3 pin is connected to port A
  #define GPIO_CRL_MODE_RADIO_GPIO3_Pos GPIO_CRL_MODE4_Pos
  #define GPIO_CRL_MODE_RADIO_GPIO3_Msk GPIO_CRL_MODE4_Msk
  #define GPIO_CRL_CNF_RADIO_GPIO3_Msk GPIO_CRL_CNF4_Msk
  #define GPIO_CRL_CNF_RADIO_GPIO3_Pos GPIO_CRL_CNF4_Pos
  #define GPIO_BSRR_BR_RADIO_GPIO3_Msk GPIO_BSRR_BR4_Msk
  #define GPIO_BSRR_BS_RADIO_GPIO3_Msk GPIO_BSRR_BS4_Msk
  #define GPIO_IDR_RADIO_GPIO3_Msk GPIO_IDR_IDR4_Msk
#endif

/*SPI*/
#define GPIO_SPI GPIOA //!< SPI pins are connected to port A
/*clock*/
#define GPIO_CRL_MODE_SPISCK_Pos GPIO_CRL_MODE5_Pos //!< SPI device SCK pin is connected to pin 5
#define GPIO_CRL_MODE_SPISCK_Msk GPIO_CRL_MODE5_Msk //!< SPI device SCK pin is connected to pin 5
#define GPIO_CRL_CNF_SPISCK_Msk GPIO_CRL_CNF5_Msk //!< SPI device SCK pin is connected to pin 5
#define GPIO_CRL_CNF_SPISCK_Pos GPIO_CRL_CNF5_Pos //!< SPI device SCK pin is connected to pin 5
/*miso*/
#define GPIO_CRL_MODE_SPIMISO_Pos GPIO_CRL_MODE6_Pos //!< SPI device MISO pin is connected to pin 6
#define GPIO_CRL_MODE_SPIMISO_Msk GPIO_CRL_MODE6_Msk //!< SPI device SCK pin is connected to pin 6
#define GPIO_CRL_CNF_SPIMISO_Msk GPIO_CRL_CNF6_Msk //!< SPI device SCK pin is connected to pin 6
#define GPIO_CRL_CNF_SPIMISO_Pos GPIO_CRL_CNF6_Pos //!< SPI device SCK pin is connected to pin 6
/*mosi*/
#define GPIO_CRL_MODE_SPIMOSI_Pos GPIO_CRL_MODE7_Pos //!< SPI device MOSI pin is connected to pin 7
#define GPIO_CRL_MODE_SPIMOSI_Msk GPIO_CRL_MODE7_Msk //!< SPI device SCK pin is connected to pin 7
#define GPIO_CRL_CNF_SPIMOSI_Msk GPIO_CRL_CNF7_Msk //!< SPI device SCK pin is connected to pin 7
#define GPIO_CRL_CNF_SPIMOSI_Pos GPIO_CRL_CNF7_Pos //!< SPI device SCK pin is connected to pin 7

/*VBAT Monitor*/

/*POWER SWITCH*/

/*GPS*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* PIN_DEFINITIONS_H */