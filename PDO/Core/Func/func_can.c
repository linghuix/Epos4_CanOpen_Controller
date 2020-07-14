/*
 * func_can.c
 *
 *  Created on: Mar 14, 2020
 *      Author: test
 */
#include "func_can.h"


#include "ucos_ii.h"
Error MX_CANx_send(CAN_HandleTypeDef *phcan, CanTxMsg *msg, MAIL pmailbox)
{
	Error error;

	error = HAL_CAN_AddTxMessage(phcan, (CAN_TxHeaderTypeDef *)& (msg->head), msg->Data, & pmailbox);
	
	if(error == HAL_OK){
		CAN_SEND_MSG("%u CAN Send 0x%x  ",OSTime, msg->head.StdId);
		for(int i=0; i < msg->head.DLC; i++){
			CAN_SEND_MSG("-0x%x ",msg->Data[i]);
		}
		CAN_SEND_MSG("\r\n");
	}
	return error;
}

Error MX_CANx_get(CAN_HandleTypeDef *phcan, CanRxMsg *msg, uint32_t FIFO)
{
	Error error;
	error = HAL_CAN_GetRxMessage(phcan, FIFO, (CAN_RxHeaderTypeDef *)& (msg->head), (msg->Data));
	return error;
}


void CAN_Start(CAN_HandleTypeDef *phcan)
{
	if(HAL_CAN_Start(phcan) != HAL_OK){
		Error_Handler()
	}
}



/*
 * author lhx
 *
 * @brief : tttttttttttttttttttttttttesttttttttttttttttttttttttttt
 * Window > Preferences > C/C++ > Editor > Templates.
 */
#ifdef TEST_ON
CanTxMsg txmsg; // CAN_TX_MAILBOX0|CAN_TX_MAILBOX1|CAN_TX_MAILBOX2;
CanRxMsg rxmsg;
int Flag_receive;
void CAN_Send_test(void)
{
	printf("CAN test beginning ...\r\n");

	int canerror = 0;
	uint32_t mailbox = 0;
	uint32_t msg[8]={0x12,0x23,0x34,0x45,0,0,0,0};

	//MX_CAN1_Init(CAN_MODE_LOOPBACK);
	MX_CAN1_Init(CAN_MODE_NORMAL);
	CAN_Start(&hcan1);

	txmsg.head.StdId = 100;
	txmsg.head.DLC = 8;
	txmsg.head.RTR = CAN_RTR_DATA;
	txmsg.head.IDE = CAN_ID_STD;

	for(int i = 0;i < 8;i++){
		txmsg.Data[i] = msg[i];
	}

	int i;
	while(1)
	{
		if(i++ > 500000){//周期 1.2s 的样子
			i = 0;
			canerror = MX_CANx_send(&hcan1, &txmsg, mailbox);
			//while(HAL_CAN_IsTxMessagePending(&hcan1, CAN_TX_MAILBOX0|CAN_TX_MAILBOX1|CAN_TX_MAILBOX2));
			/*if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0)>0) {
				Flag_receive++;
				canerror = MX_CANx_get(&hcan1, &rxmsg, CAN_RX_FIFO0);
				if(rxmsg.head.StdId == txmsg.head.StdId && rxmsg.Data[2]==txmsg.Data[2]){
					printf("CAN hardware good!!!!\r\n");
				}
			}*/

			for(int i = 0;i < 8;i++){
					txmsg.Data[i] = txmsg.Data[i]+1;
				}
		}
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	int canerror = 0;
	if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0)>0) {
		canerror = MX_CANx_get(&hcan1, &rxmsg, CAN_RX_FIFO0);
		printf("CAN receive -%ld ",rxmsg.head.StdId);
		for(int i=0;i<8;i++){
			printf("-%d",rxmsg.Data[i]);
		}
		printf("\r\n");
	}
}
#endif
