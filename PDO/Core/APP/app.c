/*
 * author lhx
 * May 16, 2020
 *
 * @brief : 任务文件
 * Window > Preferences > C/C++ > Editor > Templates.
 */


#include "app.h"

INT8U error;//错误标志

OS_STK canrcv_task_stk[CAN_RECEIVE_STK];		  	//定义栈
OS_STK cansend_task_stk[TASK_B_STK_SIZE];
OS_STK remote_task_stk[TASK_remote_STK_SIZE];
OS_STK canapp_task_stk[TASK_canapp_STK_SIZE];

/********************************** 任务函数**********************************/

/*
 * author lhx
 * May 16, 2020
 ******
 * Task_Start
 ******
 * @brief : 初始任务
 * 	Running at first
 *	to initialize other task
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void CANOpen_App_Init(void);
void Remote_App_Init(void);
void Task_Start(void *p_arg)
{
  (void)p_arg;                // 'p_arg' 并没有用到，防止编译器提示警告

	Task_MSG("Canopen motor controller ...\r\n");
	Task_MSG("task priority include: 2 3 5 6 7 \r\n");
	
	OSStatInit();
	CANOpen_App_Init();			//CANOpen协议初始化
	Remote_App_Init();		// 红外通讯远程信息接收初始化

	while (1){
		OSTimeDlyHMSM(0, 100,0,0);
	}
}


/*
 * author test
 *
 * @brief : CANOpen初始化程序
 *			important tasks for canopen :
 * 			1. can receive
 * 			2. can trasmit
 * 			3. Epos Inital
 * Window > Preferences > C/C++ > Editor > Templates.
 */

