/// @file rcc.c
/*
 * Filename:            rcc.c
 * Description:         This file containts clock related functions.
 * Author:              M. Malyska
 */

#include "rcc.h"

/**
 * Calls all clock related init functions
 */
void rcc_init(void){
  rcc_clk_init();
}

/**
 * Initializes the clocking system
 * @return Returns the error type so that the error handler can be invoked afterwards if necessary.
 */
sys_error_t rcc_clk_init(void){
  RCC->CR |= RCC_CR_HSEON_Msk; //Enable/disable ext osc
  while(!(RCC->CR & RCC_CR_HSERDY_Msk)); //Wait for ext osc to start
  
  RCC->CR &= ~(RCC_CR_PLLON_Msk); //Enable/disable pll prior to config

  RCC->CFGR &= ~(RCC_CFGR_SW_Msk); //System clk src selection, clearing all bits
  while(RCC->CFGR & RCC_CFGR_SWS_Msk == RCC_CFGR_SWS_HSI);
  if((RCC->CFGR & RCC_CFGR_SWS_Msk == RCC_CFGR_SWS_HSI)) //Check if switching succeeded
    return sys_error_rcc;
  RCC->CFGR |= RCC_CFGR_SW_HSE; //System clk src selection
  while(RCC->CFGR & RCC_CFGR_SWS_Msk == RCC_CFGR_SWS_HSE);
  if((RCC->CFGR & RCC_CFGR_SWS_Msk == RCC_CFGR_SWS_HSE)) //Check if switching succeeded
    return sys_error_rcc;
  
  RCC->CFGR &= ~(RCC_CFGR_PPRE1_Msk); //APB2 clk divider selection, clearing all bits
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV1; //APB2 clk divider selection
  RCC->CFGR &= ~(RCC_CFGR_PPRE2_Msk); //APB2 clk divider selection, clearing all bits
  RCC->CFGR |=  RCC_CFGR_PPRE2_DIV1; //APB2 clk divider selection

  return sys_error_none;
}