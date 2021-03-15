/// @file comm.c
/*
 * Filename:            comm.h
 * Description:         This file containts the communication stack. Its pourpose is to hold and manage the transmission frame.
 *                      Its finite state machine handles the transmission cycle of one frame and organizes it in conjunction with
 *                      the radio ic. It is constructed so that frames of a length greater than the radio ic's fifo size can be handled
 *                      through frame splitup and fast enough reload of fifo data. The radio needs to provide a possibility to read the
 *                      remaining free space of the fifo.
 * Author:              M. Malyska
 */

#include "comm.h"

/**
 * This flag will be set by the ISR if the radio issues an interrupt on the nIRQ pin.
 * Application code has to call comm_int_handler hereon.
 */
uint8_t comm_int_handler_pending = 0;

/**
 * This flag is set upon start state of the fsm, after first fifo loading it will be checked and when set comm_hal_tx_start will be called.
 */
static uint8_t first_packet = 0;

/**
 * Finite state machine's current state memory variable.
 */
static enum comm_fifo_tx_fsm_currentState comm_fifo_tx_fsm_currentState;

/**
 * This buffer will hold a direct shadow copy of the source frame buffer. The splitted loading of this frame into a smaller radio fifo
 * leads to the necessaty of a write proteced frame so that data changes occur during one transmission cycle. The asynchronous and slow
 * data transmission leads to exessive wait time which can be used by functions that write to the frame buffer,and otherwise would have
 * to wait for a completed transmission.
 */
static frame_t frame_txbuffer_shadow = {
  .buffer = {0},
  .start = 0,
  .end = 0,
  .pointer = 0
};

/**
 * Init of the frame buffer and various other preconditions are performed here.
 */
void comm_init(void){
  comm_hal_init();
  comm_frame_init(&frame_txbuffer_shadow);
  comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_idle;
}

/**
 * Starts transmission of a frame by starting the tx finite state machine.
 * @param frame This points to a frame struct that holds the prepared data which is to get transmitted.
 * @return If starting of a transmission is valid at the point of function calling sys_error_noe is returned, else sys_error_comm is returned.
 */
sys_error_t comm_frame_send(frame_t* frame){
  sys_error_t sys_error;
  if(comm_fifo_tx_fsm_currentState == comm_fifo_tx_fsm_state_idle){
    if(sys_error_none != comm_frame_make_shadowcopy(frame, &frame_txbuffer_shadow)){
    sys_error_handler(sys_error_comm);
    }
    comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_start;
    comm_fifo_tx_fsm();
    sys_error = sys_error_none;
  }
  else{
    sys_error = sys_error_comm;
  }
  return sys_error;
}

/**
 * This finite state machine controls frame transmission and interaction with the radio ic.
 * The start state will perform various preparation steps in order to obtain a frame that is filled
 * with the raw data which is to be transmitted.
 * The load state is invoked as long as there are still frame parts left to transmit, on completition a change
 * to idle state is issued. On first calling of the load state the transmission start command is send to the radio.
 */
void comm_fifo_tx_fsm(void){
  switch(comm_fifo_tx_fsm_currentState){
    case comm_fifo_tx_fsm_state_start:{
      first_packet = 0xff;
      frame_txbuffer_shadow.pointer = frame_txbuffer_shadow.start; //Reset frame position pointer so that fifo loader will begin at start.
      comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_load;
    }

    case comm_fifo_tx_fsm_state_load:{
      switch(comm_fifo_loader(&frame_txbuffer_shadow)){
        case comm_fifo_loader_finished:{
          comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_idle;
          break;
        }
        case comm_fifo_loader_busy:{
          comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_load;
          break;
        }
        case comm_fifo_loader_error:{
          comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_start; //Attempt retransmission if buffer underflow occurs
          break;
        }
        default:{
          break;
        }
        }
      if(first_packet){
        first_packet = 0;
        comm_hal_tx_start();
      }
      break;
    }

    case comm_fifo_tx_fsm_state_idle:{
      break;
    }

    default:{
      break;
    }
  }
}

/**
 * Provides the logic necessary to maintain a full fifo. Since the radio ic fifo has a capacity smaller than one frame,
 * it will be unavoidable to splitt one frame into multiple packages and fill the fifo acordingly to the currently availabe emtpy space.
 * @return Will be comm_fifo_loader_busy if there is still frame left for transmit, if frame is fully transmitted comm_fifo_loader_finished will be returned.
 */
static enum comm_fifo_loader_result comm_fifo_loader(frame_t* frame){
  const uint16_t frame_packet_remaining = frame->end - frame->pointer;
  const uint8_t fifo_emtpy_space = comm_hal_fifo_get_space();

  if((fifo_emtpy_space >= COMM_HAL_RADIO_FIFO_SIZE) && !first_packet){ //Check if fifo underflow occured, this is when fifo is completely empty, but not on first loading.
    return comm_fifo_loader_error;
  }

