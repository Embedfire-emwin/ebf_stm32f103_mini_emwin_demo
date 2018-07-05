/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口拼音输入法
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "./usart/rx_data_queue.h"
#include "./flash/bsp_spi_flash.h"
#include "py_app.h"


#include <string.h>
#include <stdlib.h>


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
		
	//初始化FLASH
	SPI_FLASH_Init();

	//初始化串口接收队列
	rx_queue_init();
	
	/* 发送一个字符串 */
	printf("\r\nThis is an PingYin searching demo.\r\n");
	printf("\r\nPlease input PingYin to search.\r\n");

  while(1)
	{	
		//查询是否接收到串口数据
		search_char();
	}	
}
/*********************************************END OF FILE**********************/
