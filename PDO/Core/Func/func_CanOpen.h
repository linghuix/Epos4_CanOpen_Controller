/**
  ******************************************************************************
  * @文件名     ： bsp_timer.h
  * @作者       ： strongerHuang
  * @版本       ： V1.0.0
  * @日期       ： 2018年11月14日
  * @摘要       ： TIM底层头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef __func_canopenTIMER_H
#define __func_canopenTIMER_H


/* 包含的头文件 --------------------------------------------------------------*/
#include "conf_tim.h"
#include "func_can.h"
#include "app.h"


/* 宏定义 --------------------------------------------------------------------*/
/* CANOpen定时 */
#define CANOPEN_TIMx              TIM2
#define CANOPEN_TIMx_handle       & htim2
#define CANTIM_SetCounter(count) setTimeCount(CANOPEN_TIMx, count)
#define CANTIM_GetCounter() getTimeCount(CANOPEN_TIMx)
#define CANOpenTIMCounter_start() TimeCounter_Start(CANOPEN_TIMx_handle)
#define CANOpenTIM_stop() TimeCounter_Stop(CANOPEN_TIMx_handle)
#define CANOPEN_TIM_CLK           RCC_APB1Periph_TIM2
#define CANOPEN_TIM_IRQn          TIM2_IRQn
#define CANOPEN_TIM_Priority      12
#define CANOPEN_TIM_IRQ_Handler   TIM2_IRQHandler
#define CANOPEN_TIM_COUNTER_CLOCK           100000         //计数时钟(100K次/秒)                                                          //预分频值
//#define CANOPEN_TIM_PRESCALER_VALUE         (SystemFrequency/CANOPEN_TIM_COUNTER_CLOCK - 1)
#define CANOPEN_TIM_PERIOD                  65535          //定时周期


#define N_MESSAGES 100

/* 函数申明 ------------------------------------------------------------------*/

#include <dict.h>
#include "ucos_ii.h"

//定义队列消息指针数组,存储的是 void 指针
extern void *cantxMsgGrp[N_MESSAGES];
extern void *canrxMsgGrp[N_MESSAGES];

//数据缓冲区(消息指针对应的数据)，存储的是上面指针指向的数据
extern CanTxMsg cantxmsg[N_MESSAGES];
extern CanRxMsg canrxmsg[N_MESSAGES];

extern OS_EVENT * CANRcv_Q;
extern OS_EVENT * CANSend_Q;


void TIMx_DispatchFromISR(void);
void CANRcv_DateFromISR(void);

void CANOpen_Inital(void);

#endif /* _BSP_TIMER_H */

/**** Copyright (C)2018 strongerHuang. All Rights Reserved **** END OF FILE ****/
