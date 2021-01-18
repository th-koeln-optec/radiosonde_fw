/// @file comm_hal.c
/*
 * Filename:            comm_hal.c
 * Description:         This file containts a hardware abstraction for the communication stack to interface the radio. Those functions provided here
 *                      are functionalities from the stack point of view. These functions itself implement the necessary actions and commands of the 
 *                      corresponding radio library. For different radio hardware and libraries these functions need to be updated.
 * Author:              M. Malyska
 */

#include "comm_hal.h"

/**
 * This array get initialized with the prepared register settings from radio_config_Si446x.h which is
 * automatically generated by WDS-Studio. It provides the arrary initial value as the define RADIO_CONFIGURATION_DATA_ARRAY.
 */
uint8_t comm_hal_cfg[] = RADIO_CONFIGURATION_DATA_ARRAY;

/**
 * This function initializes the radio and loads all radio registers with its presets.
 */
void comm_hal_init(void){
  si446x_reset();
  
  si446x_configuration_init(comm_hal_cfg);

  si446x_part_info();
  if(Si446xCmd.PART_INFO.PART != RADIO_IC_TYPE){
    sys_error_handler(sys_error_radio);
  }
}

/**
 * This functions sends the necessary command to the radio in order to start the transmission of a frame.
 */
void comm_hal_tx_start(void){
  si446x_start_tx(0x00, 0x00, COMM_FRAME_BUFFER_SIZE);
}

/**
 * This functions returns the amount of empty bytes in the radio fifo memory.
 * @return fifo memory empty space in bytes.
 */
uint8_t comm_hal_fifo_get_space(void){
  si446x_fifo_info_fast_read();
  return Si446xCmd.FIFO_INFO.TX_FIFO_SPACE;
}

/**
 * This functions writes multiple bytes into the radio fifo memory.
 * @param buffer Pointer to the data which is to be loader into the fifo
 * @param buffer_length How many bytes will be loaded into the fifo.
 */
void comm_hal_fifo_write(uint8_t* buffer, uint8_t buffer_length){
  si446x_write_tx_fifo(buffer_length, buffer);
}