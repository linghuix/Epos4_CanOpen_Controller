#ifndef __dict_config_H
#define __dict_config_H

#include "epos_bsp.h"

void Init_MyDict(void);

UNS32 Edit_Dict(CO_Data* d, uint32_t Index_Type,uint8_t SubIndex, void* pdata);
#endif

