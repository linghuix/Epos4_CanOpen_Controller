
/*----------------------------------------------------------------------------
  更新日志:
  2018-11-14 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "func_CanOpen.h"


/**************************************** CANOpen定时 ****************************************/
/************************************************
函数名称 ： CANOpen_TIM_Configuration
功    能 ： CANOpen定时配置
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void CANOpen_Inital(void)
{
	MX_CAN1_Init(CAN_MODE_NORMAL);
	MX_TIM_CounterInterrupt(CANOPEN_TIMx, CANOPEN_TIM_COUNTER_CLOCK, CANOPEN_TIM_PERIOD);//

	CAN_Start(&hcan1);
	HAL_TIM_Base_Start_IT(CANOPEN_TIMx_handle);

}


/*发送CAN队列*/
OS_EVENT * CANSend_Q;
void *cantxMsgGrp[N_MESSAGES];				// 定义队列消息指针数组
CanTxMsg cantxmsg[N_MESSAGES];				// 数据缓冲区(消息指针对应的数据)


/*接收CAN队列*/
OS_EVENT * CANRcv_Q;
void *canrxMsgGrp[N_MESSAGES];
CanRxMsg canrxmsg[N_MESSAGES];


/* 定时器TIM相关变量 */
static TIMEVAL last_counter_val = 0;
static TIMEVAL elapsed_time = 0;


/********************************** CANOpen接口函数(需自己实现) **********************************/



/************************************************
功    能 ： CAN接收数据, 在接收中断函数中调用
参    数 ： RxMsg --- 接收数据(队列)
返 回 值 ： 无
作    者 ： xlh
*************************************************/
INT8U rxindex = 0;

#ifndef TEST_ON
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RCV_MSG("can Qrcv %u\r\n",OSTime);
	CANRcv_DateFromISR();
}
#endif


void CANRcv_DateFromISR(void)
{
	INT8U error;
	error = MX_CANx_get(& hcan1, & canrxmsg[rxindex], CAN_FILTER_FIFO0);

	if(error == HAL_ERROR){
		MSG("CAN RECEIVE Err\r\n");
		return;
	}

	error = OSQPost (CANRcv_Q, &canrxmsg[rxindex]);
	if(error == OS_ERR_Q_FULL){
		MSG("CAN RECEIVE IS FULL\r\n");
	}
	else{
		rxindex++;															//**执行下个缓冲区，避免覆盖原来的按键数据**/
		if(rxindex == N_MESSAGES)
			rxindex=0;
		//OSSemPost(CRCVSemp);
	}
}

/************************************************
功    能 ： 定时调度(从定时器中断),
参    数 ： 无
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void TIMx_DispatchFromISR(void)
{
	last_counter_val = 0;
	elapsed_time = 0;
	TimeDispatch();				// canopen 定时调度

}

/************************************************
功    能 ： 获取从上一次中断触发以来经过的时间
参    数 ： 无
返 回 值 ： (消逝的)时间
作    者 ： strongerHuang
*************************************************/
TIMEVAL getElapsedTime(void)
{
  uint32_t timer = CANTIM_GetCounter(); // Copy the value of the running timer

  if(timer < last_counter_val)	//可以实现超过TIM最大定时时间的定时
    timer += CANOPEN_TIM_PERIOD;

  TIMEVAL elapsed = timer - last_counter_val + elapsed_time;
  //若中断前加入两次新的定时事件，必须加上旧的elapsed_time

  return elapsed;
}

/************************************************
功    能 ： Set the timer for the next alarm.
参    数 ： value --- 参数
返 回 值 ： 无
作    者 ： strongerHuang
*************************************************/
void setTimer(TIMEVAL value)
{
  uint32_t timer = CANTIM_GetCounter();

  elapsed_time += timer - last_counter_val;
  last_counter_val = CANOPEN_TIM_PERIOD - value;

  //强制修改定时器计数，因此有elapsed_time
  CANTIM_SetCounter(CANOPEN_TIM_PERIOD - value);
  //CANOpenTIMCounter_start();
//  HAL_TIM_Base_Start_IT(CANOPEN_TIMx_handle);
}

/************************************************
功    能 ： CAN发送
参    数 ： notused --- 未使用参数
            m --------- 消息参数
返 回 值 ： 0:失败  1:成功
作    者 ： xlh
*************************************************/
INT8U txindex = 0;
unsigned char canSend(CAN_PORT notused, Message *m)
{
	uint8_t i,err;

	cantxmsg[txindex].head.StdId = m->cob_id;

	if(m->rtr)
		cantxmsg[txindex].head.RTR = CAN_RTR_REMOTE;
	else
		cantxmsg[txindex].head.RTR = CAN_RTR_DATA;

	cantxmsg[txindex].head.IDE = CAN_ID_STD;
	cantxmsg[txindex].head.DLC = m->len;
	for(i=0; i<m->len; i++)
		cantxmsg[txindex].Data[i] = m->data[i];

	// 判断是否在执行中断
	if((err=OSQPost(CANSend_Q, &cantxmsg[txindex])) != OS_ERR_NONE){  //加入队列失败
		if(err == OS_ERR_Q_FULL)
			
			ERROR(5,"CANSend_Q is full  Size>%d", ((OS_Q *)CANSend_Q->OSEventPtr)->OSQSize);
	  return 0xFF;
	}
	else{
		txindex++;									// 执行下个缓冲区，避免覆盖原来的数据
		if(txindex == N_MESSAGES)
			txindex=0;
	}
	//printf("Send\r\n");
	return 0;
}



/**** Copyright (C)2018 strongerHuang. All Rights Reserved **** END OF FILE ****/