OS_Q *	gg;
OS_EVENT * CRCV_WAIT_Semp;	//等待CAN接收
#include "func_CanOpen.h"
void CANOpen_App_Init(void)
{
	MX_CAN1_Init(CAN_MODE_NORMAL);
	MX_TIM_CounterInterrupt(CANOPEN_TIMx, CANOPEN_TIM_COUNTER_CLOCK, CANOPEN_TIM_PERIOD);

	//HAL_TIM_Base_Start_IT(CANOPEN_TIMx_handle);			//在EposMaster_Start函数中启动
	CAN_Start(&hcan1);
	

	/* 操作系统管理 */
	CRCV_WAIT_Semp = OSSemCreate(0);
	CANRcv_Q  = OSQCreate(&canrxMsgGrp[0],N_MESSAGES); 			//创建消息队列
	CANSend_Q = OSQCreate(&cantxMsgGrp[0],N_MESSAGES);
	gg = (OS_Q*)CANRcv_Q->OSEventPtr;	/* 调试 void指针难以调试，因此需要强制转化为具体的结构体 */


	/* 高级任务，可以调试使用堆栈的大小 OS_TASK_OPT_STK_CHK */
	OSTaskCreateExt(CANRcv_Task,(void *)0,&canrcv_task_stk[CAN_RECEIVE_STK-1],
		TASK_canrcv_PRIO,0,&canrcv_task_stk[0],CAN_RECEIVE_STK,(void *)0,(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	OSTaskCreateExt(CANSend_Task,(void *)0,&cansend_task_stk[TASK_B_STK_SIZE-1],
			TASK_cansend_PRIO,0,&cansend_task_stk[0],TASK_B_STK_SIZE,(void *)0,(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
	
	OSTaskCreateExt(Epos_Task,(void *)0,&canapp_task_stk[TASK_canapp_STK_SIZE-1],
		TASK_canapp_PRIO,0,&canapp_task_stk[0],TASK_canapp_STK_SIZE,(void *)0,(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
	
		/*OSTaskCreate(CANRcv_Task,(void *)0,		  				//创建CANRcv_Task 初级任务
	   &canrcv_task_stk[TASK_A_STK_SIZE-1], TASK_canrcv_PRIO);
		OSTaskCreate(CANSend_Task,(void *)0,				   		//创建CANSend_Task任务
	  &cansend_task_stk[TASK_B_STK_SIZE-1], TASK_cansend_PRIO);
		OSTaskCreate(CANApp_Task,(void *)0,					  	//创建CANApp_Task任务
		 &canapp_task_stk[TASK_A_STK_SIZE-1], TASK_canapp_PRIO);*/
}


/*
 * author test
 *
 * @brief : EPOS初始化程序
 *
 * 			1.SDO配置  2.NMT 进入ENABLE模式  3.PDO控制
 * Window > Preferences > C/C++ > Editor > Templates.
 */
extern int epos_state;
extern uint8_t NumControllers;



#include "conf_epos.h"

/*轨迹曲线队列*/
OS_EVENT * Trajectory_Q_1;
void * trajectoryPointer_1[5];
trajectory trajectoryBuffer_1[5];

void Epos_Task(void *p_arg)
{
	//Task_MSG("CANApp_Task ... ");
	uint32_t data=50;
	EposMaster_Init();
	EposMaster_Start();
	
	Trajectory_Q_1 = OSQCreate(&trajectoryPointer_1[0],5);
	
	for(;;)
	{
		if(epos_state == 0){
			OSTimeDlyHMSM(0, 0,0,800); 		//waiting for PDO controlling process in EPOS end
			for(int i= 0;i < NumControllers;i++){
				masterNMT(&TestMaster_Data, Controller[i], NMT_Enter_PreOperational);	//to operation
				SDO_Write(Controller[i], Max_Profile_Velocity, 0x00, 500);				//reset speed set slower
			}
			
			
			Node_To_Home_Postion(Controller[0]);
			Node_To_Home_Postion(Controller[1]);
			Node_To_Home_Postion(Controller[2]);
			Node_To_Home_Postion(Controller[3]);
			OSTimeDlyHMSM(0, 0, 5,0); 
			
			data = SDO_Read(Controller[0], Position_actual_value, 0X00);
			MSG("get - %x\r\n",data);
			data = SDO_Read(Controller[1], Position_actual_value, 0X00);
			MSG("get - %x\r\n",data);
			
			epos_state = 50;
			
			/*for(int i= 0;i < NumControllers;i++){
				Node_DisEn(Controller[i]);
				SDO_Write(Controller[i], Max_Profile_Velocity, 0x00, MAX_P_V);				//reset to previous speed 
			}*/
		}
		OSTimeDlyHMSM(0, 0,0,10); 
	}
}


INT8U err;
//CAN 接收任务
OS_SEM_DATA  sem_data;
void CANRcv_Task(void *p_arg)
{	
	(void)p_arg; 
	
	CanRxMsg *RxMsg;
	Message msg;
	uint8_t i = 0;
 
	while(1){
		
		if((RxMsg = (CanRxMsg *)OSQPend(CANRcv_Q, 0, &error)) != (void *)0){		/* 等待时间无穷大 */
			msg.cob_id = RxMsg->head.StdId;				   //CAN-ID
			
			if(CAN_RTR_REMOTE == RxMsg->head.RTR)
				msg.rtr = 1;																			//远程帧
			else
				msg.rtr = 0;																			//数据帧
			msg.len = (UNS8)RxMsg->head.DLC;				//长度
			
			CAN_RCV_MSG("CAN Rcv 0x%x  ",msg.cob_id);
			for(i=0; i<msg.len; i++){								//数据
				msg.data[i] = RxMsg->Data[i];
				CAN_RCV_MSG("-0x%x ",msg.data[i]);
			}
			CAN_RCV_MSG("\r\n");

			//HAL_TIM_Base_Stop_IT(CANOPEN_TIMx_handle);
			canDispatch(&TestMaster_Data, &msg);	   //处理接收到的CAN帧，调用协议库相关接口
			//HAL_TIM_Base_Start_IT(CANOPEN_TIMx_handle);

			if(waiting_sdo == 1){
				OSSemQuery (CRCV_WAIT_Semp, &sem_data);
				if(sem_data.OSCnt < 1)		OSSemPost(CRCV_WAIT_Semp);
			}
		}
	}
}

//CAN 发送任务
void CANSend_Task(void *p_arg)
{
	CanTxMsg * TxMsg;
	uint8_t HAL_status;
	(void)p_arg; 
	
	while (1){
		if((TxMsg = (CanTxMsg *)OSQPend(CANSend_Q, 0, &error)) != (void *)0)	 //获取发送队列中数据    /* 等待时间无穷大 */
		{
			static MAIL pmailbox;
			HAL_status = MX_CANx_send(pHCANx, TxMsg, pmailbox);
			while( HAL_status != HAL_OK ){										//发送失败, 延时1个滴答, 再次发送
				OSTimeDly(1);
				HAL_status = MX_CANx_send(pHCANx, TxMsg, pmailbox);
			}
			OSTimeDlyHMSM(0, 0,0,2); 
		}
	}
}


void Remote_App_Init(void)
{
	/* 串口配置 */
	MX_USART2_UART_Init();
	/*OSTaskCreate(Remote_Task,(void *)0,		  				//创建 infra-red 控制任务
		   &remote_task_stk[TASK_A_STK_SIZE-1], TASK_remote_PRIO);*/
	
	OSTaskCreateExt(RemoteController_Task,(void *)0,&remote_task_stk[TASK_A_STK_SIZE-1],
		TASK_remote_PRIO,0,&remote_task_stk[0],TASK_A_STK_SIZE,(void *)0,(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
}

/*
 * author test
 *
 * @brief : infrared controller to control EPOS
 * Window > Preferences > C/C++ > Editor > Templates.
 */
uint8_t remoteData[3] = {0};
void RemoteController_Task(void *p_arg)
{
	(void)p_arg;
	HAL_UART_Receive_IT(&huart2, remoteData,3);
	
	while (1){
		OSTimeDlyHMSM(0, 0,0,5);
	}
}


extern int PERIOD,period;	//canopen_interface.c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2){
		uint8_t receiveData = remoteData[2];
		switch(receiveData){
			case(0x0C):
				PERIOD = 1;
				break;
			case(0x18):
				PERIOD = 2;
				break;
			case(0x19):
				PERIOD = 60*60/7;
				break;
			case(0x0D):
				PERIOD = 120*60/7;
				break;
			default:
				ERROR(10,"error command 0x%X\r\n",receiveData);
		}
	}
	MYMSG("#%d\t%d\r\n",period,PERIOD);
	HAL_UART_Receive_IT(&huart2, remoteData,3);
}

