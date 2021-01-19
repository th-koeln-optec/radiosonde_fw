/// @file comm.c
/*
 * Filename:            comm.h
 * Description:         This file containts the communications stack. Its pourpose is to hold and manage the transmission frames.
 *                      Its finite state machine handles the transmission cycle of one frame and organizes it in conjunction with
 *                      the radio ic. It is constructed so that packet of a length greater than the radio ic's fifo size can be handled
 *                      through frame splitup and fast enough reload of fifo data. The radio needs to provide a possibility to read the
 *                      remaining free space of the fifo.
 * Author:              M. Malyska
 */

#include "comm.h"

/**
 * Finite state machine's current state memory variable.
 */
static enum comm_fifo_tx_fsm_currentState comm_fifo_tx_fsm_currentState;

/**
 * Main frame buffer. This buffer gets written by all application functions which deliver data to be transmitted.
 * This buffer is organized as a struct, so that a buffer specific pointer aswell as an start- and end-address are maintained.
 */
frame_t comm_frame_txbuffer = 
{
  .buffer = { 0 
/*
    0x86, 0x35, 0xf4, 0x40, 0x93, 0xdf, 0x1a, 0x60, // -8 byte header
    0x54, 0x0b, 0x9d, 0x7c, 0x4d, 0x9f, 0xba, 0x6a, //+
    0x46, 0x62, 0x46, 0x6c, 0xe3, 0x60, 0x74, 0x5b, //¦
    0xcc, 0x65, 0x16, 0x1c, 0xea, 0xe1, 0x39, 0x14, //¦
    0x4b, 0x38, 0x3f, 0x29, 0xa3, 0xe1, 0x18, 0x64, //¦-48 byte reed-solomon error correction data (automaticly updated)
    0xf5, 0x0f, 0xe0, 0x0e, 0xcf, 0x68, 0xf0, 0x04, //¦
    0xd9, 0x10, 0x22, 0x9d, 0x9b, 0x39, 0xfb, 0x4c, //+
    0x0f,                                           // -1 byte frame type: encodes frame length, 0x0f for regular- and 0xf0 for extended-frames
          0x79, 0x28, 0x03, 0x1e, 0x50, 0x32, 0x37, //+
    0x34, 0x30, 0x33, 0x38, 0x37, 0x1a, 0x00, 0x00, //¦
    0x03, 0x00, 0x00, 0x15, 0x00, 0x00, 0x5d, 0x00, //¦
    0x07, 0x32, 0x20, 0xc9, 0x66, 0xb5, 0x41, 0x00, //¦- 44 byte payload: 79-STATUS
    0x00, 0x40, 0x40, 0xff, 0xff, 0xff, 0xc6, 0xff, //¦
    0xff, 0xff, 0xc6, 0x42, 0xb2,                   //+
                                  0x7a, 0x2a, 0xcf, //+
    0x52, 0x02, 0x2a, 0x00, 0x02, 0x9c, 0xe7, 0x02, //¦
    0x27, 0x8d, 0x08, 0xaf, 0x87, 0x07, 0x47, 0x91, //¦
    0x08, 0xcb, 0x2b, 0x02, 0x2b, 0x00, 0x02, 0x9d, //¦- 46 byte payload: 7A-MEAS
    0xe7, 0x02, 0x09, 0x67, 0x05, 0xee, 0xa6, 0x04, //¦
    0xfe, 0xae, 0x06, 0x00, 0x00, 0xfb, 0xfb, 0x00, //¦
    0x00, 0xd6, 0xb0,                               //+
                      0x7c, 0x1e, 0xe6, 0x07, 0x18, //+
    0xfb, 0x25, 0x12, 0x01, 0xfb, 0x11, 0xf9, 0x13, //¦
    0xf3, 0x0b, 0xfa, 0x09, 0x92, 0x16, 0xf7, 0x12, //¦- 34 byte payload: 7C-GPSINFO
    0xf7, 0x03, 0xfa, 0x17, 0xfa, 0x1f, 0xf4, 0x0e, //¦
    0xf4, 0x0c, 0x91, 0xe1, 0x08,                   //+
                                  0x7d, 0x59, 0x25, //+
    0xfc, 0x35, 0x01, 0xff, 0x3d, 0xfd, 0xd3, 0x02, //¦
    0x42, 0xbf, 0x00, 0xc6, 0x8f, 0x52, 0x0b, 0x81, //¦
    0xfe, 0xff, 0x80, 0xb5, 0xe9, 0x10, 0xd8, 0x82, //¦
    0xff, 0x37, 0xc1, 0x54, 0x0a, 0x2c, 0x31, 0x01, //¦
    0xac, 0xe5, 0x47, 0x1a, 0x51, 0x15, 0xff, 0xda, //¦
    0x73, 0x7e, 0x02, 0xd3, 0x6f, 0x00, 0x15, 0x66, //¦- 93 byte payload: 7D-GPSRAW
    0x6e, 0x0c, 0x53, 0x39, 0x01, 0x21, 0x00, 0x00, //¦
    0x00, 0x77, 0x03, 0x00, 0x8a, 0x56, 0x41, 0x0f, //¦
    0x5e, 0x55, 0xff, 0x93, 0x7c, 0x1c, 0x12, 0x8b, //¦
    0xd6, 0xff, 0x30, 0xbd, 0x00, 0x11, 0xb8, 0xfc, //¦
    0x00, 0xac, 0xa8, 0xbe, 0x1d, 0xb4, 0xe3, 0xff, //¦
    0xbd, 0x9e,                                     //+
                0x7b, 0x15, 0x08, 0xea, 0xb4, 0x17, //+
    0x96, 0xb0, 0xf1, 0x03, 0xf6, 0x5d, 0x6d, 0x1d, //¦
    0x4c, 0xfd, 0x8f, 0xf5, 0x37, 0x00, 0x0d, 0x01, //¦- 25 byte payload: 7B-GPSPOS
    0x0c, 0xac, 0x6c,                               //+
                      0x76, 0x11, 0x00, 0x00, 0x00, //+
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //¦- 21 byte payload: 7E-EMPTY
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xec, 0xc7  //+
*/
  },
  .start = 0,
  .end = 0,
  .pointer = 0,
};

