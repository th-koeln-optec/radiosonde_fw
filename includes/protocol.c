/// @file protocol.c
/*
 * Filename:            protocol.c
 * Description:         This file contains the protocol implementation of the VAISALA RS41 PROTOCOL. All protocol specific settings and data
 *                      will be specified here. It handles the insertion into the frame-buffer with the protocol inherent fields based on the
 *                      data it gets.
 * Author:              M. Malyska
 */

#include "protocol.h"

/**
 * Main frame buffer. This buffer gets written by all application functions which deliver data to be transmitted.
 * This buffer is organized as a struct, so that a buffer specific pointer aswell as an start- and end-address are maintained.
 */
 //Todo: Make this static too and check if the shadow buffer is necessary after all.
 //Todo: Highlight the dependence of COMM_BUFFER_SIZE in comm library!!!
frame_t protocol_frame_txbuffer = 
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
 * Shadow frame buffer. This buffer is a direct copy of frame_txbuffer. Its puropse is to perform various operations
 * on it without data being changed by functions that want to update frame_txbuffer with new data.
 */
static frame_t frame_txbuffer_shadow = {
  .buffer = {0},
  .start = 0,
  .end = 0,
  .pointer = 0
};

/**
 * This is the xor mask used for data whitening of the frame. It is xored with the frame itself, when the frame length is greater than 
 * the mask length the xoring engine will handle a cyclic wrap around, thefore the 'length' of the mask must be passed to the xoring engine.
 * The 'pointer' is used by the xoring engine to keep track at which point it currently is during the xoring cycle.
 */
static xor_mask_t protocol_xor = {
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
 * This sturct defines the crc properties used by the vailsa protocol.
 * In order to get the correct crc check sum by the vaisla definiton the crc shift register
 * must be prefilled with 0xffff and the CRC16-CCITT generator polynomial 0x1021 must be used.
 * For further information look into the comm_crc_engine function.
 */
const crc_t protocol_crc = {
  .initial = 0xffff,
  .generator = 0x1021
};

GF_t GF256RS = { 
  .prime = 0x11D, // RS-GF(2^8): X^8 + X^4 + X^3 + X^2 + 1 : 0x11D
  .order = 256, // 2^8
  .alpha = 0x02, // generator: alpha = X
  .exp_a = {0},
  .log_a = {0}
};

RS_t RS256 = {
  .N = 255,
  .t = 12,
  .R = 24,
  .K = 231,
  .b = 0,
  .p = 1,
  .ip = 1,
  .g = {0},
  .gf = 0
};

/**
 * This is the header that is inherent to the vaisala protocol, it is constant and does not change.
 * Its length must correspond with the length statet in the field definition.
 */
static const uint8_t protocol_header[8] = {0x86, 0x35, 0xf4, 0x40, 0x93, 0xdf, 0x1a, 0x60};

/**
 * This byte specifies the frametype, it is inherent to the vaisala protocol.
 * When sending a regular frame this byte musst be used.
 */
const uint8_t protocol_frametype_regular[1] = {0x0f};
/**
 * This byte specifies the frametype, it is inherent to the vaisala protocol.
 * When sending an extended frame this byte musst be used.
 */
const uint8_t protocol_frametype_extended[1] = {0xf0};

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
  comm_init();
  rs_init(&RS256, &GF256RS);

  protocol_frame_txbuffer.start = protocol_frame_txbuffer.buffer;
  protocol_frame_txbuffer.end = (protocol_frame_txbuffer.start + sizeof(protocol_frame_txbuffer.buffer));
  protocol_frame_txbuffer.pointer = protocol_frame_txbuffer.start;
  frame_txbuffer_shadow.start = frame_txbuffer_shadow.buffer;
  frame_txbuffer_shadow.end = (frame_txbuffer_shadow.start + sizeof(frame_txbuffer_shadow.buffer));
  frame_txbuffer_shadow.pointer = frame_txbuffer_shadow.start;

  protocol_field_write(&protocol_f_header, protocol_header);
  #ifdef PROTOCOL_XDATA
    protocol_field_write(&protocol_f_frametype, protocol_frametype_extended);
  #else
    protocol_field_write(&protocol_f_frametype, protocol_frametype_regular);
  #endif
}

