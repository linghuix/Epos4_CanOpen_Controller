/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：建立3个任务，每个任务控制一个LED，以固定的频率轮流闪烁（频率可调）。         
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.0.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "main.h"


OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		  //定义栈

/*
 * author lhx
 * May 16, 2020
 *
 * @brief :
 * 			最最基础的硬件初始化
 * 			启动操作系统
 * 			启动最初的一个APP - Task_Start
 * Window > Preferences > C/C++ > Editor > Templates.
 */

int main(void)
{
	BSP_Init();
	debug_init();
	//CAN_Send_test();

	OSInit();
	MSG("--------------------------------------                   \r\n");
	MSG("   --------------------------------------                   \r\n");
	MSG("      --------------------------------------                   \r\n");
	MSG("         --------------------------------------                   \r\n");
	MSG("Welcome to EPOS Controller !\r\n");
	MSG("         --------------------------------------                   \r\n");
	MSG("      --------------------------------------                   \r\n");
	MSG("   --------------------------------------                   \r\n");
	MSG("--------------------------------------                   \r\n");

	OSTaskCreate(Task_Start,(void *)0, &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);

	OSStart();

	while(1){
		printf("system not start !");
	}
	//return 0;
}


/*
*********************************************************************************************************
*
*                                              TaskIdleHook
*
*********************************************************************************************************
*/


/*
 * author lhx
 * May 16, 2020
 *
 * @brief :  空闲闪烁灯提示
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void App_TaskIdleHook(void){
	LEDSwich;
}


void          App_TaskCreateHook      (OS_TCB          *ptcb){};

void          App_TaskDelHook         (OS_TCB          *ptcb){};

void          App_TaskReturnHook      (OS_TCB          *ptcb){};



/*
 * author lhx
 * May 16, 2020
 *
 * @brief : CPU 使用率提示
 * 			堆栈数据统计
 * @Note(s)    : 必须OS启动后才能调用
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void          App_TaskStatHook        (void){
	
	OSSchedLock();
	
	//Task_MSG("CPUsage = %d %% ", OSCPUUsage);
	
	/*Task_MSG("prior task\t\tstack used ");
	
	OS_TCB *ptcb;
	INT8U prio;
	for (prio = 0u; prio <= OS_TASK_IDLE_PRIO; prio++) {
			ptcb = OSTCBPrioTbl[prio];
			if (ptcb != (OS_TCB *)0) {                                //Make sure task 'ptcb' is ...
					if (ptcb != OS_TCB_RESERVED) {                       // ... still valid.
#if OS_TASK_PROFILE_EN > 0u
					printf("%d\t\t\t%d/%d\r\n",prio, ptcb->OSTCBStkUsed, ptcb->OSTCBStkSize);
#endif
					}
			}
	}*/
	
	OSSchedUnlock();
};

/*
 * author lhx
 * May 16, 2020
 *
 * @brief : 任务切换提示
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void          App_TaskSwHook          (void){
	
	/*printf("\r\n%d:%d->%d\r\n",OSTime,OSTCBCur->OSTCBPrio,OSTCBHighRdy->OSTCBPrio);*/

};

/*TCB初始化时调用的函数，可以在这里定义TCB的名称，等属性。此时传入TCB优先级等都已经初始化好了*/
void          App_TCBInitHook         (OS_TCB          *ptcb){
	//ptcb->OSTCBTaskName =  &(ptcb->OSTCBPrio);
};

void          App_TimeTickHook        (void){};



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
