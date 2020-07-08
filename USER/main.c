
/******************** (C) COPYRIGHT 2019 HOME Team **************************
 * 描述    ：通过单片机CAN总线，发送数据给控制器，实现多个电机的控制      
 * 实验平台：STM32F103最小系统+CAN接收器+TTL转USB+EPOS2 50/5控制器+maxon DC电机
 * 库版本  ：ST3.5.0
*****************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "can.h"
#include "delay.h"
#include "timer.h"
#include "HW_epos.h"

// 描述  : "主机"的主函数 
int main(void){

    SysTick_Init();                             //延迟中断定时器
    USART1_Config();                            //初始化串口模块
    
    TIME_NVIC_Configuration();
    TIME2_Configuration();
    
    CAN_Config();                               //配置CAN模块   
    
    printf("\r\n\r\n\r\nboard init complete!\r\n");  
    
		Epos_INIT();
    
    while(1){
			//Epos_ReceiveDate();
			//Epos_ControlTask();
    }
}
