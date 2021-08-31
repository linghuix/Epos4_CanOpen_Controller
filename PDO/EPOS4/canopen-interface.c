/*
 * canopen-interface.c
 *
 *  Created on: Mar 5, 2020
 *      Author: test
 */

#include "canopen_interface.h"

/*
 * author lhx
 *
 * @brief : statement in state.c.  callback when Already into preOperational state.
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void _preOperational(CO_Data* d){

	MSG("complete preOperational\r\n");

}

void _initialisation(CO_Data* d)
{
	(void)d;
	MSG("complete initialization\r\n");

}
void _operational(CO_Data* d)
{
	(void)d;
	MSG("complete operational\r\n");
}
void _stopped(CO_Data* d)
{
	(void)d;
	MSG("complete stop\r\n");
}


/*
 * author lhx
 *
 * @brief : after sendPDOevent, so at beginning ,the first PDO's value is configured in the dict.
 * Window > Preferences > C/C++ > Editor > Templates.
 */


void assive (CO_Data* d);
void Test_curve(CO_Data* d);
void sin_cos_test (CO_Data* d);

void _post_TPDO(CO_Data* d)
{
	//assive(d);
	//sin_cos_test(d);
}



#include "gait.h"


#define ARRAY_K   knee
#define ARRAY_H   hip

int PERIOD = 0;							// 设定运行次数
int period = 0;						// 实际运行次数
uint16_t endP = 0;
uint8_t Index = 0;
int x=0, temp_x;						//extern int x=0;语法错误

extern INTEGER32 Pos_Actual_Val_node1, Pos_Actual_Val_node2, Pos_Actual_Val_node3, Pos_Actual_Val_node4, Pos_Actual_Val_node5, Pos_Actual_Val_node6;
extern INTEGER32 Actual_Velocity_VALUE_node1, Actual_Velocity_VALUE_node2, Actual_Velocity_VALUE_node3, Actual_Velocity_VALUE_node4, Actual_Velocity_VALUE_node5, Actual_Velocity_VALUE_node6;
extern INTEGER16 Current_Actual_Val_node2;
extern INTEGER32 Pos_SET_VALUE_node3;

Uint32 Position[6];
float Position_float[6];
int Position_int[6];

int epos_state = 50;

#include "func_CanOpen.h"
#define QC_TO_Degree_EC90 1//4551.0 //100*4096*4/360.0//1820.44	 100 REDUCER   4096 ENCODER  
#define QC_TO_Degree_RE40 1//2222.22

void assive (CO_Data* d)
{
	if(PERIOD != 0){
		
		endP = sizeof(ARRAY_H)/sizeof(*ARRAY_H);				// period size
		
		//Position_float[0] = (ARRAY_H[x]*QC_TO_Degree_EC90);
		Position_int[0] = (ARRAY_H[x]*QC_TO_Degree_EC90);		// postive means go forward
		Position[0] = (Uint32)Position_int[0];					//for node2
		
		//Position_float[1] = (ARRAY_K[x]*QC_TO_Degree_EC90);
		Position_int[1] = (ARRAY_K[x]*-QC_TO_Degree_EC90);
		Position[1] = (Uint32)Position_int[1];					//for node3 counterclkwise
		
		temp_x = x + endP/2;									//start form half period. another side
		if(temp_x >= endP){
			temp_x = temp_x - endP;
		}
		
		//Position_float[2] = (ARRAY_H[temp_x]*QC_TO_Degree_EC90);
		Position_int[2] = (ARRAY_H[temp_x]*-QC_TO_Degree_EC90);
		Position[2] = (Uint32)Position_int[2];					//for node4
		
		//Position_float[3] = (ARRAY_K[temp_x]*QC_TO_Degree_RE40);
		Position_int[3] = (ARRAY_K[temp_x]*QC_TO_Degree_EC90);
		Position[3] = (Uint32)Position_int[3];					//for node5  clkwise

		x=x+3;
		if( x >= endP){
			x = 0;
			period++;
			MYMSG("#%d\t%d\r\n",period,PERIOD);
		}
		
		Edit_Dict(d , 0x20610020, 0x00, &Position[0]);			//右hip
		Edit_Dict(d , 0x20660020, 0x00, &Position[1]);			//右knee
		//Edit_Dict(d , 0x20620020, 0x00, &Position[0]);//Pos_SET_VALUE node_1
		Edit_Dict(d , 0x20640020, 0x00, &Position[2]);			//左hip
		Edit_Dict(d , 0x20630020, 0x00, &Position[3]);			//左knee

		//Edit_Dict(d , 0x20650020, 0x00, &Position[3]);
		
		ROW_MSG("pos -%d ",x);
		for(uint8_t i = 0;i<4;i++){
			ROW_MSG("%d\t", Position_int[i]);
		}
		printf("\r\n");
		//ROW_MSG("%d\t%d\t%d\t%d\r\n",Pos_Actual_Val,Pos_Actual_Val_node3,Pos_Actual_Val_node4, Pos_Actual_Val_node5);
	}
}



