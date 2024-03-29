/// @file radio_comm.c
/*
 * Filename:            radio_comm.c
 * Description:         This file contains the RADIO communication layer.
 * Author:              Copyright 2011 Silicon Laboratories, Inc.
 * Edited by:           M. Malyska; The original file can be found in the same directory: radio_comm.c.bak
 */

#include "radio_comm.h"

/*GLOBAL VARIABLES*/
uint8_t ctsWentHigh = 0x00;

/**
 * Gets a command response from the radio chip
 * @param byteCount   Number of bytes to get from the radio chip
 * @param pData       Pointer to where to put the data
 * @return CTS value
 */
uint8_t radio_comm_GetResp(uint8_t byte_count, uint8_t* pData){
  //SEGMENT_VARIABLE(ctsVal = 0u, uint8_t, SEG_DATA);
  uint8_t ctsVal = 0x00;
  //SEGMENT_VARIABLE(errCnt = RADIO_CTS_TIMEOUT, uint16_t, SEG_DATA);
  uint16_t errCnt = RADIO_CTS_TIMEOUT;

  while (errCnt != 0){ //wait until radio IC is ready with the data
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(0x44); //read CMD buffer
    ctsVal = radio_hal_SpiReadByte();
    if (ctsVal == 0xFF){
      if (byte_count){
        radio_hal_SpiReadData(byte_count, pData);
      }
      radio_hal_SetNsel();
      break;
    }
    radio_hal_SetNsel();
    errCnt--;
  }

  if (errCnt == 0){
    while(1){
      /* ERROR!!!!  CTS should never take this long. */
      #ifdef RADIO_COMM_ERROR_CALLBACK
        RADIO_COMM_ERROR_CALLBACK();
      #endif
    }
  }

  if (ctsVal == 0xFF){
    ctsWentHigh = 1;
  }

  return ctsVal;
}

/**
 * Sends a command to the radio chip
 * @param byteCount   Number of bytes in the command to send to the radio device
 * @param pData       Pointer to the command to send.
 */
void radio_comm_SendCmd(uint8_t byte_count, uint8_t* pData){
    while (!ctsWentHigh){
      radio_comm_PollCTS();
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteData(byte_count, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

/**
 * Gets a command response from the radio chip
 * @param cmd         Command ID
 * @param pollCts     Set to poll CTS
 * @param byteCount   Number of bytes to get from the radio chip.
 * @param pData       Pointer to where to put the data.
 */
void radio_comm_ReadData(uint8_t cmd, uint8_t pollCts, uint8_t byte_count, uint8_t* pData){
    if(pollCts){
        while(!ctsWentHigh){
            radio_comm_PollCTS();
        }
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    radio_hal_SpiReadData(byte_count, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}


/**
 * Gets a command response from the radio chip
 * @param cmd         Command ID
 * @param pollCts     Set to poll CTS
 * @param byteCount   Number of bytes to get from the radio chip
 * @param pData       Pointer to where to put the data
 */
void radio_comm_WriteData(uint8_t cmd, uint8_t pollCts, uint8_t byteCount, uint8_t* pData){
    if(pollCts){
        while(!ctsWentHigh){
            radio_comm_PollCTS();
        }
    }
    radio_hal_ClearNsel();
    radio_hal_SpiWriteByte(cmd);
    radio_hal_SpiWriteData(byteCount, pData);
    radio_hal_SetNsel();
    ctsWentHigh = 0;
}

/**
 * Waits for CTS to be high
 * @return CTS value
 */
uint8_t radio_comm_PollCTS(void){
#ifdef RADIO_USER_CFG_USE_GPIO1_FOR_CTS
    while(!radio_hal_Gpio1Level()){
        /* Wait...*/
    }
    ctsWentHigh = 1;
    return 0xFF;
#else
    return radio_comm_GetResp(0, 0);
#endif
}

/**
 * Clears the CTS state variable.
 */
void radio_comm_ClearCTS(){
  ctsWentHigh = 0;
}

/**
 * Sends a command to the radio chip and gets a response
 * @param cmdByteCount  Number of bytes in the command to send to the radio device
 * @param pCmdData      Pointer to the command data
 * @param respByteCount Number of bytes in the response to fetch
 * @param pRespData     Pointer to where to put the response data
 * @return CTS value
 */
uint8_t radio_comm_SendCmdGetResp(uint8_t cmdByteCount, uint8_t* pCmdData, uint8_t respByteCount, uint8_t* pRespData){
  radio_comm_SendCmd(cmdByteCount, pCmdData);
  return radio_comm_GetResp(respByteCount, pRespData);
}

