
#include "func_epos.h"

/*
 * 函数名：接收传感器数
 * 描述 
 * 调用  
 */
uint8_t NODE_ID[] = {3,4,5,6,7,7};                          																//EPOS ID
Epos Controller1, Controller2, Controller3, Controller4, Controller5, Controller6;        //控制器对
Epos *Controller[] = {&Controller1, &Controller2, &Controller3, &Controller4, &Controller5, &Controller6};
uint8_t NumControllers = 4;


#include "canopen_interface.h"
#include "func_CanOpen.h"
extern uint8_t NumControllers;
int home[] = {0, 0, 0, 0, 0, 0};
extern int PERIOD ;
extern int x;
#define QC_TO_Degree_EC90 4554.0//100*4096*4/360.0//1820.44	 100 REDUCER   4096 ENCODER  
void EposMaster_Start(void)
{
	//uint32_t data[6];
	Init_MyDict();
	Uint32 data[6];
	
	setState(&TestMaster_Data, Initialisation);
	
	if (!(*(TestMaster_Data.iam_a_slave)))		//master
	{
		//EPOS_NMT_Reset();
		//OSTimeDlyHMSM(0, 0, 1, 0);
		
        Epos_init();
        Epos_ModeSet(Profile_Position_Mode);
        EPOS_Enable();
		
		printf("waiting\r\n");
		while(PERIOD == 0){
			OSTimeDlyHMSM(0, 0,0,5);
		}
		
		int pos[4]={-33828,127960,46213,35865};
		int intpos[4];
//		firstPos(pos);
		for(int i=0;i<NumControllers;i++){
			//SDO_Write(Controller[i], Max_Profile_Velocity, 0x00, 100);				//reset speed set slower
			SDO_Write(Controller[i], Max_motor_speed, 0x00, 100);					//参考电机手册
			
			intpos[i] = pos[i];
			printf("pos-%d\r\n",intpos[i]);
			Epos_PosSet(Controller[i], intpos[i]);
		}
	
		OSTimeDlyHMSM(0, 0,10,0);
		
		for(int i=0;i<NumControllers;i++){
			SDO_Write(Controller[i], Max_motor_speed, 0x00, 100);				//reset speed set slower
			//SDO_Write(Controller[i], Max_Profile_Velocity, 0x00, 2000);
		}
//		
		x=0;
		Epos_ModeSet(Cyclic_Synchronous_Position_Mode);
		
//        EPOS_Enable();
		
		EPOS_PDOEnter();
	}
	
	/* 验证是否进入位于home */
	for(int i=0;i<NumControllers;i++){
		data[i] = SDO_Read(Controller[i], Position_actual_value, 0X00);
		MSG("pos - %x\r\n",data[i]);
		//SDO_Write(Controller[i], Max_Profile_Velocity, 0x00, 6000);
		//SDO_Write(Controller[i], Max_Profile_Velocity, 0x00, MAX_P_V);				//reset to previous speed 
	}
	
	/* 验证是否进入 Operational 模式 */
	for(int i=0;i<NumControllers;i++){
		data[i] = SDO_Read(Controller[i], Statusword, 0X00);
		MSG("state - %x\r\n",data[i]);
	}
	
	//if(((data[0]>>9)&0x01) & ((data[1]>>9)&0x01) & ((data[2]>>9)&0x01) & ((data[3]>>9)&0x01)){
		HAL_TIM_Base_Start_IT(CANOPEN_TIMx_handle);
		MSG("already start MNT\r\n");
		printf("-----------------------------------------------\r\n");
		printf("-----------------PDO_ENABLE -------------------\r\n");
		printf("-----------------------------------------------\r\n");
		//setState(&TestMaster_Data, Pre_operational); //心跳,同步周期协议配置
		setState(&TestMaster_Data, Operational);
	//}
    
}


/** Paremeter configure and struct init */
/**
 @brief Controller结构体中的参数配置，并写入EPOS4中
 */
void Epos_init(void)
{
    uint8_t i;		                                            //index
	for(i=0;i<NumControllers;i++){
		Node_StructInit(Controller[i], NOT_USED, NODE_ID[i]);	//初始化最大加速度，速度，跟踪误差，波特1M/s
    }

	printf("-----------------------------------------------\r\n");
	printf("-----------------Epos_Init---------------------\r\n");
	printf("-----------------------------------------------\r\n");

	OSTimeDlyHMSM(0, 0,0,500);

	for(i=0;i < NumControllers;i++){
		Node_ParamConfig(Controller[i]);                        //通过canopen设定EPOS控制器参
	}	
    
    printf("-----------------------------------------------\r\n");
	printf("-------------Initial_EPOS_Done!----------------\r\n");
	printf("-----------------------------------------------\r\n");
	//OSTimeDlyHMSM(0, 0,0,500);
}


