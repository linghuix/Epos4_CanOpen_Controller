#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "epos.h"

extern CanTxMsg TxMessage;                     //发送缓冲区
extern CanRxMsg RxMessage;                     //接收缓冲区

static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);
static void CAN_Mode_Config(void);
static void CAN_Filter_Config(void);
void CAN_Config(void);
//void CAN_SetMsg(uint32_t StdId, uint32_t low_4byte, uint32_t high_4byte);
void CAN_SetMsg(uint32_t ID, uint32_t low, uint32_t high);

#endif
