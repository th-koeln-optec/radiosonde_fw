#ifndef RCC_H
#define	RCC_H
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

/****INCLUDES*/
#include "stm32f1xx.h"
#include <stdint.h>
#include "sys.h"

/****GLOBAL VARIABLES*/
    
/****enumERATORS*/

/****typedefINITIONS*/

/*****SYSTEM TICK*/
    
/*****SYSTEM STATES*/
    
/****DEFINES*/

/****PUBLIC PROTOTYPES*/
void rcc_init(void);
sys_error_t rcc_clk_init(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */
#endif	/* RCC_H */
