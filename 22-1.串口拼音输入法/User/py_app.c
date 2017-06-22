/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   拼音检索应用
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include <stdlib.h>
#include "./usart/rx_data_queue.h"
#include "stm32f10x.h"
#include "py_app.h"


extern char *PYSearch(unsigned char *msg , int *num);
static uint8_t select_char(char *char_list,int char_num);

/**
  * @brief  根据输入的拼音查找字符，本函数需要在while中循环调用
  */
void search_char(void)
{
	QUEUE_DATA_TYPE *rx_data;	
	
	char *ptr;
	static uint16_t  s_Hznum=0;          
		
	/*从缓冲区读取数据，进行处理，*/
	rx_data = cbRead(&rx_queue); 

	if(rx_data != NULL)//缓冲队列非空
	{		
		//加上字符串结束符，方便直接输出字符串
		*(rx_data->head+rx_data->len) = '\0';
		
		ptr = PYSearch((unsigned char *)rx_data->head, (int *)&s_Hznum);
			
		QUEUE_DEBUG("receive data:%s",rx_data->head);
		
		//使用完数据必须调用cbReadFinish更新读指针
		cbReadFinish(&rx_queue);

		if (ptr != '\0')
		{
			uint8_t result;
			
			QUEUE_DEBUG("pyserach:%s",ptr);
			QUEUE_DEBUG("pynum:%d",s_Hznum);
			
			QUEUE_DEBUG("Please input num between:[1-%d] to select char,",s_Hznum);
			QUEUE_DEBUG("Input \"q\" to quit selecting,");

			do
			{ 
				result = select_char(ptr,s_Hznum);
			}while(result != 0 && result != 2  );
		}

	}
}


//输出选择的字符
//返回0，选择正常
//返回1，没有检测到输入
//返回2，用词退出选择
//返回3，选择错误
uint8_t select_char(char *char_list,int char_num)
{
	QUEUE_DATA_TYPE *rx_data;		
	
	int select_num;
	char select_char[4];
	uint8_t result = 1;
		
	/*从缓冲区读取数据，进行处理，*/
	rx_data = cbRead(&rx_queue); 

	if(rx_data != NULL)//缓冲队列非空
	{		
		//加上字符串结束符，方便直接输出字符串
		*(rx_data->head+rx_data->len) = '\0';
		
		select_num = atoi(rx_data->head);
		
		if( select_num > 0 && select_num <= char_num)
		{					
			//若字符数据存储在STM32FLASH，是不能直接修改的
			//复制字符数据到内存，便于添加结束符，
			memcpy(select_char,&char_list[(select_num-1)*3],3);
			select_char[3] = '\0';
			
			QUEUE_DEBUG("select_num = %d,select_char = %s",select_num,select_char);
			
			result = 0;
		}	
		else if(strcmp(rx_data->head,"q")==0)
		{
			QUEUE_DEBUG("quit selecting!");		
		
			result = 2;

		}
		else	
		{
			QUEUE_DEBUG("select error!");	
			QUEUE_DEBUG("Please input num between:[1-%d] to select char,",char_num);
			QUEUE_DEBUG("Input \"q\" to quit selecting.");			
				
			result = 3;
		}				

		//使用完数据必须调用cbReadFinish更新读指针
		cbReadFinish(&rx_queue);
	}
	else
	{
		result = 1;
	}
	
	return result;
}
