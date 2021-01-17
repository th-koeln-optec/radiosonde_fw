#include "sys.h"

void sys_init(void){
  SystemInit();
}

void sys_error_handler(sys_error_t error_type){
  sys_error_t sys_error_type = error_type;
  while(1);
}