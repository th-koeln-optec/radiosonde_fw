/// @file timer.h
/*
 * Filename:            timer.h
 * Description:         This file containts timer related function headers.
 * Author:              M. Malyska
 */

#ifndef TIMER_H
#define TIMER_H
#ifdef _cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>

/*GLOBAL VARIABLES*/
extern uint8_t tick_flag;
/*enumERATORS*/

/*typedefINITIONS*/

/*DEFINES*/

/*PUBLIC PROTOTYPES*/
void timer_init(void);
void TIM6_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TIMER_H */