#include "dict_config.h"

/***** 
write parameter to Object Dictionary through CAN use SDO messages
******/
#define MS10 0x2710
#define MS5 0x1388
#define MS15 0x3A98
#define MS50 0xC350
#define MS20 0x4E20
#define MS100 0x186A0
#define MS500 0x7A120
#define S1 0xF4240




UNS32 Edit_Dict(CO_Data* d, uint32_t Index_Type,uint8_t SubIndex, void* pdata)
{
	UNS32 retcode;
	UNS16 Index = Index_Type>>16;
	UNS32 ByteSize = (Index_Type&(UNS32)0xFF)>>3;

	retcode = setODentry( d, Index, SubIndex, pdata, &ByteSize, 1 );

	if(retcode != OD_SUCCESSFUL){
		ERROR(0,"-Edit Dict Error- 0x%x",retcode);
		return 0;
	}
	
	return 1;
}


extern CO_Data TestMaster_Data;
extern UNS32 TestMaster_obj1006;
void Init_MyDict(void)
{
	uint32_t data;
	TestMaster_obj1006 = MS100;		//set sync cycle

	data = 0x182;
	Edit_Dict(&TestMaster_Data,0x14000120, 0x01, &data);		//RPDO	node 1
	data = 0x183;
	Edit_Dict(&TestMaster_Data,0x14010120, 0x01, &data);		//RPDO	node 2
	data = 0x184;
	Edit_Dict(&TestMaster_Data,0x14020120, 0x01, &data);		//RPDO	node 3
	data = 0x185;
	Edit_Dict(&TestMaster_Data,0x14030120, 0x01, &data);		//RPDO	node 4
	data = 0x186;
	Edit_Dict(&TestMaster_Data,0x14040120, 0x01, &data);		//RPDO	node 5
	data = 0x187;
	Edit_Dict(&TestMaster_Data,0x14050120, 0x01, &data);		//RPDO	node 6
	
	data = 0x202;
	Edit_Dict(&TestMaster_Data,0x18000120, 0x01, &data);		//TPDO	node 1
	data = 0x203;
	Edit_Dict(&TestMaster_Data,0x18010120, 0x01, &data);		//TPDO	node 2
	data = 0x204;
	Edit_Dict(&TestMaster_Data,0x18020120, 0x01, &data);		//TPDO	node 3
	data = 0x205;
	Edit_Dict(&TestMaster_Data,0x18030120, 0x01, &data);		//TPDO	node 4
	data = 0x206;
	Edit_Dict(&TestMaster_Data,0x18040120, 0x01, &data);		//TPDO	node 5
	data = 0x207;
	Edit_Dict(&TestMaster_Data,0x18050120, 0x01, &data);		//TPDO	node 6
}
