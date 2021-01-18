/// @file radio_comm.h
/*
 * Filename:            radio_comm.h
 * Description:         This file contains the RADIO communication layer.
 * Author:              Copyright 2011 Silicon Laboratories, Inc.
 * Edited by:           M. Malyska; The original file can be found in the same directory: radio_comm.h.bak
 */

#ifndef RADIO_COMM_H
#define RADIO_COMM_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include <stdint.h>
#include "radio_hal.h"

/*GLOBAL VARIABLES*/
//extern SEGMENT_VARIABLE(radioCmd[16u], uint8_t, SEG_XDATA);

/*enumERATORS*/

/*typedefINITIONS*/

/*DEFINES*/
#define RADIO_CTS_TIMEOUT 10000

/*PUBLIC PROTOTYPES*/
uint8_t radio_comm_GetResp(uint8_t byte_count, uint8_t* pData);
void radio_comm_SendCmd(uint8_t byte_count, uint8_t* pData);
void radio_comm_ReadData(uint8_t cmd, uint8_t pollCts, uint8_t byte_count, uint8_t* pData);
void radio_comm_WriteData(uint8_t cmd, uint8_t pollCts, uint8_t byte_count, uint8_t* pData);

uint8_t radio_comm_PollCTS(void);
uint8_t radio_comm_SendCmdGetResp(uint8_t cmdByteCount, uint8_t* pCmdData, \
                             uint8_t respByteCount, uint8_t* pRespData);
void radio_comm_ClearCTS(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* RADIO_COMM_H */