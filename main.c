/// @file main.c
/*
 * Filename:            main.c
 * Description:         This file is the entry point for program execution.
 * Author:              M. Malyska
 */
#include "stm32f1xx.h"
#include "spi.h"
#include "rcc.h"
#include "gpio.h"
#include "timer.h"
#include "comm.h"

uint32_t frame_flag = 0;

/**
 * After the startup code finishes it will call this function, here is the entry point for user code exeution.
 */
void main(void)
{
  sys_init();
  rcc_init();
  gpio_init();
  spi1_init();
  comm_hal_init();
  comm_init();
  timer_init();

  uint8_t test_data[40] = {
  0x03, 0x1e, 0x50, 0x32, 0x37, 0x34, 0x30, 0x33,
  0x38, 0x37, 0x1a, 0x00, 0x00, 0x03, 0x00, 0x00,
  0x15, 0x00, 0x00, 0x5d, 0x00, 0x07, 0x32, 0x20,
  0xc9, 0x66, 0xb5, 0x41, 0x00, 0x00, 0x40, 0x40,
  0xff, 0xff, 0xff, 0xc6, 0xff, 0xff, 0xff, 0xc6
  };
  protocol_field_write(&status, test_data);


  while(1){
    if(tick_flag){
      tick_flag = 0x00;
      comm_fifo_tx_fsm();
      if(frame_flag == 0){
        comm_frame_send();
        frame_flag = 1000;
      }
      else{
        frame_flag--;
      }
    }
  }
}

