#include "epos.h"
#include "delay.h"
#include "sdo.h"
#include "can.h" 

extern CanRxMsg RxMessage;


/*
*输入：EPOS结构体，串口对象fd1.
*功能：启动EPOS，并且设置最大的跟踪误差，最大的速度、加速度、负加速度，以及快速停止的负加速度设定
*/
void Epos_ParamInit(Epos* epos)
{
    //SDO_Read(epos,OD_STATUS_WORD,0x00);                                 //Fault Status=0x0108  红灯闪烁

    SDO_Write(epos, OD_CTRL_WORD, 0x00, Fault_Reset);      //Fault_Reset command 控制字设置为0x80 第7位置1，参考固件手册 Fault reset figure3-3 事件15 驱动初始化完成

     //SDO_Read(epos,OD_STATUS_WORD,0x00);                               //Switch On    Status=0x0540/0140   绿灯闪烁

    SDO_Write(epos, OD_Following_ERR_window, 0x00, MAX_F_ERR);     //最大误差设置

    SDO_Write(epos, OD_MAX_P_VELOCITY, 0x00, MAX_P_V);   //最大速度

    SDO_Write(epos, OD_P_ACCELERATION, 0x00, epos->acc); //加速度

    SDO_Write(epos, OD_P_DECELERATION, 0x00, epos->dec); //负加速度

    SDO_Write(epos, OD_QS_DECELERATION, 0x00, QDEC);     //快速停止负加速度

    //SDO_Write(epos,OD_CAN_BITRATE,0x00,0x00);              //set value = 0. set CAN bitrate 1M/s. 

    printf("Epos_ParamInit \r\n");
}


/*
*输入：EPOS结构体，CAN设备(节点)序号 CAN_ID，节点的ID：NODE_ID1，串口对象 fd1
*功能：启动EPOS，并且设置最大的跟踪误差，最大的速度、加速度、负加速度，以及快速停止的负加速度设定
*/
void Epos_Init(Epos* epos1, Uint8 CAN_ID, Uint8 NODE_ID)
{
    // EPOS结构体初始化
    epos1->b_init = 0;
    epos1->CAN_Handles = NULL;
    epos1->node_ID = NODE_ID;               //节点ID设置
    
    epos1->b_init = 1;                      //EPOS 初始化标志
    epos1->cur_mode = MODE_NONE;            //控制模式
    epos1->opt = 0x001f;
    epos1->acc = MAX_ACC;                   //最大加速度
    epos1->dec = MAX_DEC;                   //最大负加速度
    epos1->b_need_shutdown = 0;             //是否需要关闭控制器

    // 通过canopen设定EPOS控制器参数
    Epos_ParamInit(epos1);

    printf("Epos_SInit\r\n");
}


/************打印CAN帧********************/
void Print(CanRxMsg RxMessage){
    
    int i;
    printf("0x%x\t",RxMessage.StdId);

    printf("0x");
    for(i=0;i<8;i++) printf("%02X ",RxMessage.Data[i]);
    printf("\r\n");
}




/***未通过实验**/
void PDO_Config(Epos* A)
{
    
    /** A 节点控制器是需要发送PDO给 B 的 ***/
    /*  Transmit PDO 1 Parameter.启动PDO + 不允许RTR + 配置对应的CAN-ID 180+ID 
    SDO_Write(A,0x1400,0x01,((Uint32)0x0<<31)+((Uint32)0x0<<30) + 0x201); 
    
    //Transmit PDO 1 Mapping 映射一个对象 + 32位角度设置寄存器
    SDO_Write(A,0x1A00,0x00,0x01); 
    SDO_Write(A,0x1A00,0x01,((Uint32)PM_SET_VALUE<<16)+((Uint32)0x00<<8)+0x20); */
    
    
    /** B 设置对应的 RXPDO **/
    //Receive PDO 1 Parameter
    SDO_Write(A,0x1400,0x01,(((Uint32)0x0<<31)+((Uint32)0x0<<30)+0x201)); //ID与接收的TxPDO对应，实现两者之间的传输
    
    //Receive PDO 1 Mapping。设置条件：首先PDO必须使能，NMT必须位于Pre-Operation
    SDO_Write(A,0x1400,0x02,1); 
    
    SDO_Write(A,0x1600,0x00,0); 
    SDO_Write(A,0x1600,0x01,(((Uint32)Target_pos<<16)+((Uint32)0x00<<8)+0x20)); //映射的对象可以与TxPDO不同！ 
    SDO_Write(A,0x1600,0x00,1);
}


