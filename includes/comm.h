/// @file comm.h
/*
 * Filename:            comm.h
 * Description:         This file containts comm.c related function headers.
 * Author:              M. Malyska
 */

#ifndef COMM_H
#define COMM_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>
#include "comm_hal.h"


/*DEFINES*/
#define COMM_FRAME_REPETITION_RATE
#define COMM_DATA_RATE

/*typedefINITIONS*/
typedef struct {
  uint16_t initial;
  uint16_t generator;
}crc_t;

typedef struct {
  uint16_t length;
  uint8_t* pointer;
  uint8_t array[];
}xor_mask_t;

/*
Protocol.h must be included only at this point, since it depends on the xor_mask_t type, which is defined in this header.
Ok yes, this is somewhat ugly but I did not find a better solution. Typedef must be defined here, but the acutal declaration is
an inherent property of the protocol itself, so it should be declared there.*/
#include "protocol.h"

typedef struct {
  uint8_t* start;
  uint8_t* end;
  uint8_t* pointer;
  uint8_t buffer[COMM_FRAME_BUFFER_SIZE];
}frame_t;

/*GLOBAL VARIABLES*/
extern frame_t comm_frame_txbuffer;

/*VARIABLES*/

/*enumERATORS*/
enum comm_fifo_tx_fsm_currentState {comm_fifo_tx_fsm_state_start, comm_fifo_tx_fsm_state_load, comm_fifo_tx_fsm_state_idle};
enum comm_fifo_loader_result {comm_fifo_loader_busy, comm_fifo_loader_finished};

/*PUBLIC PROTOTYPES*/
void comm_init(void);
void comm_frame_send(void);
void comm_fifo_tx_fsm(void);
enum comm_fifo_loader_result comm_fifo_loader(frame_t* frame);
sys_error_t comm_frame_make_shadowcopy(frame_t* source, frame_t* destination);
void comm_frame_xor_engine(frame_t* frame, xor_mask_t* mask);
uint16_t comm_crc16_engine(uint8_t* data, uint16_t length, const crc_t crc);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* COMM_H */
