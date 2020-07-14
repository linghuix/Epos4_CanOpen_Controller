/**
  ******************************************************************************
  * @文件名     ： canfestival.h
  * @作者       ： strongerHuang
  * @版本       ： V1.0.0
  * @日期       ： 2018年11月14日
  * @摘要       ： CANOpen底层驱动接口定义头文件
                   本工程主要实现canSend函数，位于App\canopen\canopen_drv.c
                   其它接口在本工程未使用(通过bsp下其它类似函数实现了)
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _CANFESTIVAL_H
#define _CANFESTIVAL_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "applicfg.h"
#include "data.h"


/* 函数申明 ------------------------------------------------------------------*/
void initTimer(void);
void clearTimer(void);

unsigned char canSend(CAN_PORT notused, Message *m);
unsigned char canInit(CO_Data * d, uint32_t bitrate);
void canClose(void);

void disable_it(void);
void enable_it(void);


#endif /* _CANFESTIVAL_H */

/**** Copyright (C)2018 strongerHuang. All Rights Reserved **** END OF FILE ****/
