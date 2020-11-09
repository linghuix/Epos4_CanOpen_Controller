
#include "conf_epos.h"


void EposMaster_Init(void)
{
	unsigned char nodeID = 0x01;
	setNodeId(&TestMaster_Data, nodeID);
}



void EposMaster_Stop(void)
{
	setState(&TestMaster_Data, Stopped); //心跳,同步周期协议配置
}


/*
 *
 *
 *
 *
 */

/*
 * author lhx
 *
 * @brief : 配置网络EPOS 节点，并且设置最大的跟踪误差，最大的速度、加速度、负加速度，以及快速停止的负加速度设定
 * Window > Preferences > C/C++ > Editor > Templates.
 */
void Node_ParamConfig(Epos* epos)
{
	MSG("Epos_base_ParamInit node %d : \r\n", epos->node_ID);
	
    SDO_Write(epos, Controlword, 0x00, Disable_voltage);
    SDO_Write(epos, Controlword, 0x00, Fault_Reset);      //Fault_Reset command 控制字设置为0x80 第7位置1，参考固件手册

    SDO_Write(epos, Following_error_window, 0x00, MAX_F_ERR);   //最大误差设置
	//SDO_Write(epos, Following_error_window, 0x00, 20000);   //最大误差设置

	SDO_Write(epos, Max_motor_speed, 0x00, epos->max_velocity);//MAX_P_V);   //最大允许速度
	SDO_Write(epos, Max_Profile_Velocity, 0x00, epos->max_velocity);//MAX_P_V);  //最大速度
	//SDO_Write(epos, Max_motor_speed, 0x00, 3000);//MAX_P_V);   //最大允许速度
	//SDO_Write(epos, Max_Profile_Velocity, 0x00, 3000);//MAX_P_V);  //最大速度

    SDO_Write(epos, Max_Acceleration, 0x00, MAX_ACC);	    //max acc set
    SDO_Write(epos, Profile_Acceleration, 0x00, MAX_ACC); 		//加速度  无效参数
    SDO_Write(epos, Profile_Deceleration, 0x00, MAX_ACC); 		//负加速度
    SDO_Write(epos, Q_deceleration, 0x00, QDEC);     		//快速停止负加速度

    SDO_Write(epos,OD_CAN_BITRATE,0x00,0x00);              	    //set value = 0. set CAN bitrate 1M/s.
}


/*
 * author lhx
 *
 * @brief : 设定EPOS 结构体，并且设置最大的跟踪误差，最大的速度、加速度、负加速度，以及快速停止的负加速度设定
 * Window > Preferences > C/C++ > Editor > Templates.
 */
void Node_StructInit(Epos* epos1, Uint8 CAN_ID, Uint8 NODE_ID)
{
    // EPOS结构体初始化
    epos1->node_ID = NODE_ID;               //节点ID设置
    epos1->cur_mode = MODE_NONE;            //控制模式

    switch(epos1->node_ID){
        case(5):                            //RE40
		    epos1->acc = 50000; 
		    epos1->dec = 50000;
            epos1->max_velocity = 6000;     
            epos1->error_window = 2000;
            break;
        default:                            //EC90
		    epos1->acc = MAX_ACC;                   //最大加速度
		    epos1->dec = MAX_DEC;                   //最大负加速度
            epos1->max_velocity = MAX_P_V;
            epos1->error_window = 20000;
            break;
    }
}



