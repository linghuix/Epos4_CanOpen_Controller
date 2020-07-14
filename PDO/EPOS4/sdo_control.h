#ifndef __sdo_control_H
#define __sdo_control_H

#include "epos_bsp.h"

extern uint8_t waiting_sdo;
extern CanRxMsg RxMessage;

uint8_t SDO_Write(Epos* epos1,Uint32 Index_Type,Uint8 SubIndex,Uint32 param);
uint32_t SDO_Read(Epos* epos,Uint32 Index_Type,Uint8 SubIndex);

#endif

