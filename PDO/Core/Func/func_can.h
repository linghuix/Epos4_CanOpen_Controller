/*
 * func_can.h
 *
 *  Created on: Mar 14, 2020
 *      Author: test
 */

#ifndef BSP_FUNC_CAN_H_
#define BSP_FUNC_CAN_H_

#include "conf_can.h"
#include "stdio.h"

void CAN_Send_test(void);

Error MX_CANx_send(CAN_HandleTypeDef *hcan, CanTxMsg *msg, MAIL pmailbox);
Error MX_CANx_get(CAN_HandleTypeDef *phcan, CanRxMsg *msg, uint32_t FIFO);
void CAN_Start(CAN_HandleTypeDef *phcan);

#endif /* BSP_FUNC_CAN_H_ */