Uint32 pos;
int subI = 0;
void sin_cos_test (CO_Data* d)
{
	pos = (int)(20000*sin(5.0f*3.14f*0.005f*subI));
	
	Uint32 pos1 = pos+1;
	Uint32 pos2 = pos+2;
	Uint32 pos3 = pos+3;
	Uint32 pos4 = pos+4;
	Uint32 pos5 = pos+5;
	
	Edit_Dict(d , 0x20610020, 0x00, &pos);
	Edit_Dict(d , 0x20620020, 0x00, &pos1);
	Edit_Dict(d , 0x20630020, 0x00, &pos2);
	Edit_Dict(d , 0x20640020, 0x00, &pos3);
	Edit_Dict(d , 0x20650020, 0x00, &pos4);
	Edit_Dict(d , 0x20660020, 0x00, &pos5);
	
	subI++;
	ROW_MSG("%d\t%d\t%d\t%d\t%d\t%d\t%d\t\t",Pos_Actual_Val_node1,Pos_Actual_Val_node2,Pos_Actual_Val_node3, Pos_Actual_Val_node4,Pos_Actual_Val_node5,Pos_Actual_Val_node6, pos);
	ROW_MSG("%d\t%d\t%d\t%d\t%d\t%d\r\n",Actual_Velocity_VALUE_node1,Actual_Velocity_VALUE_node2,Actual_Velocity_VALUE_node3, Actual_Velocity_VALUE_node4,Actual_Velocity_VALUE_node5,Actual_Velocity_VALUE_node6);
	
}

void Test_curve (CO_Data* d)
{
	endP = sizeof(test_angle)/sizeof(*test_angle);
	pos = test_angle[x++]*20;
	if( x == endP){
		x = 0;
	}
	
	Uint32 pos1 = pos+1;
	Uint32 pos2 = pos+2;
	Uint32 pos3 = pos+3;
	Uint32 pos4 = pos+4;
	Uint32 pos5 = pos+5;
	
	Edit_Dict(d , 0x20610020, 0x00, &pos) ;
	Edit_Dict(d , 0x20620020, 0x00, &pos1);
	Edit_Dict(d , 0x20630020, 0x00, &pos2);
	Edit_Dict(d , 0x20640020, 0x00, &pos3);
	Edit_Dict(d , 0x20650020, 0x00, &pos4);
	Edit_Dict(d , 0x20660020, 0x00, &pos5);
	
	ROW_MSG("%d\t%d\t%d\t%d\t%d\r\n",Pos_Actual_Val_node5, Pos_Actual_Val_node6, Actual_Velocity_VALUE_node5, Actual_Velocity_VALUE_node6, pos);
}


/*
 * author lhx
 *
 * @brief : in SYNC but before _sendPDOevent.
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void _post_sync(CO_Data* d)
{
	(void)d;
	SYNC_MSG("-post_sync-\r\n");
	//waiting for sensor information
	assive(d);
	//sin_cos_test(d);
	//Test_curve(d);

	#ifdef REMOTE_APP
    if(Stop == 1){					// 实现快速的停止，其实在EPos task中也有对应的操作，那个不是实时停止的
         EPOSMaster_PDOStop();
    }
    else if(PeriodRun == 1){
        if(period >= PERIOD){
			PeriodRun = 0;
			period = 0;				// reset period to 0 for next start
            EPOSMaster_PDOStop();
            Stop = 1;Reset = 1;
        }
    }
	#endif
}



/*
 * author lhx
 *
 * @brief : life guard
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void _heartbeatError(CO_Data* d, UNS8 heartbeatID)
{
	(void)d;
	(void)heartbeatID;
	MSG_WAR(0x44, "heart beat error", heartbeatID);
	EposMaster_Stop();
}
void _post_SlaveBootup(CO_Data* d, UNS8 SlaveID){(void)d;(void)SlaveID;}
void _post_SlaveStateChange(CO_Data* d, UNS8 nodeId, e_nodeState newNodeState){(void)d;(void)nodeId;(void)newNodeState;}
void _nodeguardError(CO_Data* d, UNS8 id){(void)d;(void)id;}


/*
 * author lhx
 *
 * @brief : receive a emcy
 * Window > Preferences > C/C++ > Editor > Templates.
 */
void _post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg, const UNS8 errSpec[5])
{
	(void)d;
	(void)nodeID;
	(void)errCode;
	(void)errReg;
	(void)errSpec;
}
