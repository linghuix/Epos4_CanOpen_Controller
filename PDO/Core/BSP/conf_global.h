#ifndef __conf_global_H
#define __conf_global_H

#include "device_bsp.h"
//#include "stm32f1xx_hal_rcc_ex.h"


void SystemClock_Config(void);
void tick_init(uint32_t period);
void NVIC_Init(void);
void Flash_prefetch(void);

#endif
