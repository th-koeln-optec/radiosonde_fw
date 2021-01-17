#include "stm32f1xx.h"
#include "spi.h"
#include "rcc.h"
#include "gpio.h"
#include "radio.h"
#include "comm.h"

uint16_t temp = 0;
uint8_t tick_flag = 0;
uint32_t frame_flag = 0;

void TIM6_IRQHandler(void);
void timer_init(void);

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

void timer_init(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN_Msk;
  NVIC_EnableIRQ(TIM6_DAC_IRQn);
  TIM6->DIER |= TIM_DIER_UIE_Msk;
  TIM6->PSC = 0x0007;
  TIM6->ARR = 0x03e8;
  TIM6->CR1 |= TIM_CR1_CEN_Msk;
}

void TIM6_IRQHandler(void){
  tick_flag = 1;
  TIM6->SR &= ~(TIM_SR_UIF_Msk);
}