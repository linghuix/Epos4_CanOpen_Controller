#include "sdo_control.h"

/***** 
write parameter to Object Dictionary through CAN use SDO messages
******/
uint8_t waiting_sdo = 0;
uint8_t last_success = 1;	//上一次是否发送成功，这关系到发送line是否被初始化

/**
 * 在SDOTimeoutAlarm 中回调 | sdo发送成功后调用
 * 每次都调用reset，太浪费了
 */
void _sdocallback(CO_Data* d, UNS8 nodeId){
	//MSG_WAR(0x0000, "sdocallback in nodeid: ", nodeId);
	//resetSDO(d);
}


extern OS_EVENT * CRCV_WAIT_Semp;
/* writing SDO to Node for 3 times at most. it doesn't need CANOpen TIMER to send time out signal, so TIMER doesn't need to START IT */
uint8_t SDO_Write(Epos* epos,Uint32 Index_Type,Uint8 SubIndex,Uint32 param)
{
	int i = 3;
	UNS32 abortCode;
	UNS8 err;
	UNS8 SDO_state;
	UNS16 Index = Index_Type>>16;
	UNS32 Size = (Index_Type&(UNS32)0xFF)>>3;
	SubIndex = (Index_Type>>8)&0xFF;
	UNS8 Type;
	if(Size == 4){Type = uint32;}
	if(Size == 2){Type = uint16;}
	if(Size == 1){Type = uint8;}
	//_writeNetworkDict(&TestMaster_Data,epos->node_ID ,Index, SubIndex, Size, Type, &param, _sdocallback, 1, 0);	//��can�����еĽڵ㷢��
	do{
		waiting_sdo = 1;
		if(last_success == 0){	//
			resetSDO(&TestMaster_Data);
		}
		
		if(i != 3){				//上一次发送失败了
			resetSDO(&TestMaster_Data);
		}
		
		_writeNetworkDict(&TestMaster_Data,epos->node_ID ,Index, SubIndex, Size, Type, &param, _sdocallback, 1, 0);
		OSSemPend(CRCV_WAIT_Semp, 100, &err); 																		// 100ms waiting for CAN receive a frame.
	}while( --i && (SDO_state = getWriteResultNetworkDict(&TestMaster_Data, epos->node_ID , &abortCode)) == SDO_DOWNLOAD_IN_PROGRESS );

	waiting_sdo = 0;

	if (SDO_state == SDO_FINISHED){
		last_success = 1;
		MMSG(" node %d write SDO OK! 0x%X %X\r\n",epos->node_ID, Index_Type, param);
	}
	else{
		last_success = 0;
		ERROR(1," node %d write SDO error!  0x%X  0x%X state = %u",epos->node_ID, Index_Type, param, SDO_state);
		//resetSDO(&TestMaster_Data);
		//SDO_Write(epos, Index_Type,SubIndex,param);
	}
	return 0;
}


/**** CAN SDO***/
Uint32 SDO_Read(Epos* epos,Uint32 Index_Type,Uint8 SubIndex)
{	
	UNS8 i=3,err;
	UNS32 data=0;
	UNS32 abortCode;
	UNS16 Index = Index_Type>>16;
	UNS32 Size = (Index_Type&(UNS32)0xFF)>>3;
	UNS8 Type;
	if(Size == 4){Type = uint32;}
	if(Size == 2){Type = uint16;}
	if(Size == 1){Type = uint8;}
	
	do{
		waiting_sdo = 1;
		resetSDO(&TestMaster_Data);
		readNetworkDict(&TestMaster_Data,epos->node_ID ,Index, SubIndex, Type, 0);
		OSSemPend(CRCV_WAIT_Semp, 200, &err); // 500ms. ����Ҫ���ӳ�ʱ��,����Ļ�����ϵͳ��������������,Ҳ���޷�����CAN֡
	}while( i-- && (getReadResultNetworkDict(&TestMaster_Data, epos->node_ID, &data, &Size, &abortCode) == SDO_UPLOAD_IN_PROGRESS));

	waiting_sdo = 0;
	return data;
}