/**
 * This function will issue a complete transmission cycle. It will create a frame copy and perform ecc calculation and xoring on this copied
 * frame. Then it will trigger the comm transmission starting. 
 */
void protocol_frame_send(void){
  sys_error_t sys_error;

  comm_frame_make_shadowcopy(&protocol_frame_txbuffer, &frame_txbuffer_shadow);

  #ifdef PROTOCOL_ECC_ENABLE
    protocol_ecc_engine(&RS256, &frame_txbuffer_shadow);
  #endif
  #ifdef PROTOCOL_XOR_ENABLE
    comm_xor_engine(&frame_txbuffer_shadow, &protocol_xor);
  #endif

  sys_error = comm_frame_send(&frame_txbuffer_shadow);
  if(sys_error != sys_error_none){
    sys_error_handler(sys_error);
  }
}

/**
 * This function writes a field as defined by the vaisala protocol into the frame_txbuffer.
 * It prefixes the raw field data with the field id and the data length if field->head is 0xff.
 * It suffixes the raw field data with the crc checksum over the raw data if field->crc is 0xff.
 * field->offset holds the position offset of the field, counted from the beginning of the frame.
 * field->length holds the amount of raw data bytes without head and crc checksum
 * @param field Pointer to the field struct which should be written to.
 * @param data Pointer to an array of payload data.
 */
void protocol_field_write(const field_t* field, const uint8_t* data){
  uint16_t crc = 0x0000;
  uint8_t* field_start = (protocol_frame_txbuffer.start + field->offset);
  protocol_frame_txbuffer.pointer = field_start;
  if(field->head){
    *protocol_frame_txbuffer.pointer++ = field->id;
    *protocol_frame_txbuffer.pointer++ = field->length;
    field_start += 2U; //If head is enabled field_start has to be incremented by the two head bytes, so that it will point to where plain data starts.
  }
  for(;protocol_frame_txbuffer.pointer < (field_start + field->length);){
    *protocol_frame_txbuffer.pointer++ = *data++;
  }
  if(field->crc){
    crc = comm_crc16_engine(field_start, field->length, protocol_crc);
    *protocol_frame_txbuffer.pointer++ = crc;
    *protocol_frame_txbuffer.pointer = (crc >> 8);
  }
}

/**
 * This function prepares a data array which is fed to the rs_encode funtion. This is necessary because the vaisala protocol uses a special data order.
 * RS(255,231) is used to generate 24 byte of parity data, but for a complete frame 48 byte of parity data have to be generated. This is acheived by
 * performing two runs, the first one starts at array offset 'protocol_f_frametype.offset' and from there on it will take every even byte-offset and
 * write it into message. If frame end is encounterd from there on zero will be written into message (zero padding). The generated parity data is
 * wirtten at array offset 'protocol_f_ecc.offset'. The second run is started at offset 'protocol_f_frametype.offset + 1' and from there on it will
 * take every odd byte-offset and write it into message. Zero padding is performed here aswell. The generated parity data is written at array
 * offset 'protocol_f_ecc.offset' plus the size of one parity data set.
 * @param rs Points to a rs_t struct which holds all the necessary parameters that characterize the reed-solomon properties.
 * @param frame Points to a frame struct whose data is source to rs calculation and whose buffer partity data gets written to.
 */
void protocol_ecc_engine(RS_t* rs,frame_t* frame){
  uint8_t message[RS_ORDER - 1] = {0};
  uint16_t offset = 0;
  for(uint16_t j = 0; j <= 1; j++){
    for(uint16_t i = 0; i < rs->K; i++){
      if(&frame->buffer[protocol_f_frametype.offset + 2*i + j] < frame->end){
        message[i + rs->R] = frame->buffer[protocol_f_frametype.offset + 2*i + j];
      }
      else{
        message[i + rs->R] = 0;
      }
    }
    offset = j ? (protocol_f_ecc.offset + rs->R) : protocol_f_ecc.offset;
    rs_encode(rs, message, &frame->buffer[offset]);
  }
}