/***未通过实验**/
void PDO_Write(Uint16 ID, Uint32 angle)
{
    Uint32 low_4byte = angle;
    Uint32 high_4byte = 0;
    CAN_SetMsg(ID, low_4byte, high_4byte);
}


/***未通过实验**/
void NMT_Pre(Epos* epos, Uint32 ID)
{
    
Epos_Delay(2);
epos->buf.msg_id.bit.STDMSGID = 0x00;                        //CANOPEN 的 客户端发送到服务器命令 SOD报文ID，参考CanOpen 标准

//CAN中的8个数据字节，中的低四个字节定义  CS(01H)+Node-ID
epos->buf.low_4byte = 0x80 + (ID << 8);                                      //buf中高位在前
epos->buf.high_4byte = 0;                                    //SDO 携带的实际数据，CS定义了只能携带四个字节
epos->buf.DataLen  = 8;

CAN_SetMsg(epos->buf.msg_id.bit.STDMSGID, epos->buf.low_4byte, epos->buf.high_4byte);

Epos_Delay(2);  
    
/*CAN_SetMsg(0x701, 0, 0);
Epos_Delay(2);  
CAN_SetMsg(0x702, 0, 0);
Epos_Delay(2);  */
    
}


/***未通过实验**/
void NMT_Start(Epos* epos, Uint32 ID)
{
    
Epos_Delay(2);
epos->buf.msg_id.bit.STDMSGID = 0x00;                        //CANOPEN 的 客户端发送到服务器命令 SOD报文ID，参考CanOpen 标准

//CAN中的8个数据字节，中的低四个字节定义  CS(01H)+Node-ID
epos->buf.low_4byte = 0x01 + (ID << 8);                                      //buf中高位在前
epos->buf.high_4byte = 0;                                    //SDO 携带的实际数据，CS定义了只能携带四个字节
epos->buf.DataLen  = 8;

CAN_SetMsg(epos->buf.msg_id.bit.STDMSGID, epos->buf.low_4byte, epos->buf.high_4byte);

Epos_Delay(2);  
    
/*CAN_SetMsg(0x701, 0, 0);
Epos_Delay(2);  
CAN_SetMsg(0x702, 0, 0);
Epos_Delay(2);  */
    
}