  if(frame_packet_remaining > fifo_emtpy_space){ //Check if there is enough frame left to completly refill the fifo.
    comm_hal_fifo_write(frame->pointer, fifo_emtpy_space);
    frame->pointer += fifo_emtpy_space;
    return comm_fifo_loader_busy;
  }
  else{
    comm_hal_fifo_write(frame->pointer, frame_packet_remaining);
    return comm_fifo_loader_finished;
  }
}

  /**
   * This function will initialize a frame_t struct, it sets the start pointer according to the position
   * of the buffer array in memory. The end pointer will be initialized according to array length.
   * @param frame This is a pointer to the frame_t struct which is to be initialized.
   */
  void comm_frame_init(frame_t* frame){
    frame->start = frame->buffer;
    //(frame->end = (frame->start + (sizeof(frame->buffer)-1));
    frame->end = (frame->start + sizeof(frame->buffer));
    frame->pointer = frame->start;
  }

/**
 * This function will copy the buffer of one frame struct into another frame sturct.
 * @param source Pointer to the frame struct whose buffer serves as data source.
 * @param destination Pointer to the frame struct whose buffer will be copyed to.
 * @return If a buffer size error occurs sys_error_comm will be returned, else sys_error_none will be returned.
 */
sys_error_t comm_frame_make_shadowcopy(frame_t* source, frame_t* destination){
  //Todo: disable all interrupts during execution of this function, in order to make it atomic
  for(source->pointer = source->start, destination->pointer = destination->start;
      source->pointer < source->end;
      source->pointer++, destination->pointer++){
    if(destination->pointer > destination->end){ //Check if destination buffer is too small.
      return sys_error_comm;
    }
    else{
      *destination->pointer = *source->pointer;
    }
  }
  return sys_error_none;
}

/**
 * This function calculates a crc-16 checksum of a data array of given length.
 * @param data Pointer to an array whose crc sum will be calculated.
 * @param length Length of data array or on how many bytes will the crc sum be calculated.
 * @param crc This struct holds all settings needed to define a crc calculation.
 * @return This is the crc calculation result.
 */
uint16_t comm_crc16_engine(uint8_t* data, uint16_t length, const crc_t crc){
  uint16_t crc_buffer = crc.initial;

  while(length--){
    crc_buffer ^= (*data++ << 8); //Increment pointer address after data is retreived.
    for(uint8_t i = 8; i > 0; i--){
      if(crc_buffer & 0x8000){ //Check if MSB is set
        crc_buffer = (crc_buffer << 1) ^ crc.generator;
      }
      else{
        crc_buffer <<= 1;
      }
    }
  }
  return crc_buffer;
}

/**
 * This function xor's the frame with a mask of limited length and handles the cyclic wraping from mask end to mask start.
 * @param frame Pointer to the frame struct whose buffer the xoring will be applied to.
 * @param mask Pointer to the mask struct whose array is the xoring mask.
 */
void comm_xor_engine(frame_t* frame, xor_mask_t* mask){
  for(frame->pointer = frame->start, mask->pointer = mask->array;
      frame->pointer <= frame->end;
      frame->pointer++, mask->pointer++){
    if(mask->pointer >= (mask->array + mask->length)){
      mask->pointer = mask->array;
    }

    *frame->pointer ^= *mask->pointer;
  }
}

/**
 * This function has to be called when the radio ic issues an interrupt. It determines by which internal source the interrupt was triggerd.
 * Based on this various functions can be called.
 */
void comm_int_handler(void){
  uint8_t ph_int_status = 0;
  ph_int_status = comm_hal_int_get_ph();
  if(ph_int_status & COMM_HAL_INT_PH_TX_FIFO_ALMOST_EMPTY_Msk){
    //Call the fifo management engine when the radio issues a almost emtpy fifo.
    comm_fifo_tx_fsm();
  }
  #ifdef COMM_RX_ENABLED
  if(ph_int_status & COMM_HAL_INT_PH_PACKET_SENT_Msk){
    //Start rx mode when radio finished sending a packet.
    comm_hal_rx_start();
  }
  if(ph_int_status & COMM_HAL_INT_PH_PACKET_RX_Msk){
    //Read received data out, when radio issues a vaild received packet.
    comm_fifo_getter();
  }
  #endif
}

#ifdef COMM_RX_ENABLED
/**
 * This flag will be set if a packet was received and fifo was read out.
 */
uint8_t comm_rx_pending = 0;

/**
 * Received data will be written here.
 */
uint8_t comm_rx_buffer[COMM_FRAME_RX_BUFFER_SIZE] = {0};

/**
 * If a valid packet is received by the radio the interrupt handler has to call this function.
 * The FIFO will be read out and the data will be written in comm_rx_buffer.
 */
void comm_fifo_getter(void){
  comm_hal_fifo_read(comm_rx_buffer, COMM_FRAME_RX_BUFFER_SIZE);
  comm_rx_pending = 0xff;
}
#endif