/**
  ******************************************************************************
  * @文件名     ： bsp_can.h
  * @作者       ： strongerHuang
  * @版本       ： V1.0.0
  * @日期       ： 2018年11月14日
  * @摘要       ： CAN底层头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _conf_CAN_H
#define _conf_CAN_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "device_bsp.h"
extern CAN_HandleTypeDef hcan1;



#define SJW SyncJumpWidth
#define BS1 TimeSeg1
#define BS2 TimeSeg2
#define TTCM TimeTriggeredMode
#define ABOM AutoBusOff
#define AWUM AutoWakeUp
#define NART AutoRetransmission
#define RFLM ReceiveFifoLocked
#define TXFP TransmitFifoPriority



/* 宏定义 --------------------------------------------------------------------*/
#if 1 /* 主站板卡定义 */
#define CANx                      CAN1
#define pHCANx                     & hcan1
#define CAN_RX_PIN                GPIO_PIN_0
#define CAN_TX_PIN                GPIO_PIN_1
#define CAN_GPIO_PORT             GPIOB                    //同一个PORT
#define CAN_RX_Priority           10                       //中断函数接口(见stm32f10x_it.c)
#define CAN_RX_IRQHandler         CAN1_RX1_IRQHandler

#else /* 从站板卡定义 */

#define CANx                      CAN1
#define CAN_CLK                   RCC_APB1Periph_CAN1
#define CAN_GPIO_CLK              RCC_APB2Periph_GPIOA
#define CAN_RX_PIN                GPIO_Pin_11
#define CAN_TX_PIN                GPIO_Pin_12
#define CAN_GPIO_PORT             GPIOA                    //同一个PORT
#define CAN_RX_IRQn               USB_LP_CAN1_RX0_IRQn
#define CAN_RX_Priority           10                       //中断函数接口(见stm32f10x_it.c)
#define CAN_RX_IRQHandler         USB_LP_CAN1_RX0_IRQHandler
#endif


typedef struct {
	CAN_TxHeaderTypeDef head;
	uint8_t Data[8];
} CanTxMsg;


typedef struct {
	CAN_RxHeaderTypeDef head;
	uint8_t Data[8];
} CanRxMsg;


#define MAIL uint32_t
#define Error HAL_StatusTypeDef

/* 函数申明 ------------------------------------------------------------------*/
void MX_CAN1_Init(uint32_t mode);
void CanFilter_Init(CAN_HandleTypeDef* hcan, uint32_t FILTER_FIFO);
void MX_CAN1_Test_Init(uint32_t mode);
#endif /* _BSP_CAN_H */

/**** Copyright (C)2018 strongerHuang. All Rights Reserved **** END OF FILE ****/