/** chose mode for epos4 */
void Epos_ModeSet(uint8_t mode)
{
	//******** 控制模式设置 *******
	for(int i=0;i<NumControllers;i++){
		Node_setMode(Controller[i], mode);
	}
	printf("-----------------------------------------------\r\n");
	printf("-----------------Mode_set----------------------\r\n");
	printf("-----------------------------------------------\r\n");
	//OSTimeDlyHMSM(0, 0,0,500);
}


/** Epos enter into operation. So we can drive the motor.  */
void EPOS_Enable(void)
{
 	//******** 使能EPOS *******
	for(int i=0;i<NumControllers;i++){
		Node_OperEn(Controller[i]);                                              //Switch On Disable to Operation Enable
	}
	printf("-----------------------------------------------\r\n");
	printf("-----------------Enable_EPOS-------------------\r\n");
	printf("-----------------------------------------------\r\n");

	
}

/* Make Epos's NMT state Pre-Operation */
void EPOS_NMT_Reset(void)
{
	for(int i=0;i<NumControllers;i++){
		masterNMT(&TestMaster_Data, Controller[i], NMT_Reset_Node);	//to Pre-Operation
	}
}

/* Make Epos's NMT state operation. So we can proceed PDO contorl */
void EPOS_PDOEnter(void)
{
	printf("-----------------------------------------------\r\n");
	printf("---------NMT -enter into operation-------------\r\n");
	printf("-----------------------------------------------\r\n");
	for(int i=0;i<NumControllers;i++){
		masterNMT(&TestMaster_Data, Controller[i], NMT_Start_Node);	// NMT state is set to operation
	}
}


void EPOSMaster_PDOStart(void)
{
    HAL_TIM_Base_Start_IT(CANOPEN_TIMx_handle);
	printf("-----------------------------------------------\r\n");
	printf("-----------------PDO_Start -------------------\r\n");
	printf("-----------------------------------------------\r\n");
	//setState(&TestMaster_Data, Pre_operational); //心跳,同步周期协议配置
	setState(&TestMaster_Data, Operational);
}


void EPOSMaster_PDOStop(void)
{
    HAL_TIM_Base_Stop_IT(CANOPEN_TIMx_handle);
	setState(&TestMaster_Data, Initialisation);
	printf("-----------------------------------------------\r\n");
	printf("-----------------PDO_Stop -------------------\r\n");
	printf("-----------------------------------------------\r\n");
} 

/** use SDO protocol to set speed */
void Epos_SDOSpeedSet(Uint32 speed){
	
	 SDO_Write(Controller[1],Target_Velocity,0x00,speed);
	 SDO_Write(Controller[1],Position_actual_value ,0x00,0x0F);	
}


/**************Position Mode*********************************/
void EPOS_SetAngle(Epos* epos, Uint32 angle){
    
    #if defined SDO
    SDO_Write(epos, Target_pos, 0x00, angle);
    #endif 
}


/** Position Set */
void Epos_PosSet(Epos* epos, Uint32 pos)
{

//	 SDO_Write(epos,Controlword ,0x00,0x0F);	
	 EPOS_SetAngle(epos,pos);
	 SDO_Write(epos,Controlword ,0x00,0x3F);	
}


/*
 * 函数名：实时控制任务
 * 描述  
 * 调用  
 */
/**实现速度摇摆控制 */
void SDO_SpeedTest(void){
	
	Uint32 speed = 50;
	
	Epos_SDOSpeedSet(speed);
	Epos_Delay(1000); 
	
	Epos_SDOSpeedSet(-speed);
	Epos_Delay(1000); 
	
	Epos_SDOSpeedSet(speed*5);
	Epos_Delay(1000); 

	Epos_SDOSpeedSet(-speed*5);
	Epos_Delay(1000); 
}



void Epos_PDOConfig(void)
{
    //NMT_Pre(Controller[1], ALL);                        
//    SDO_Read(Controller[1],Statusword,0x00);

	for(int i=0;i<NumControllers;i++){
		Node_PDOConfig(Controller[i]);
	}
	
//    SDO_Read(Controller[1],0x1400,0x01);
//    SDO_Read(Controller[1],0x1600,0x00);
//    SDO_Read(Controller[1],0x1600,0x01);
    //NMT_Start(Controller[1], ALL);
}


