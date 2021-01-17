#ifndef COMM_H
#define	COMM_H
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

/****INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>
#include "radio.h"

/****DEFINES*/
#define COMM_FRAME_BUFFER_SIZE  (320)

#define COMM_FRAME_REPETITION_RATE
#define COMM_DATA_RATE

/****typedefINITIONS*/
typedef struct {
  uint8_t* start;
  uint8_t* end;
  uint8_t* pointer;
  uint8_t buffer[COMM_FRAME_BUFFER_SIZE];
}frame_t;

/****GLOBAL VARIABLES*/
extern frame_t comm_frame_txbuffer;

/****VARIABLES*/

/****enumERATORS*/
enum comm_fifo_tx_fsm_currentState {comm_fifo_tx_fsm_state_start, comm_fifo_tx_fsm_state_load, comm_fifo_tx_fsm_state_idle};
enum comm_fifo_loader_result {comm_fifo_loader_busy, comm_fifo_loader_finished};

/****PUBLIC PROTOTYPES*/
void comm_init(void);
void comm_frame_send(void);
void comm_fifo_tx_fsm(void);
enum comm_fifo_loader_result comm_fifo_loader(frame_t* frame);
sys_error_t comm_frame_make_shadowcopy(frame_t* source, frame_t* destination);
void comm_frame_calc_xor(frame_t* frame);
uint16_t comm_crc16_engine(uint8_t* data, uint16_t length, const uint16_t initial, const uint16_t generator);

void test(void);
#ifdef	__cplusplus
}
#endif /* __cplusplus */
#endif	/* COMM_H */
