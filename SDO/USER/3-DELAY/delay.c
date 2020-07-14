/**
  ******************************************************************************
  * @file    delay.c
  * @author  Linghui Xu
  * @version V1.0
  * @date    2019.4.27
  * @brief   用3.5.0版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */



#include "delay.h"
volatile u32 TimingDelay;

/**
  * @brief  用于延时的嘀嗒时钟初始化
  * @param  None
  * @retval None
  */
void SysTick_Init(){
	
	#if defined US_DELAY	
	SysTick_us_Init();
	
	#elif defined MS_DELAY	
	SysTick_ms_Init();
	
	#endif
}


/**
  * @brief  不精确的延迟
  * @param  32位无符号整数
  * @retval None
  */
void Delay(__IO u32 nCount)	 //
{
	for(; nCount != 0; nCount--);
} 


/**
  * @brief 精确毫秒延迟
  * @param  32位无符号整数
  * @retval None
  */
void Delay_ms(__IO u32 nTime)
{ 
  TimingDelay = nTime;
  
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                     //使能嘀嗒定时器
  
  while(TimingDelay != 0);                                      //等待中断，延时
}


/**
  * @brief 精确微秒延迟
  * @param  32位无符号整数
  * @retval None
  */
void Delay_us(__IO u32 nTime)
{ 
  TimingDelay = nTime;

  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

  while(TimingDelay != 0);
}


/**
  * @brief 时钟初始化
  * @param  32位无符号整数
  * @retval None
  * @attention
  *     SystemCoreClock / 1000      1ms中断一次
  *     SystemCoreClock / 100000    10us中断一次
  *     SystemCoreClock / 1000000   1us中断一次
  *     f = 72000000HZ
  *     time = (72000000/1000) * 1/72000000 = 1ms
  */
void SysTick_ms_Init(void){

while(SysTick_Config( SystemCoreClock/1000));                               //Systick 配置延时n*ms	
NVIC_SetPriority (SysTick_IRQn, 0);
SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                                  //SysTick_Config配置时已经打开。使用定时器前先关闭

}


void SysTick_us_Init(void){	

while(SysTick_Config( SystemCoreClock/1000000));

SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


/**
  * @brief Systick中断服务函数
  * @param  None
  * @retval None
  */
void TimingDelay_decrement(void){
    
    if (TimingDelay != 0x00){  TimingDelay--; }
}
