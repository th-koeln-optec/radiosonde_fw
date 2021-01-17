#include "radio.h"

uint8_t radio_cfg[] = RADIO_CONFIGURATION_DATA_ARRAY;

void radio_init(void){
  si446x_reset();
  
  si446x_configuration_init(radio_cfg);

  si446x_part_info();
  if(Si446xCmd.PART_INFO.PART != RADIO_IC_TYPE){
    sys_error_handler(sys_error_radio);
  }
}

void radio_tx_start(void){
  si446x_start_tx(0x00, 0x00, COMM_FRAME_BUFFER_SIZE);
}

uint8_t radio_fifo_status(void){

}

uint8_t radio_fifo_get_space(void){
  si446x_fifo_info_fast_read();
  return Si446xCmd.FIFO_INFO.TX_FIFO_SPACE;
}

void radio_fifo_write(uint8_t* buffer, uint8_t buffer_length){
  si446x_write_tx_fifo(buffer_length, buffer);
}

void radio_deviation_set(uint16_t value){

}