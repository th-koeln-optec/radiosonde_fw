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

/*DEFINES*/
#define PROTOCOL_TYPE VAISALA

#define PROTOCOL_XOR_ENABLE
//#define PROTOCOL_ECC_ENABLE

#define PROTOCOL_CRC_INITIAL 0xffff
#define PROTOCOL_CRC_GENERATOR 0x1021 //CRC16-CCITT Generator polynomial
#define PROTOCOL_CRC_BYTESWAP 0xff //0xff if crc low-byte and crc-highbyte should be swaped, 0x00 if not

/*typedefINITIONS*/
typedef struct {
  uint16_t offset;
  uint8_t length;
  uint8_t crc;
}field_t;

/*GLOBAL VARIABLES*/
extern xor_mask_t protocol_xor;
extern crc_t protocol_crc16_ccitt;

/*VARIABLES*/

/*enumERATORS*/

/*PUBLIC PROTOTYPES*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* PROTOCOL_H */
