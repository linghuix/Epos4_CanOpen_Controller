#include <stdio.h>
#include <prinf_debug_ITM.h>


#include "main.h"

#ifdef ITM_dbg
//重定位到 ITM port0 输出字符
int _write(int file, char *ptr, int len)
{
	int DataIndex;

	for (DataIndex = 0; DataIndex < len; DataIndex++)
	{
		ITM_SendChar( *ptr++ );
	}
	return len;
}

#endif


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ test

void test_ITM_debug(void)
{
	Core_Config();
	tick_init(1);
	MSG_ITM("USART1_UART_Init\r\n");
	HAL_Delay(250);
}
