/// @file sys.h
/*
 * Filename:            sys.h
 * Description:         This file containts housekeeping related function headers.
 * Author:              M. Malyska
 */

#ifndef SYS_H
#define SYS_H
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>

/*GLOBAL VARIABLES*/

/*enumERATORS*/

/*typedefINITIONS*/
typedef enum {
  sys_error_none,
  sys_error_radio,
  sys_error_spi,
  sys_error_rcc,
  sys_error_comm
}sys_error_t;

/*DEFINES*/

/*PUBLIC PROTOTYPES*/
void sys_init(void);
void sys_error_handler(uint8_t error_type);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SYS_H */
