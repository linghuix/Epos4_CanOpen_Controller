#include "func_global.h"

void Core_Config(void){
	
	Flash_prefetch();
	NVIC_Init();
	SystemClock_Config();
	tick_init(1000);
}

