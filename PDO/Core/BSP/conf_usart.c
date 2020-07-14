
/* Includes ------------------------------------------------------------------*/
#include "conf_usart.h"
//#include "conf_global.h""

GPIO_InitTypeDef GPIO_InitStruct;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

DMA_HandleTypeDef hdma_usart1_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	
  if (HAL_UART_Init(&huart1) != HAL_OK){
    Error_Handler()
  }
}

/*
 * author lhx
 * May 13, 2020
 *
 * @brief : 调用自HAL_UART_MspInit
 * 			引脚初始化
 * 			时钟开启
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void USART_Hardware_Init(UART_HandleTypeDef* uartHandle)
{
    if(uartHandle->Instance==USART1)
    {
		/*
		UART1 TX���� PA9
		UART1 RX���� PA10
		*/
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		//    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
		//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		//    GPIO_InitStruct.Pull = GPIO_NOPULL;
		//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    }
    else if(uartHandle->Instance==USART2)
    {

      /* Peripheral clock enable */
      __HAL_RCC_USART2_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();

      /**USART2 GPIO Configuration
      PA3     ------> USART2_RX
      PA2     ------> USART2_TX
      */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
    else if(uartHandle->Instance==USART3)
    {
      /* Peripheral clock enable */
      __HAL_RCC_USART3_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();

      /**USART3 GPIO Configuration
      PB11     ------> USART3_RX
      PB10     ------> USART3_TX
      */
      
	/*GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;  这种配置，可以发送，但是不能接受数据
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);*/

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

/*
 * author lhx
 * May 13, 2020
 *
 * @brief : 串口中断功能设置和开启
 * Window > Preferences > C/C++ > Editor > Templates.
 */
void USART_NVIC_Init(UART_HandleTypeDef* uartHandle)
{
    if(uartHandle->Instance==USART1)
    {
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);

    }
    else if(uartHandle->Instance==USART2)
    {
		HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    else if(uartHandle->Instance==USART3)
    {
		/* Peripheral interrupt init */
		HAL_NVIC_SetPriority(USART3_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
}


void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_MultiProcessor_Init(&huart2, 0, UART_WAKEUPMETHOD_IDLELINE) != HAL_OK)
  {
    Error_Handler()
  }

}


void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_MultiProcessor_Init(&huart3, 0, UART_WAKEUPMETHOD_IDLELINE) != HAL_OK)
  {
    Error_Handler()
  }

}

/*
 * author lhx
 * May 13, 2020
 *
 * @brief : 调用自 HAL_UART_Init，用于硬件初始化
 * Window > Preferences > C/C++ > Editor > Templates.
 */

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	USART_Hardware_Init(uartHandle);
	USART_NVIC_Init(uartHandle);
}


void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PB7     ------> USART1_RX
    PB6     ------> USART1_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7|GPIO_PIN_6);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART1_IRQn);

  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PD6     ------> USART2_RX
    PD5     ------> USART2_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6|GPIO_PIN_5);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART2_IRQn);

  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PD9     ------> USART3_RX
    PD8     ------> USART3_TX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_9|GPIO_PIN_8);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART3_IRQn);

  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
} 

void USART_init(void)
{
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
