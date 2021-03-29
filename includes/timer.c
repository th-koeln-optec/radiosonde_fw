/// @file timer.c
/*
 * Filename:            timer.c
 * Description:         This file containts timer related functions.
 * Author:              M. Malyska
 */

#include "timer.h"

uint8_t send_flag = 0;

/**
 * Initializes timer4 peripheral and starts it.
 */
void timer_init(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN_Msk;
  NVIC_EnableIRQ(TIM4_IRQn);
  TIM4->PSC = 0x031f;
  TIM4->ARR = 0x2710;
  TIM4->EGR |= TIM_EGR_UG_Msk;
  TIM4->SR &= ~(TIM_SR_UIF_Msk);
  TIM4->DIER |= TIM_DIER_UIE_Msk;
  TIM4->CR1 |= TIM_CR1_CEN_Msk;
}

/**
 * Timer4 interrupt service routine, if a tim4 interrupt is issue this function will be called
 */
void TIM4_IRQHandler(void){
  if(TIM4->SR & TIM_SR_UIF_Msk){
    send_flag = 0xff;
    TIM4->SR &= ~(TIM_SR_UIF_Msk);
  }
}