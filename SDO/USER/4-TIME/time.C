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
#include "timer.h"
#include "HW_epos.h"

extern int angle_1[323];
extern int angle_2[404];
extern Epos Controller;
extern Epos Controller1;

extern Uint32 pos;										//电机位置
extern int x;												//角度自变量 extern int x=0;是错的，不能再一次初始化
extern int angle_sensor;


extern __IO uint32_t flag;	

/**
  * @brief  TIM中断优先级配置
  * @param  None
  * @retval None
  */
void TIME_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                         /*设置NVIC中断分组2:2位抢占优先级，2位响应优先级*/

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                         /*TIM2中断*/ 

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               /*先占优先级1级*/

    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;                      /*从优先级2级*/ 
        
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         /*IRQ通道被使能*/ 

    NVIC_Init(&NVIC_InitStructure);                                         /*根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器*/
}


/**
  * @brief  TIM3 Base 单元配置，中断使能
  * @param  None
  * @retval None
  */
void TIME3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                                              /*定时器TIM3时钟使能*/

    TIM_TimeBaseStructure.TIM_Period = 10000;                                                         /*设置在下一个更新事件装入活动的自动重装载寄存器周期的值,计数到5000为500ms*/

    TIM_TimeBaseStructure.TIM_Prescaler =(7200-1);                                                    /*设置用来作为TIMx时钟频率除数的预分频值72MHz / (7200) = 10000Hz*/

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                                      /*设置时钟分割:TDTS = Tck_tim*/ 

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                                       /*TIM向上计数模式*/

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                                                   /*根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位*/

    //TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);  主程序中控制                           /*使能或者失能指定的TIM中断*/

    TIM_Cmd(TIM3, ENABLE);                                                                            //使能TIMx外设
}



/**
  * @brief  TIME2 Base 单元配置，中断使能
  * @param  None
  * @retval None
  */
void TIME2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);                    /*定时器TIM2时钟使能*/
    
    TIM_TimeBaseStructure.TIM_Period = 8000;                              /*设置在下一个更新事件装入活动的自动重装载寄存器周期的值,计数到10000为10ms*/
    
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1);                            /*设置用来作为TIMx时钟频率除数的预分频值 72MHz/72 = 1MHz */
    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                            /*设置时钟分割:TDTS = Tck_tim*/   
    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             /*TIM向上计数模式*/ 
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                         /*根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位*/ 
    
    //TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);               /*主程序中控制,使能或者失能指定的TIM中断*/
    
    TIM_Cmd(TIM2, ENABLE);                                                  //使能TIMx外设
}


void TIM2_IRQHandler(void){

	//Epos_Conroller_TIMBack();

}

