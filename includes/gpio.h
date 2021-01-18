/// @file gpio.h
/*
 * Filename:            gpio.h
 * Description:         This file containts gpio related function headers.
 * Author:              M. Malyska
 */

#ifndef GPIO_H
#define GPIO_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include "stm32f1xx.h"
#include "pin_definitions.h"
#include <stdint.h>

/*GLOBAL VARIABLES*/

/*enumERATORS*/

/*typedefINITIONS*/

/*DEFINES*/
#define GPIO_CRL_CNF_IN_ANALOG 0x0UL  //!< GPIO input pin function is analog
#define GPIO_CRL_CNF_IN_FLOAT 0x1UL   //!< GPIO input pin fucntion is floating
#define GPIO_CRL_CNF_IN_PULL 0x2UL    //!< GPIO input pin functiion is pull-up or -down
#define GPIO_CRL_CNF_OUT_PP 0x0UL     //!< GPIO output pin function is push pull
#define GPIO_CRL_CNF_OUT_OD 0x1UL     //!< GPIO output pin function is open drain
#define GPIO_CRL_CNF_OUT_AFPP 0x2UL   //!< GPIO alternate pin function is push pull
#define GPIO_CRL_CNF_OUT_AFOD 0x3UL   //!< GPIO alternate pin function is open drain
#define GPIO_CRH_CNF_IN_ANALOG 0x0UL  //!< GPIO input pin function is analog
#define GPIO_CRH_CNF_IN_FLOAT 0x1UL   //!< GPIO input pin fucntion is floating
#define GPIO_CRH_CNF_IN_PULL 0x2UL    //!< GPIO input pin functiion is pull-up or -down
#define GPIO_CRH_CNF_OUT_PP 0x0UL     //!< GPIO output pin function is push pull
#define GPIO_CRH_CNF_OUT_OD 0x1UL     //!< GPIO output pin function is open drain
#define GPIO_CRH_CNF_OUT_AFPP 0x2UL   //!< GPIO alternate pin function is push pull
#define GPIO_CRH_CNF_OUT_AFOD 0x3UL   //!< GPIO alternate pin function is open drain

#define GPIO_CRL_MODE_IN 0x0UL      //!< GPIO pin direction mode is input
#define GPIO_CRL_MODE_OUT10 0x1UL   //!< GPIO pin direction mode is output with 10MHz max speed
#define GPIO_CRL_MODE_OUT2 0x2UL    //!< GPIO pin direction mode is output with 2MHz max speed
#define GPIO_CRL_MODE_OUT50 0x3UL   //!< GPIO pin direction mode is output with 50MHz max speed
#define GPIO_CRH_MODE_IN 0x0UL      //!< GPIO pin direction mode is input
#define GPIO_CRH_MODE_OUT10 0x1UL   //!< GPIO pin direction mode is output with 10MHz max speed
#define GPIO_CRH_MODE_OUT2 0x2UL    //!< GPIO pin direction mode is output with 2MHz max speed
#define GPIO_CRH_MODE_OUT50 0x3UL   //!< GPIO pin direction mode is output with 50MHz max speed

/*PUBLIC PROTOTYPES*/
void gpio_init(void);
void gpio_rcc_init(void);
void gpio_spi1_init(void);
void gpio_radio_init(void);
void gpio_led_init(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* GPIO_H */


