/**
  ******************************************************************************
  * @file    main.c
  * @author  野火
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   emwin速度测试程序
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
#include ".\usart\bsp_usart.h"
#include ".\SysTick\bsp_SysTick.h"
#include ".\led\bsp_led.h"

#include "GUIDEMO.h"


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();	
  
  /* 初始化定时器 */
	SysTick_Init();
	
	/*CRC和emWin没有关系，只是他们为了库的保护而做的，这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	
	/* 初始化GUI */
	GUI_Init();
	
	/* 初始化串口 */
	USART_Config();
	
	printf("\r\n ********** emwin速度测试程序*********** \r\n"); 
	
 GUI_SetColor(GUI_WHITE);   
	/* 显示测试 */
	GUI_DispStringAt(" emWin Speed test Demo",10,10);
	GUI_Delay(2000);
   

	//移植本程序时，注意要把启动文件中的栈空间设置大一点

	//emwin官方demo，修改guidemo.h文件中的宏可增加演示应用
	GUIDEMO_Main();


}


/*********************************************END OF FILE**********************/
