/// @file protocol.h
/*
 * Filename:            protocol.h
 * Description:         This file containts protocol.c related function headers.
 * Author:              M. Malyska
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//#define PROTOCOL_XDATA
#ifdef PROTOCOL_XDATA
#define COMM_FRAME_BUFFER_SIZE 518
#else
#define COMM_FRAME_BUFFER_SIZE 320
#endif

/*INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>
#include "comm.h"

/*DEFINES*/
#define PROTOCOL_TYPE VAISALA



#define PROTOCOL_XOR_ENABLE
//#define PROTOCOL_ECC_ENABLE

#define PROTOCOL_CRC_INITIAL 0xffff
#define PROTOCOL_CRC_GENERATOR 0x1021 //CRC16-CCITT Generator polynomial
#define PROTOCOL_CRC_BYTESWAP 0xff //0xff if crc low-byte and crc-highbyte should be swaped, 0x00 if not

/*typedefINITIONS*/
/**
 * This struct holds the definitions of all the diffrent fields of a frame.
 * id is a field identifier, if head is enabled this will be writen in the first byte of a field
 * offset is a the position of the field inside the frame, it states the position of the first field byte
 * length is the amount of pure data bytes, without the two head bytes and without the two crc bytes
 * crc enables/disables the generation and the suffixing of the crc sum
 * head enables/disables the prefixing of the id field and the length field
 */
typedef struct {
  uint8_t id;
  uint16_t offset;
  uint8_t length;
  uint8_t crc;
  uint8_t head;
}field_t;

/*GLOBAL VARIABLES*/
extern xor_mask_t protocol_xor;
extern crc_t protocol_crc;

extern const uint8_t protocol_header[];
extern const uint8_t protocol_frametype_regular[];
extern const uint8_t protocol_frametype_extended[];

extern const field_t protocol_f_header;
extern const field_t protocol_f_ecc;
extern const field_t protocol_f_frametype;
extern const field_t protocol_f_status;
extern const field_t protocol_f_meas;
extern const field_t protocol_f_gpsinfo;
extern const field_t protocol_f_gpsraw;
extern const field_t protocol_f_gpspos;
extern const field_t protocol_f_empty;


/*VARIABLES*/

/*enumERATORS*/

/*PUBLIC PROTOTYPES*/
void protocol_init(void);
void protocol_field_write(const field_t* field, uint8_t* data);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* PROTOCOL_H */
