/// @file spi.h
/*
 * Filename:            spi.h
 * Description:         This file containts spi related function headers.
 * Author:              M. Malyska
 */

#ifndef SPI_H
#define SPI_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include "stm32f1xx.h"
#include "gpio.h"
#include <stdint.h>

/*GLOBAL VARIABLES*/

/*enumERATORS*/

/*typedefINITIONS*/

/*DEFINES*/
#define SPI_READWRITE_Msk 0x80U //!< SPI read- or write-bit bit-mask

#define SPI_CR1_BR_FPCLK2 0x0UL   //!< SPI baud rate is peripheral clock divided by 2
#define SPI_CR1_BR_FPCLK4 0x1UL   //!< SPI baud rate is peripheral clock divided by 4
#define SPI_CR1_BR_FPCLK8 0x2UL   //!< SPI baud rate is peripheral clock divided by 8
#define SPI_CR1_BR_FPCLK16 0x3UL  //!< SPI baud rate is peripheral clock divided by 16
#define SPI_CR1_BR_FPCLK32 0x4UL  //!< SPI baud rate is peripheral clock divided by 32
#define SPI_CR1_BR_FPCLK64 0x5UL  //!< SPI baud rate is peripheral clock divided by 64
#define SPI_CR1_BR_FPCLK128 0x6UL //!< SPI baud rate is peripheral clock divided by 128
#define SPI_CR1_BR_FPCLK256 0x7UL //!< SPI baud rate is peripheral clock divided by 256

/*PUBLIC PROTOTYPES*/
void spi1_init(void);
uint8_t spi_trx(uint8_t data);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SPI_H */
