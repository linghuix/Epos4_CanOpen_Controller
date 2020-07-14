/*******************************************************************************
 * 文件名  ：can.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：野火STM32开发板
 * 硬件连接：------------------------
 *          |       PB8-CAN-RX       |
 *          |       PB9-CAN-TX       |
 *           ------------------------
 * 库版本  ：ST3.5.0
 * 作者    ：保留
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/

#include "can.h" 
#include "epos.h"

CanTxMsg TxMessage;                     //发送缓冲区
CanRxMsg RxMessage;                     //接收缓冲区

/*
 * 函数名：CAN_GPIO_Config
 * 描述  ：CAN的GPIO 配置,PB8上拉输入，PB9推挽输出
 * 调用  ：内部调用
 */
static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*外设时钟设置*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);                           // 重映射IO功能为can

    /* Configure CAN pin: RX */                                                  // PB8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Configure CAN pin: TX */                                                // PB9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
}


/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
         * 发送报文中断
         * 接收FIFO报文中断，USB_LP_CAN1_RX0_IRQn
         * 错误中断
 */
static void CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // Configure one bit for preemption priority 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    //中断设置
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;      //CAN1 RX0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*
 * 函数名：CAN_Mode_Config
 * 描述  ：CAN的模式 配置
 * 调用  ：内部调用
 */
static void CAN_Mode_Config(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    /************************CAN通信参数设置**********************************/
    /*CAN寄存器初始化*/
    CAN_DeInit(CAN1);
    CAN_StructInit(&CAN_InitStructure);
    /*CAN单元初始化*/
    CAN_InitStructure.CAN_TTCM=DISABLE;                 //MCR-TTCM  关闭时间触发通信模式使能
    CAN_InitStructure.CAN_ABOM=ENABLE;                  //MCR-ABOM  自动离线管理 
    CAN_InitStructure.CAN_AWUM=ENABLE;                  //MCR-AWUM  使用自动唤醒模式
    CAN_InitStructure.CAN_NART=DISABLE;                 //MCR-NART  禁止报文自动重传      DISABLE-自动重传
    CAN_InitStructure.CAN_RFLM=DISABLE;                 //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
    CAN_InitStructure.CAN_TXFP=DISABLE;                 //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //正常工作模式
    CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;          //BTR-SJW 重新同步跳跃宽度 2个时间单元
    CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;          //BTR-TS1 时间段1 占用了6个时间单元
    CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;          //BTR-TS1 时间段2 占用了3个时间单元
    CAN_InitStructure.CAN_Prescaler =4;                 //BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+5+3)/4= 1Mbps EPOS比特率默认为1Mbps
    CAN_Init(CAN1, &CAN_InitStructure);
}


/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 调用  ：内部调用
 */
static void CAN_Filter_Config(void)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    //CAN过滤器初始化
    CAN_FilterInitStructure.CAN_FilterNumber=0;                                         //过滤器组0
    CAN_FilterInitStructure.CAN_FilterMode =CAN_FilterMode_IdList;  //工作在标识符列表模式
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;  //过滤器位宽为单个32位。
    // 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 

    CAN_FilterInitStructure.CAN_FilterIdHigh= (u16)0x581<<5;                //要过滤的ID RTR=0,IDE=0  过滤器编号 2
    CAN_FilterInitStructure.CAN_FilterIdLow = (u16)0x582<<5;                //过滤器编号 0
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0000;           
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;           //过滤器低16位每位不必匹配
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;             //过滤器被关联到FIFO的filter0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;            //使能过滤器
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    //CAN通信中断使能
    //CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);*/
}


/*
 * 函数名：CAN_Config
 * 描述  ：完整配置CAN的功能
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  //CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();   
}



/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */  
/*
void CAN_SetMsg(uint32_t StdId, uint8_t IDE, uint32_t *p)
{     
  int i;
    
    TxMessage.StdId = StdId;                         
  //TxMessage.ExtId=0x1314;                                                                     //使用的扩展ID
    
  TxMessage.IDE = (IDE==0)? CAN_ID_STD:CAN_ID_EXT;                  //扩展模式
  TxMessage.RTR=CAN_RTR_DATA;                                                                   //发送的是数据
  TxMessage.DLC=8;                                                                                      //数据长度为2字节
  
    for(i = 0;i<TxMessage.DLC;i++){
        TxMessage.Data[i]=p[i];
    }
}*/

//TxMessage.Data[0]为低地址
void CAN_SetMsg(uint32_t ID, uint32_t low, uint32_t high){
    
    
    uint32_t tmp;
    uint8_t mail;
    int i;
    
    //low = 0x0EFEFABCD;
    //high = 0x0ABCDEFAD;
    
    TxMessage.StdId = ID;                        
    
  TxMessage.IDE = CAN_ID_STD;                   //扩展模式
  TxMessage.RTR = CAN_RTR_DATA;             //发送的是数据
  TxMessage.DLC = 8;                                    //数据长度为8字节

    tmp = low;                                                  //0-cs，1,2-Index  ,3-Sub
    for(i=0;i<4;i++){
        TxMessage.Data[i] = low;
        low = low>>8;
    }
    
    tmp = high;
    
    for(i=4;i<8;i++){
        TxMessage.Data[i] = tmp;
        tmp = tmp>>8;
    }
    mail = CAN_Transmit(CAN1, &TxMessage);
    
    i= 50;
    while(CAN_TransmitStatus(CAN1, mail) != CANTXOK && --i>0){};    //发送不成功的话，等待
    
    //CAN_Transmit(CAN1, &TxMessage);
}

