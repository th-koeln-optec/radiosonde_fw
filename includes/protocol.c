/// @file protocol.c
/*
 * Filename:            protocol.c
 * Description:         This file contains the protocol implementation of the VAISALA RS41 PROTOCOL. All protocol specific stettings and data
 *                      will be specified here. It handles the insertion into the transmission frame with the protocol inherent fields based on the
 *                      data it gets.
 * Author:              M. Malyska
 */

#include "protocol.h"

/**
 * This is the xor mask used for data whitening of the frame. It is xored with the frame itself, when the frame length is greater than 
 * the mask length the xoring engine will handle a cyclic wrap around, thefore the 'length' of the mask must be passed to the xoring engine.
 * The 'pointer' is used by the xoring engine to keep track at which point it currently is during the xoring cycle.
 */
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

/**
 * This is the header that is inherent to the vaisala protocol, it is constant and does not change.
 * Its length must correspond with the length statet in the field definition.
 */
const uint8_t protocol_header[8] = {0x86, 0x35, 0xf4, 0x40, 0x93, 0xdf, 0x1a, 0x60};

/**
 * This byte specifies the frametype send, it is inherent to the vaisala protocol.
 * When sending a regualr frame this byte musst be used.
 */
const uint8_t protocol_frametype_regular[1] = {0x0f};
/**
 * This byte specifies the frametype send, it is inherent to the vaisala protocol.
 * When sending a extended frame this byte musst be used.
 */
const uint8_t protocol_frametype_extended[1] = {0xf0};

/**
 * This sturct defines the crc properties used by the vailsa protocol.
 * In order to get the correct crc check sum by the vaisla definiton the crc shift register
 * must be prefilled with 0xffff and the CRC16-CCITT generator polynomial 0x1021 must be used.
 * For further information look into the comm_crc_engine function.
 */
crc_t protocol_crc = {
  .initial = 0xffff,
  .generator = 0x1021
};

/*FIELD DEFINTIONS*/
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

#ifdef PROTOCOL_XDATA
  field_t protocol_f_xdata = {
    .id = 0x7e,
    .offset = 0x012b,
    .length = 0x15,
    .crc = 0xff,
    .head = 0xff
  };

  field_t protocol_f_empty = {
    .id = 0x76,
    .offset = 0x0144,
    .length = 0xbe,
    .crc = 0xff,
    .head = 0xff
  };
#else
  const field_t protocol_f_empty = {
    .id = 0x76,
    .offset = 0x012b,
    .length = 0x11,
    .crc = 0xff,
    .head = 0xff
  };
#endif
/*FIELD DEFINITIONS END*/

/**
 * All preparations that need to be taken before unsing the porotocol functions are stated here.
 */
void protocol_init(void){
  protocol_field_write(&protocol_f_header, protocol_header);
  #ifdef PROTOCOL_CDATA
    protocol_field_write(&protocol_f_frametype, protocol_frametype_extended);
  #else
    protocol_field_write(&protocol_f_frametype, protocol_frametype_regular);
  #endif
}

/**
 * This function writes a field as defined by the vaisala protocol into the comm_frame_txbuffer.
 * It prefixes the raw field data with the field id an the data length if field->head is 0xff.
 * It suffices the raw field data with the crc checksum over the raw data if field->crc is 0xff.
 * field->offset holds the position offset of the field, counted from the beginning of the frame.
 * field->length holds the amount of raw data bytes without head and crc checksum
 * @param field Pointer to the field struct which should be written to.
 * @param data Pointer to an array of payload data.
 */
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