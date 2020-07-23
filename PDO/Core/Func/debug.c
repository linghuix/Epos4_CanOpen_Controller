/*
	串口打印调试代码
*/
#include "debug.h"


/*
 * author lhx
 * May 13, 2020
 *
 * @brief : in - point to null
 * 			out - point to char
 * Window > Preferences > C/C++ > Editor > Templates.
 */



//G T R Vt

extern UART_HandleTypeDef huart1;
uint8_t ch_print;
uint8_t ch_scanf;
struct Buffer debugBuffer = {"inital",0,0};


/*
 * author lhx
 * May 13, 2020
 *
 * @brief : 向buff中添加数据，buffer满了则返回0
 * Window > Preferences > C/C++ > Editor > Templates.
 */

uint8_t addDebugBuffer(char c)
{
	if((debugBuffer.in+1)%BufferSize == debugBuffer.out)
		return 0;
	debugBuffer.data[debugBuffer.in] = c;
	debugBuffer.in = (debugBuffer.in + 1)%BufferSize;
	return 1;
}
/*
 * author lhx
 * May 13, 2020
 *
 * @brief : 获取buff中的数据，buffer为空则返回0
 * Window > Preferences > C/C++ > Editor > Templates.
 */
char getDebugBuffer(void)
{
	char c;
	if(debugBuffer.in == debugBuffer.out)
			return 0;
	c = debugBuffer.data[debugBuffer.out++];
	debugBuffer.out = (debugBuffer.out)%BufferSize;
	return c;
}

/*
 * author lhx
 * May 14, 2020
 *
 * @brief : STM32CubeIDE printf 重定位到 PORT
 * Window > Preferences > C/C++ > Editor > Templates.
 */
#ifndef ITM_dbg
int _write(int file, char *ptr, int len)
{
	int DataIndex;

	for (DataIndex = 0; DataIndex < len; DataIndex++)
	{
		addDebugBuffer( *ptr++ );
	}
	__HAL_UART_ENABLE_IT(&PORT, UART_IT_TXE);
	return len;
}
#endif
//printf 重定向
#ifdef __GNUC__		// STM32CubeIDE

#else

int fputc(int c, FILE * f)		// Keil
{
    ch_print = c;
    HAL_UART_Transmit(&PORT,&ch_print,1,1000);//���ʹ���
    return c;
}
#endif



//scanf 重定向
#ifdef __GNUC__		// STM32CubeIDE

#else

int fgetc(FILE * F)		// Keil
{
    HAL_UART_Receive(&PORT,&ch_scanf,1,0xffff);//����
    return ch_scanf;
}
#endif

/*
 * author lhx
 * May 13, 2020
 *
 * @brief : Enable serial NVIC
 *			Set serial property
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void debug_init(void)
{
	MX_USART1_UART_Init();
}

/*
 * author lhx
 * May 13, 2020
 *
 * @brief : 中断处理函数
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void debug_IRQ(void)
{
	uint32_t isrflags   = READ_REG(huart1.Instance->SR);
	uint32_t cr1its     = READ_REG(huart1.Instance->CR1);
	char c;
	/* UART in mode Transmitter -----------TXE ------------------------------------*/
	if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET)){
	  c = getDebugBuffer();
	  if(c != 0){
		  huart1.Instance->DR = (uint16_t)( c & (uint16_t)0x01FF);
	  }
	  else{
		  /* Disable the UART Transmit Complete Interrupt */
		  __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);
		  //__HAL_UART_DISABLE_IT(huart, UART_IT_TC);
	  }
	}

	/* UART in mode Receiver -------------------RXNE------------------------------*/
	/*if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET)){
		UART_Receive_IT(huart1);
		return;
	}*/
}

/**
  ******************************************************************************
  * @section    Test
  * @author  xlh
  * @brief   
  ******************************************************************************
  */
//@@@@@@@@@@@@@T@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ test
#ifdef TEST_ON
	#include "main.h"

	TEST test_SpeedOfBuffer_printf(void)
	{
		Core_Config();
		tick_init(1000000);
		debug_init();
		int i;
		while(1){
			i=50;
			while(i--)
				printf("%d , %ld - send buffer test for skipping error possibility and speeding: in-%d-out-%d\r\n",i,HAL_GetTick(),debugBuffer.in,debugBuffer.out);
			printf("delay 2000ms\r\n");
			i = 100000000;
			while(i--);
		}
	}

	uint8_t test_buffRX[10];
	TEST test_printf(void)
	{
		Core_Config();
		debug_init();

		uint8_t test_data[] = "hello word!\r\n";
		HAL_UART_Transmit_IT(&huart1, test_data, sizeof(test_data));
		HAL_Delay(10);
		
		/* */
		MSG_WAR(1,"seral send OK!\r\n", 54);
	

		/*scanf��printf*/
		MSG("test scanf ... \r\nplease enter a num");
		int x = 365;
	//	scanf("%d",&x);
		printf("receive : %d\r\n",x);

		/*IT*/
		MSG("please input one hex data\r\n");
		MSG("it will return 0x0A, that is the last char when you input to scanf function\r\n");
		for(int i; i<10; i++){
			test_buffRX[i] = i;
		}
		HAL_UART_Receive_IT(&huart1, test_buffRX, 2);
	}

	
	void test_RXTX_callback(void)
	{
		static uint8_t msg[2];
		for(int i=0; i<5; i++){
			msg[i] = test_buffRX[i];
		}
			HAL_UART_Transmit_IT(&huart1, msg, 2);
			HAL_UART_Receive_IT(&huart1, test_buffRX, 2);
	}
	
	
	/*void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		if(huart->Instance==USART1)
		{
			test_RXTX_callback();
		}
	}
	*/
#endif
