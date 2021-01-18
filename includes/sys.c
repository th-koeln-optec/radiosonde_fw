/// @file sys.c
/*
 * Filename:            sys.c
 * Description:         This file containts housekeeping related functions.
 * Author:              M. Malyska
 */

#include "sys.h"

/**
 * Performs system related initialization
 */
void sys_init(void){
  SystemInit();
}

/**
 * This function call be call if an error is catched somewhere
 * @param error_type Provide an error value which enables tracing from where this error handler was called.
 */
void sys_error_handler(sys_error_t error_type){
  sys_error_t sys_error_type = error_type;
  while(1);
}