/**
  ******************************************************************************
  * @file    main.c
  * @author  野火
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   emwin 使用XBF字体
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"

#include "GUI.h"
#include ".\lcd\bsp_ili9341_lcd.h"
#include ".\lcd\bsp_xpt2046_lcd.h"
#include ".\usart\bsp_usart.h"
#include ".\SysTick\bsp_SysTick.h"
#include ".\led\bsp_led.h"
#include "GUIFont_Port.h"   

#include "ff.h"


extern void Fatfs_MainTask(void);


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();	
	
	XPT2046_Init();
  
  /* 初始化定时器 */
	SysTick_Init();
	
	/*CRC和emWin没有关系，只是他们为了库的保护而做的，这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	
	/* 初始化GUI */
	GUI_Init();
	
	/* 初始化串口 */
	USART_Config();
	
	/* 串口调试信息 */
	printf("emWin FatFs demo\r\n");
	
		//创建XBF字体，使用UTF8编码
	Creat_XBF_Font();
	GUI_UC_SetEncodeUTF8();
	GUI_SetDefaultFont(&FONT_XINSONGTI_16);
	
	Fatfs_MainTask();


}


/*********************************************END OF FILE**********************/
