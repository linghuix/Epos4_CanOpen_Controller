
#ifndef __delay_H
#define	__delay_H

#include "stm32f10x.h"

/*****************  macro宏 ***************************************************/
/* 根据需求取消注释 */

//#define US_DELAY	USE
#define MS_DELAY	USE

/*****************  global variable 全局变量***************************************************/
extern volatile u32 TimingDelay;                                    //全局变量，使得中断函数能够操作TimingDelay. 易变

/*****************  function declaration 函数声明********************************************/

void Delay(__IO u32 nCount);                                        //不精确的延迟

	#if defined MS_DELAY	
        void SysTick_ms_Init(void);
        void Delay_ms(__IO u32 nTime);

	#elif defined US_DELAY	
        void SysTick_us_Init(void);
        void Delay_us(__IO u32 nTime);
	#endif
	
	#if (defined MS_DELAY) || (defined US_DELAY)
        void TimingDelay_decrement(void);							//调用外部函数
        void SysTick_Init(void);

	#endif

#endif /* __DELAY_H */