/*
 * author lhx
 *
 * @brief : 打印CAN帧
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void Print(CanRxMsg RxMessage){
    
    int i;
//    printf("0x%x\t",RxMessage.StdId);
    printf("0x%x\t",RxMessage.head.StdId);

    printf("0x");
    for(i=0;i<8;i++) printf("%02X ",RxMessage.Data[i]);
    printf("\r\n");
}



/***通过实验**/
#define RTR_NOAllow (uint32_t)0x1<<30
#define PDO_NoAllow (uint32_t)0x1<<31
void Node_PDOConfig(Epos* epos)
{
    //Receive PDO 1 Parameter
    SDO_Write(epos,0x14000120,0x01,0x200+epos->node_ID); 				//ID与接收的RxPDO对应，实现两者之间的传输
	SDO_Write(epos,0x14000208,0x01,0x1); 								//ID与接收的RxPDO对应，实现两者之间的传输
    SDO_Write(epos,0x14010120,0x01,(0x300+epos->node_ID)|PDO_NoAllow); 	//unvaild
	SDO_Write(epos,0x14010208,0x01,0x1); 								//ID与接收的RxPDO对应，实现两者之间的传输
    SDO_Write(epos,0x14020120,0x01,(0x400+epos->node_ID)|PDO_NoAllow);
	SDO_Write(epos,0x14020208,0x01,0x1); 
    SDO_Write(epos,0x14030120,0x01,(0x500+epos->node_ID)|PDO_NoAllow);
	SDO_Write(epos,0x14030208,0x01,0x1); 
	
	SDO_Write(epos,0x16000008,0x01,0x0); 								//RxPDO map
	SDO_Write(epos,0x16000120,0x01,Target_pos); 
	SDO_Write(epos,0x16000008,0x01,0x1); 					

	
	SDO_Write(epos,0x18000120,0x01,(0x180+epos->node_ID)|RTR_NOAllow); 	//TxPDO
	SDO_Write(epos,0x18000208,0x01,0x1); 
	SDO_Write(epos,0x18000310,0x01,1);
	
	SDO_Write(epos,0x18010120,0x01,(0x280+epos->node_ID)|PDO_NoAllow|RTR_NOAllow); 
	SDO_Write(epos,0x18010208,0x01,0x1); 
	SDO_Write(epos,0x18010310,0x01,1);
	
	SDO_Write(epos,0x18020120,0x01,(0x380+epos->node_ID)|PDO_NoAllow|RTR_NOAllow); 
	SDO_Write(epos,0x18020208,0x01,0x1); 
	SDO_Write(epos,0x18020310,0x01,1);
	
	SDO_Write(epos,0x18030120,0x01,(0x480+epos->node_ID)|PDO_NoAllow|RTR_NOAllow); 
	SDO_Write(epos,0x18030208,0x01,0x1); 
	SDO_Write(epos,0x18030310,0x01,1);
	
	SDO_Write(epos,0x1A000008,0x01,0); 
	SDO_Write(epos,0x1A000120,0x01,0x60640020); 
	SDO_Write(epos,0x1A000220,0x01,0x606C0020);
	SDO_Write(epos,0x1A000008,0x01,2); 
	
	SDO_Write(epos,0x1A010008,0x01,0); 
	SDO_Write(epos,0x1A020008,0x01,0); 
	SDO_Write(epos,0x1A030008,0x01,0); 
}



/*
 * author lhx
 *
 * @brief : 设置 EPOS node 工作模式, 并配置对应的工作模式
 * Window > Preferences > C/C++ > Editor > Templates.
 */
