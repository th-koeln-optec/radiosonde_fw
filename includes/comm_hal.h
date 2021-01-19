/// @file comm_hal.h
/*
 * Filename:            comm_hal.h
 * Description:         This file containts comm_hal.c related function headers.
 * Author:              M. Malyska
 */

#ifndef COMM_HAL_H
#define COMM_HAL_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>
#include "sys.h"
#include "radio/Si446x/si446x_api_lib.h"
#include "radio/Si446x/radio_config_Si4464.h"
#include "comm.h"

//Todo: move wds generated file radio_congig... into radio folder

/*GLOBAL VARIABLES*/

/*enumERATORS*/

/*typedefINITIONS*/

/*DEFINES*/
#define RADIO_IC_TYPE 0x4464

#define RADIO_FREQUENCY_LIMIT_UPPER 0x78

//Todo: implement data rate switching functions
#define RADIO_DATA_RATE 4800

/*PUBLIC PROTOTYPES*/
void comm_hal_init(void);
void comm_hal_tx_start(void);
uint8_t comm_hal_fifo_get_space(void);
void comm_hal_fifo_write(uint8_t* buffer, uint8_t buffer_length);
void comm_hal_datarate_set(uint16_t datarate);
void comm_hal_frequency_set(uint8_t frequency);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* RADIO_H */
