#ifndef __SDO_H
#define __SDO_H

#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "epos.h"

extern CanRxMsg RxMessage;

void SDO_Write(Epos* epos1,Uint16 Index,Uint8 SubIndex,Uint32 param);
//void SDO_Read(Epos* epos,Uint16 Index,Uint8 SubIndex);
Uint32 SDO_Read(Epos* epos,Uint16 Index,Uint8 SubIndex);

#endif

