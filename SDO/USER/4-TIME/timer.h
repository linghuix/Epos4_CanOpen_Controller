#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "epos.h"

/*****************  function declaration *******************************************/

void TIME_NVIC_Configuration(void);

void TIME3_Configuration(void);
void TIME2_Configuration(void);
 
void TIM2_interrupt(void);
#endif/* __TIMER_H */