/**
 * Shadow frame buffer. This buffer is a direct copy of the main frame buffer. Its puropse is to perform various operations
 * on it without data being change by functions that want to update the frame buffer with new data. Furthermore the splitted
 * loading of this frame into a smaller radio fifo leads to the necessaty of a write proteced frame so that data changes will be atomic.
 * The asynchronous and slow data transmission leads to exessive wait time which can be used by functions that write to the frame buffer,
 * and otherwise would haver wait for a completed transmission.
 */
static frame_t frame_txbuffer_shadow = {
  .buffer = {0},
  .start = 0,
  .end = 0,
  .pointer = 0
};

/**
 * Init of both frame buffers is performed here.
 */
void comm_init(void){
  comm_frame_txbuffer.start = comm_frame_txbuffer.buffer;
  comm_frame_txbuffer.end = (comm_frame_txbuffer.start + sizeof(comm_frame_txbuffer.buffer));
  comm_frame_txbuffer.pointer = comm_frame_txbuffer.start;

  frame_txbuffer_shadow.start = frame_txbuffer_shadow.buffer;
  frame_txbuffer_shadow.end = (frame_txbuffer_shadow.start + sizeof(frame_txbuffer_shadow.buffer));
  frame_txbuffer_shadow.pointer = frame_txbuffer_shadow.start;
}

/**
 * Starts transmission of a frame by starting the tx finite state machine.
 */
void comm_frame_send(void){
  //Todo: lock out if currentState != idle
  comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_start;
}

/**
 * This finite state machine controls frame transmission and interaction with the radio ic.
 * The start state will perform various preparation steps in order to obtain a frame that is filled
 * with the raw data which is to be transmitted.
 * The load state is invoked as long as there are still frame parts left to transmit, on completition a change
 * to idle state is issued. On first calling of the load state the transmission start command is send to the radio.
 */
void comm_fifo_tx_fsm(void){
  static uint8_t first_packet = 0x00;
  switch(comm_fifo_tx_fsm_currentState){
    case comm_fifo_tx_fsm_state_start:{
      if(sys_error_none != comm_frame_make_shadowcopy(&comm_frame_txbuffer, &frame_txbuffer_shadow)){
        sys_error_handler(sys_error_comm);
      }
      #ifdef PROTOCOL_ECC_ENABLE
        comm_frame_calc_ecc(&frame_txbuffer_shadow);
      #endif
      #ifdef PROTOCOL_XOR_ENABLE
        comm_frame_xor_engine(&frame_txbuffer_shadow, &protocol_xor);
      #endif
      first_packet = 0xff;
      frame_txbuffer_shadow.pointer = frame_txbuffer_shadow.start; //Reset frame position pointer so that fifo loader will begin at start.
      comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_load;
      break;
    }

    case comm_fifo_tx_fsm_state_load:{
      if(comm_fifo_loader(&frame_txbuffer_shadow) == comm_fifo_loader_finished){
        comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_idle;
      }
      else{
        comm_fifo_tx_fsm_currentState = comm_fifo_tx_fsm_state_load;
      }
      if(first_packet){
        first_packet = 0x00;
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
enum comm_fifo_loader_result comm_fifo_loader(frame_t* frame){
  const uint16_t frame_packet_remaining = frame->end - frame->pointer;
  const uint8_t fifo_emtpy_space = comm_hal_fifo_get_space();

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
 * This function will copy the buffer of one frame struct into another frame sturct.
 * @param source Pointer to the frame struct whose buffer serves as data source.
 * @param destination Pointer to the frame struct whose buffer will be copyed to.
 * @return If a buffer size error occurs sys_error_comm will be returned, else sys_error_none will be returned.
 */
sys_error_t comm_frame_make_shadowcopy(frame_t* source, frame_t* destination){
  //Todo: disable all interrupts during execution of this function, in order to make it atomic
  for(source->pointer = source->start, destination->pointer = destination->start;
      source->pointer <= source->end;
      source->pointer++, destination->pointer++){
    if(destination->pointer > destination->end){ //Check if destination buffer is too small.
      return sys_error_comm;
    }
    else{
    //Todo: increment value here with post increment.
      *destination->pointer = *source->pointer;
    }
  }
  return sys_error_none;
}

/**
 * This function xor's the frame with a mask of limited length and handles the cyclic wraping from mask end to mask start.
 * @param frame Pointer to the frame struct whose buffer the xoring will be applied to.
 * @param mask Pointer to the mask struct whose array is the xoring mask.
 */
 #ifdef PROTOCOL_XOR_ENABLE
  void comm_frame_xor_engine(frame_t* frame, xor_mask_t* mask){
    for(frame->pointer = frame->start, mask->pointer = mask->array;
        frame->pointer <= frame->end;
        frame->pointer++, mask->pointer++){
      if(mask->pointer >= (mask->array + mask->length)){
        mask->pointer = mask->array;
      }

      *frame->pointer ^= *mask->pointer;
    }
  }
#endif

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