void Node_setMode(Epos* epos, Uint16 mode){

    epos->cur_mode = mode;
    
    SDO_Write(epos,OP_MODE,0x00,mode); 
                
    switch(mode){

	/** EPOS4 **/
	case(Profile_Position_Mode):		//CONFIGURATION PARAMETERS
		break;
	
	case(Profile_Velocity_Mode):
		//SDO_Write(epos,OP_MODE,0x00,Velocity_Mode);
		SDO_Write(epos,Max_Acceleration,0x00,1000);                                                // set Max Acceleration
		SDO_Write(epos, Max_Profile_Velocity, 0x00, MAX_P_V);                                            // Maximal Profile Velocity
		break;

	case(Homing_Mode):

		break;

	case(Cyclic_Synchronous_Position_Mode):	
        SDO_Write(epos, Nominal_current, 0, 7000);                      // max is 8.73 A
        SDO_Write(epos, Motor_torque_constant, 0, 136000);              // 136 mNm/A
		SDO_Write(epos, Q_deceleration, 0x00, QDEC);                    //快速停止负加速度
		SDO_Write(epos, Profile_Deceleration, 0x00, MAX_DEC);           //快速停止负加速度
		SDO_Write(epos, Following_error_window, 0x00, MAX_F_ERR);            // Maximal Profile Velocit		
        SDO_Write(epos, Soft_P_Limit_Min, 0x01, 0x80000000);            //-2147483648
		SDO_Write(epos, Soft_P_Limit_Max, 0x02, 0x7FFFFFFF);            //2147483647
        SDO_Write(epos, Max_motor_speed, 0x00, 2000);					//参考电机手册
		//SDO_Write(epos, Max_gear_input_speed, 0x03,2000);
		SDO_Write(epos, Interpolation_Time_index, 0, (uint8_t)-3);
        SDO_Write(epos, Interpolation_Time_Period_value, 0, 10);			// ms
        SDO_Write(epos,Max_Acceleration,0x00,MAX_ACC);
		break;

	case(Cyclic_Synchronous_Velocity_Mode):
		SDO_Write(epos, Soft_P_Limit_Min, 0x01, 0x80000000);                //-2147483648
		SDO_Write(epos, Soft_P_Limit_Max, 0x02, 0x7FFFFFFF);                //2147483647
		SDO_Write(epos, Max_Profile_Velocity, 0x00,3000);                 //最大速度 Maximal Profile Velocity
		SDO_Write(epos, Q_deceleration, 0x00, 50000);              //快速停止负加速度
		SDO_Write(epos, Max_motor_speed, 0x00, 5000);              // Maximal Profile Velocity
		SDO_Write(epos, Max_gear_input_speed, 0x03,1000);
		SDO_Write(epos,Max_Acceleration,0x00,10000);
		break;

	case(Cyclic_Synchronous_Torque_Mode):
        SDO_Write(epos, Nominal_current, 0, 7000);                      // max is 8.73 A
        SDO_Write(epos, Motor_torque_constant, 0, 136000);              // 136 mNm/A
        SDO_Write(epos, Max_motor_speed, 0x00, 1000);					//参考电机手册
		SDO_Write(epos, Q_deceleration, 0x00, QDEC);                    //快速停止负加速度
		SDO_Write(epos, Profile_Deceleration, 0x00, MAX_DEC);           //快速停止负加速度
		SDO_Write(epos, Following_error_window, 0x00, MAX_F_ERR);       // Maximal Profile Velocit		
        SDO_Write(epos, Soft_P_Limit_Min, 0x01, 0x80000000);            //-2147483648
		SDO_Write(epos, Soft_P_Limit_Max, 0x02, 0x7FFFFFFF);            //2147483647
		SDO_Write(epos, Max_gear_input_speed, 0x03,1000);
		SDO_Write(epos,Max_Acceleration,0x00,10000);
		SDO_Write(epos, Interpolation_Time_index, 0, (uint8_t)-3);
        SDO_Write(epos, Interpolation_Time_Period_value, 0, 10);		// ms
		break;

	default:
		ERROR(12,"Error Mode\r\n");
    }
}


void Node_OperEn(Epos* epos){
	
    SDO_Write(epos,Controlword,0x00,Shutdown);                    // Shut down  驱动函数失能
    Epos_Delay(500);

    //SDO_Read(epos,OD_STATUS_WORD,0x00);                      // Ready to Switch On    Status=0x0121   绿灯闪烁
    
    SDO_Write(epos,Controlword,0x00,Switch_on_Enable_operation);                    // Switch on AND Enable Operation 驱动参数设定
    Epos_Delay(500);
    
    //SDO_Read(epos,OD_STATUS_WORD,0x00);                      // Operation Enable      Status=0x0137   绿灯常亮
}

void Node_DisEn(Epos* epos){
	
    SDO_Write(epos,Position_actual_value,0x00,0x06);                    // Disable Operation    Controlword=0xxx 0111
    Epos_Delay(500);
}


void Epos_Delay(Uint32 time){
	OSTimeDly(time);
}
