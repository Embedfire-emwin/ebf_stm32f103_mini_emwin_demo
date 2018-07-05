/**
  ******************************************************************************
  * @file    main.c
  * @author  野火
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   emwin 显示图片
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
#include ".\key\bsp_key.h"  
#include "GUIFont_Port.h"   

#include "ff.h"

#include "GUIFont_Port.h"   

FATFS fs;													/* Work area (file system object) for logical drives */


extern void drawbmp(void);
extern void drawJPEG(void);
extern void drawpng(void);
extern void drawgif(void);


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{		
	
	GUI_RECT info_text ={0,100,240,200};
	
	/* LED 端口初始化 */
	LED_GPIO_Config();	
	
	Key_GPIO_Config();
	
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
	printf("emWin picture demo\r\n");
	
		//创建XBF字体，使用UTF8编码
	Creat_XBF_Font();
	GUI_UC_SetEncodeUTF8();
	GUI_SetDefaultFont(&FONT_XINSONGTI_16);
	
	GUI_SetBkColor(GUI_WHITE);
  GUI_Clear(); 
  GUI_SetFont(GUI_FONT_8X16_ASCII);
  GUI_SetColor(GUI_BLUE);
	
	info_text.x0 = 10;
	info_text.x1 = 240-10;
	info_text.y0 = 100;
	info_text.y1 = 200;

	GUI_DispStringInRectWrap("Press 'KEY1' to show BMP format picture",
															&info_text,
															GUI_TA_CENTER,
															GUI_WRAPMODE_WORD) ;
	
	//挂载文件系统，必须先挂载文件系统再显示文件系统中的图片	
	f_mount(&fs,"1:",1);	
	
	while(1)
	{	
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			drawbmp();

			LED2_TOGGLE;
		}   
    
	  GUI_Delay(10);
	}


}


/*********************************************END OF FILE**********************/
