/// @file radio_hal.h
/*
 * Filename:            radio_hal.h
 * Description:         This file contains RADIO HAL.
 * Author:              Copyright 2011 Silicon Laboratories, Inc.
 * Edited by:           M. Malyska; The original file can be found in the same directory: radio_hal.h.bak
 */

#ifndef RADIO_HAL_H
#define RADIO_HAL_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include <stdint.h>
#include "pin_definitions.h"
#include "spi.h"

/*GLOBAL VARIABLES*/

/*enumERATORS*/

/*typedefINITIONS*/

/*DEFINES*/

/*PUBLIC PROTOTYPES*/
void radio_hal_AssertShutdown(void);
void radio_hal_DeassertShutdown(void);
void radio_hal_ClearNsel(void);
void radio_hal_SetNsel(void);
//uint8_t radio_hal_NirqLevel(void);

void radio_hal_SpiWriteByte(uint8_t byte_to_write);
uint8_t radio_hal_SpiReadByte(void);

void radio_hal_SpiWriteData(uint8_t byte_count, uint8_t* pData);
void radio_hal_SpiReadData(uint8_t byte_count, uint8_t* pData);

#ifdef RADIO_DRIVER_EXTENDED_SUPPORT

#ifdef GPIO_RADIO_GPIO0_USED
uint8_t radio_hal_Gpio0Level(void);
#endif //#ifdef GPIO_RADIO_GPIO0_USED

#ifdef GPIO_RADIO_GPIO1_USED
uint8_t radio_hal_Gpio1Level(void);
#endif //#ifdef GPIO_RADI1_GPIO0_USED

#ifdef GPIO_RADIO_GPIO2_USED
uint8_t radio_hal_Gpio2Level(void);
#endif //#ifdef GPIO_RADIO_GPIO2_USED

#ifdef GPIO_RADIO_GPIO3_USED
uint8_t radio_hal_Gpio3Level(void);
#endif //#ifdef GPIO_RADIO_GPIO3_USED

#endif //#ifdef RADIO_DRIVER_EXTENDED_SUPPORT

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* RADIO_HAL_H */