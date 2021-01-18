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

