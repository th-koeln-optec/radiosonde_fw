#ifndef RADIO_H
#define RADIO_H
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

/****INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>
#include "sys.h"
#include "radio/Si446x/si446x_api_lib.h"
#include "comm.h"

/****GLOBAL VARIABLES*/

/****enumERATORS*/

/****typedefINITIONS*/

/*****SYSTEM TICK*/
    
/*****SYSTEM STATES*/
    
/****DEFINES*/
#define RADIO_IC_TYPE 0x4464

#define RADIO_DATA_RATE 4800

#define RADIO_FIFO_SIZE 0x3b //!< Si4x3x FIFO is 64 byte long minus 5 byte ae
#define RADIO_FIFO_ALMOST_EMPTY_Msk 0x01

/****PUBLIC PROTOTYPES*/
void radio_init(void);
void radio_tx_start(void);
uint8_t radio_fifo_status(void);
uint8_t radio_fifo_get_space(void);
void radio_fifo_write(uint8_t* buffer, uint8_t buffer_length);
void radio_deviation_set(uint16_t value);

#ifdef	__cplusplus
}
#endif /* __cplusplus */
#endif /* RADIO_H */
