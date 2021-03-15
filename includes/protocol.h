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

/*INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>
#include "comm.h"
#include "rs.h"

/*DEFINES*/
#define PROTOCOL_TYPE VAISALA

#define PROTOCOL_XOR_ENABLE
#define PROTOCOL_ECC_ENABLE

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
void protocol_frame_send(void);
void protocol_field_write(const field_t* field, const uint8_t* data);
void protocol_ecc_engine(RS_t* rs,frame_t* frame);

#ifdef COMM_RX_ENABLED
  uint16_t protocol_command_get(void);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* PROTOCOL_H */
