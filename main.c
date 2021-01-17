#include "stm32f1xx.h"
#include "spi.h"
#include "rcc.h"
#include "gpio.h"
#include "timer.h"
#include "radio.h"
#include "comm.h"

uint32_t frame_flag = 0;

void main(void)
{
  sys_init();
  rcc_init();
  gpio_init();
  spi1_init();
  radio_init();
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