/****** 设置EPOS工作模式 *****/
void Epos_setMode(Epos* epos, Uint16 mode){

    epos->cur_mode = mode;
    
    SDO_Write(epos,OP_MODE,0x00,mode); 
                
    switch(mode){
        
        case(CSP):
                //SDO_Write(epos,OD_MOTOR_DATA,0x01,15000);//nominal电流0-15000                   
                //SDO_Write(epos, OD_MOTOR_DATA, 0x05,0);  //扭矩常数 0-10 000 000  
				
                //SDO_Write(epos, Current_control_parameter , 0x01, 1171880);//P                   
                //SDO_Write(epos, Current_control_parameter, 0x02, 3906250);  //I  
				
                //SDO_Write(epos,Pos_control_para,0x01,1500000);//P                  
                //SDO_Write(epos, Pos_control_para, 0x02,7800000);  //I              
                //SDO_Write(epos, Pos_control_para , 0x03, 16000);//D      
                //SDO_Write(epos, Pos_control_para, 0x04, 0);  //Position controller FF velocity gain		
                //SDO_Write(epos, Pos_control_para, 0x05, 0);  //Position controller FF acceleration gain
				
				
                SDO_Write(epos, OD_P_DECELERATION, 0x00,3000); 
                SDO_Write(epos, OD_QS_DECELERATION , 0x00, 5000);
				
					      SDO_Write(epos, OD_Following_ERR_window , 0x00, 0);
				
                SDO_Write(epos, Soft_P_Limit, 0x01, 0x80000000);                //-2147483648
                SDO_Write(epos, Soft_P_Limit, 0x02, 0x7FFFFFFF);                //2147483647
				
                break;
        
        case(Velocity_Mode):		//CONFIGURATION PARAMETERS
                //SDO_Write(epos,OP_MODE,0x00,Velocity_Mode); 
                SDO_Write(epos,OD_Max_Acceleration,0x00,1000);                                                  // set Max Acceleration
                SDO_Write(epos, OD_MAX_P_VELOCITY, 0x00, MAX_P_V);                                            // Maximal Profile Velocity 
                break;
                
        case(Current_Mode):
                SDO_Write(epos,OD_MOTOR_DATA,0x01,2000);         // set Continuous Current Limit 连续输出电流最大值 mA
                SDO_Write(epos, OD_MOTOR_DATA, 0x02, 4000);     //输出最大电流，推荐为两倍连续最大电流
                SDO_Write(epos, OD_MOTOR_DATA, 0x03, 1);        //Changes only in “Disable” state.Number of magnetic pole pairs 参考电机手册
                SDO_Write(epos, OD_MOTOR_DATA, 0x04, 25000);     //限制电机最大速度 rpm
                SDO_Write(epos, OD_MOTOR_DATA, 0x05, 1);         // thermal time constant
                break;
                
        case(Profile_Position_Mode):	//CONFIGURATION PARAMETERS
                SDO_Write(epos, Soft_P_Limit, 0x01, 0x80000000);                //-2147483648
                SDO_Write(epos, Soft_P_Limit, 0x02, 0x7FFFFFFF);                //2147483647
                SDO_Write(epos, OD_MAX_P_VELOCITY, 0x00, MAX_P_V);              // Maximal Profile Velocity 
								SDO_Write(epos, OD_MAX_MOTOR_SPEED, 0x00, 5000);								//参考电机手册
								SDO_Write(epos, Max_gear_input_speed, 0x03,1000);
                SDO_Write(epos, OD_QS_DECELERATION, 0x00, QDEC);                //快速停止负加速度
                SDO_Write(epos,OD_Max_Acceleration,0x00,10000);   
                break;
                
        case(Profile_Velocity_Mode):		//CONFIGURATION PARAMETERS
                SDO_Write(epos, Soft_P_Limit, 0x01, 0x80000000);                //-2147483648
                SDO_Write(epos, Soft_P_Limit, 0x02, 0x7FFFFFFF);                //2147483647
                SDO_Write(epos, OD_MAX_P_VELOCITY, 0x00,3000);                 //最大速度 Maximal Profile Velocity 
	              SDO_Write(epos, OD_QS_DECELERATION, 0x00, 50000);              //快速停止负加速度			
                SDO_Write(epos, OD_MAX_MOTOR_SPEED, 0x00, 5000);              // Maximal Profile Velocity 
				        SDO_Write(epos, Max_gear_input_speed, 0x03,1000);
                SDO_Write(epos,OD_Max_Acceleration,0x00,10000);   
				
								break;
                
        case(Homing_Mode):      //可以将该功能配置给某个数字口
                SDO_Write(epos, OD_Motion_Profile_Type, 0x00,1);                 //sin2  ramp 
                break;
                
        case(Interpolated_Position_Mode):
                SDO_Write(epos, OD_Interpolation_Sub_Mode, 0x00,(Uint32)(-1));//always -1 cubic spline interpolation (PVT)
                SDO_Write(epos, Interpolation_Time_Period, 0x01,1);//always 1
                SDO_Write(epos, Interpolation_Time_Period, 0x02,(Uint32)(-3));//always -3  插值周期 10^-3s
                SDO_Write(epos, Soft_P_Limit, 0x01, 0x80000000);                //-2147483648
                SDO_Write(epos, Soft_P_Limit, 0x02, 0x7FFFFFFF);                //2147483647
                //SDO_Write(epos,OD_Position_Window, 0x00,4294967295);             //关闭 position window 
                SDO_Write(epos, Pos_Window_Time, 0x00, 0);
                break;
                
        default: 
                printf("\r\nerror mode\r\n");
        
    }
}

    
void Epos_OperEn(Epos* epos){
    SDO_Write(epos,OD_CTRL_WORD,0x00,0x06);                    // Shut down  驱动函数失能
    Epos_Delay(500);

        //SDO_Read(epos,OD_STATUS_WORD,0x00);                                                // Ready to Switch On    Status=0x0121   绿灯闪烁
    
    SDO_Write(epos,OD_CTRL_WORD,0x00,0x0F);                    // Switch on AND Enable Operation 驱动参数设定
    Epos_Delay(500);
    
        //SDO_Read(epos,OD_STATUS_WORD,0x00);                                                // Operation Enable      Status=0x0137   绿灯常亮
}


void Epos_Delay(Uint32 time){
    Delay_ms(time);
}

