#include <BSP.h>

/*
 * 函数名：BSP_Init
 * 描述  ：时钟初始化、硬件初始化
 * 输入  ：无
 * 输出  ：无
 */
void BSP_Init(void)
{
	Core_Config();		/* 配置系统时钟为72M */
	SysTick_init();		/* 初始化并使能SysTick定时器 */
	Jlink_Init();
}

/*
 * 函数名：SysTick_init
 * 描述  ：配置SysTick定时器
 * 输入  ：无
 * 输出  ：无
 */
void SysTick_init(void)
{
	tick_init(1000);
}
