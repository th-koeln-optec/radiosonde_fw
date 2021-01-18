/// @file timer.c
/*
 * Filename:            timer.c
 * Description:         This file containts timer related functions.
 * Author:              M. Malyska
 */

#include "timer.h"

uint8_t tick_flag = 0x00;

/**
 * Initializes timer6 peripheral and starts it.
 */
void timer_init(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN_Msk;
  NVIC_EnableIRQ(TIM6_DAC_IRQn);
  TIM6->DIER |= TIM_DIER_UIE_Msk;
  TIM6->PSC = 0x0007;
  TIM6->ARR = 0x03e8;
  TIM6->CR1 |= TIM_CR1_CEN_Msk;
}

/**
 * Timer6 interrupt service routine, if a tim6 interrupt is issue this function will be called
 */
void TIM6_IRQHandler(void){
  tick_flag = 1;
  TIM6->SR &= ~(TIM_SR_UIF_Msk);
}