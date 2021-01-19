/// @file protocol.c
/*
 * Filename:            protocol.c
 * Description:         TBD
 * Author:              M. Malyska
 */

#include "protocol.h"

xor_mask_t protocol_xor = {
  .length = 0x40,
  .pointer = 0x00,
  .array = {
    0x96, 0x83, 0x3E, 0x51, 0xB1, 0x49, 0x08, 0x98,
    0x32, 0x05, 0x59, 0x0E, 0xF9, 0x44, 0xC6, 0x26,
    0x21, 0x60, 0xC2, 0xEA, 0x79, 0x5D, 0x6D, 0xA1,
    0x54, 0x69, 0x47, 0x0C, 0xDC, 0xE8, 0x5C, 0xF1,
    0xF7, 0x76, 0x82, 0x7F, 0x07, 0x99, 0xA2, 0x2C,
    0x93, 0x7C, 0x30, 0x63, 0xF5, 0x10, 0x2E, 0x61,
    0xD0, 0xBC, 0xB4, 0xB6, 0x06, 0xAA, 0xF4, 0x23,
    0x78, 0x6E, 0x3B, 0xAE, 0xBF, 0x7B, 0x4C, 0xC1
    }
};

const uint8_t protocol_header[8] = {0x86, 0x35, 0xf4, 0x40, 0x93, 0xdf, 0x1a, 0x60};
const uint8_t protocol_frametype_regular[1] = {0x0f};
const uint8_t protocol_frametype_extended[1] = {0xf0};

crc_t protocol_crc = {
  .initial = 0xffff,
  .generator = 0x1021
};

const field_t protocol_f_header = {
  .id = 0x00,
  .offset = 0x0000,
  .length = 0x08,
  .crc = 0x00,
  .head = 0x00
};

const field_t protocol_f_ecc = {
  .id = 0x00,
  .offset = 0x0008,
  .length = 0x30,
  .crc = 0x00,
  .head = 0x00
};

const field_t protocol_f_frametype = {
  .id = 0x00,
  .offset = 0x0038,
  .length = 0x01,
  .crc = 0x00,
  .head = 0x00
};

const field_t protocol_f_status = {
  .id = 0x79,
  .offset = 0x0039,
  .length = 0x28,
  .crc = 0xff,
  .head = 0xff
};

const field_t protocol_f_meas = {
 .id = 0x7a,
 .offset = 0x0065,
 .length = 0x2a,
 .crc = 0xff,
 .head = 0xff
};

const field_t protocol_f_gpsinfo = {
  .id = 0x7c,
  .offset = 0x0093,
  .length = 0x1e,
  .crc = 0xff,
  .head = 0xff
};

const field_t protocol_f_gpsraw = {
  .id = 0x7d,
  .offset = 0x00b5,
  .length = 0x59,
  .crc = 0xff,
  .head = 0xff
};

const field_t protocol_f_gpspos = {
  .id = 0x7b,
  .offset = 0x0112,
  .length = 0x15,
  .crc = 0xff,
  .head = 0xff
};

const field_t protocol_f_empty = {
  .id = 0x76,
  .offset = 0x012b,
  .length = 0x11,
  .crc = 0xff,
  .head = 0xff
};

void protocol_init(void){
  protocol_field_write(&protocol_f_header, protocol_header);
  protocol_field_write(&protocol_f_frametype, protocol_frametype_regular);
}

void protocol_field_write(const field_t* field, uint8_t* data){
  uint16_t crc = 0x0000;
  uint8_t* field_start = (comm_frame_txbuffer.start + field->offset);
  comm_frame_txbuffer.pointer = field_start;
  if(field->head){
    *comm_frame_txbuffer.pointer++ = field->id;
    *comm_frame_txbuffer.pointer++ = field->length;
    field_start += 2U; //If head is enabled field_start has to be incremented by the two head bytes, so that it will point to where plain data starts.
  }
  for(;comm_frame_txbuffer.pointer < (field_start + field->length);){
    *comm_frame_txbuffer.pointer++ = *data++;
  }
  if(field->crc){
    crc = comm_crc16_engine(field_start, field->length, protocol_crc);
    *comm_frame_txbuffer.pointer++ = crc;
    *comm_frame_txbuffer.pointer = (crc >> 8);
  }
}