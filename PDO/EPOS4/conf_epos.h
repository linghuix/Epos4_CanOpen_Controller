#ifndef __CONF_EPOS_H
#define __CONF_EPOS_H

#define SDO

#include "epos_bsp.h"
//below include is related with Epos struct
#include "sdo_control.h"
#include "dict_config.h"
#include "nmt_control.h"

//------------------------------------------------------------------------
//define functions
//------------------------------------------------------------------------




void EposMaster_Init(void);
void EposMaster_Start(void);
void EposMaster_Stop(void);

void Node_PDOConfig(Epos* A);
void NMT_Start(Epos* epos, Uint32 ID);
void NMT_Pre(Epos* epos, Uint32 ID);

void Print(CanRxMsg RxMessage);//打印接收到的CAN帧
        
//EPOS

void Node_StructInit(Epos* epos1, Uint8 CAN_ID, Uint8 NODE_ID);

void Node_ParamConfig(Epos* epos);

void Epos_Delay(Uint32 time);

void Node_setMode(Epos* epos, Uint16 mode);        //设定控制器的工作模式

void Node_OperEn(Epos* epos);                   //从 Switch On Disable 到 Operation Enable
void Node_DisEn(Epos* epos);

//Control Funtional
void EPOS_SetAngle(Epos* epos, Uint32 angle);//Postion Mode

#endif
