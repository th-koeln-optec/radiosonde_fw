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
#include "sys.h"

/*DEFINES*/
#define COMM_FRAME_BUFFER_SIZE 320

/*typedefINITIONS*/
typedef struct {
  uint8_t* start;
  uint8_t* end;
  uint8_t* pointer;
  uint8_t buffer[COMM_FRAME_BUFFER_SIZE];
}frame_t;

typedef struct {
  uint16_t initial;
  uint16_t generator;
}crc_t;

typedef struct {
  uint16_t length;
  uint8_t* pointer;
  uint8_t array[];
}xor_mask_t;

/*GLOBAL VARIABLES*/

/*VARIABLES*/

/*enumERATORS*/
enum comm_fifo_tx_fsm_currentState {comm_fifo_tx_fsm_state_start, comm_fifo_tx_fsm_state_load, comm_fifo_tx_fsm_state_idle};
enum comm_fifo_loader_result {comm_fifo_loader_busy, comm_fifo_loader_finished, comm_fifo_loader_error};

/*PUBLIC PROTOTYPES*/
void comm_init(void);
sys_error_t comm_frame_send(frame_t* frame);
void comm_fifo_tx_fsm(void);
static enum comm_fifo_loader_result comm_fifo_loader(frame_t* frame);
void comm_frame_init(frame_t* frame);
sys_error_t comm_frame_make_shadowcopy(frame_t* source, frame_t* destination);
uint16_t comm_crc16_engine(uint8_t* data, uint16_t length, const crc_t crc);
void comm_xor_engine(frame_t* frame, xor_mask_t* mask);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* COMM_H */
