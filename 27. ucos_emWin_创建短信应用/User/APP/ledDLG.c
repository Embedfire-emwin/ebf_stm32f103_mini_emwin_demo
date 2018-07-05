/**
  ******************************************************************************
  * @file    ledDLG.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LED灯的应用窗口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 MINI 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
// USER START (Optionally insert additional includes)
#include "includes.h"


// USER END
/**************************************************************************************
*
*       Defines
*
***************************************************************************************
*/


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
// USER START (Optionally insert additional static data)
// USER END

/*--------------------  (使用U2C.exe小工具生成)  ------------------------------------*/
/*
*  U2C.exe小工具使用方法：新建一个TXT文本文件，输入待转换文本，另存为UTF8编码格式文件，
*  打开U2C.exe小工具，先导入文件文件，然后选择"Convert"进行转换，完成后在同路径下输出
*  同名.c文件。
*/
#define UTF8_KEY_CONTROL     "\xe6\x8c\x89\xe9\x94\xae\xe6\x8e\xa7\xe5\x88\xb6LED\xe4\xba\xae\xe7\x81\xad"  //按键控制LED亮灭
#define UTF8_LED      					"\xe6\xb5\x81\xe6\xb0\xb4\xe7\x81\xaf"   //流水灯
#define UTF8_TOUCH_CAL      	"\xe8\xa7\xa6\xe6\x91\xb8\xe5\x81\x8f\xe5\xb7\xae\xe5\xa4\xa7\xe6\x89\x8d\xe9\x9c\x80\xe8\xa6\x81\xe8\xa7\xa6\xe6\x91\xb8\xe6\xa0\xa1\xe5\x87\x86"     //触摸偏差大才需要触摸校准
#define UTF8_CAL_BUTTON       "\xe8\xa7\xa6\xe6\x91\xb8\xe6\xa0\xa1\xe5\x87\x86"//触摸校准


/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateLED[] = {
  { FRAMEWIN_CreateIndirect, "KEY", 0, 0, 0, 240, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "KEY1", GUI_ID_BUTTON0, 20, 55, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "KEY2", GUI_ID_BUTTON1, 120, 55, 80, 40, 0, 0x0, 0 },
	{ SLIDER_CreateIndirect, "LED_Slider", GUI_ID_SLIDER0, 35, 135, 170, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Calibration", GUI_ID_BUTTON2, 118-60, 240, 120, 40, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogLED(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	uint16_t i,v;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) 
  {
	  case WM_DELETE:
		// USER START (Optionally insert additional code for further widget initialization)
			tpad_flag=0;
		  APP_TRACE_DBG(("LED app delete\n"));
			ICON_Clicked[0] = 0;
			UserApp_Running = 0;		
			LED1_OFF;LED2_OFF;
		// USER END
		break;
	  case WM_INIT_DIALOG:
		//
		// Initialization of 'KEY TEST'
		//
			hItem = pMsg->hWin;
			FRAMEWIN_SetTitleHeight(hItem, 20);
			FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
			FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
			FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);		
			//
			// Initialization of 'Calibration'
			//
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
			BUTTON_SetText(hItem, UTF8_CAL_BUTTON);
			BUTTON_SetFont(hItem, &FONT_XINSONGTI_16);
		// USER START (Optionally insert additional code for further widget initialization)		
		// USER END
		break;
	  case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
		switch(Id) {
		case GUI_ID_BUTTON0: // Notifications sent by 'KEY1'
		  switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
				LED1_ON;
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
		    LED1_OFF;
			// USER END
			break;
		  case WM_NOTIFICATION_MOVED_OUT:
			// USER START (Optionally insert code for reacting on notification message)
				LED1_OFF;
			// USER END
			break;
		  // USER START (Optionally insert additional code for further notification handling)
		  // USER END
		  }
		  break;
		case GUI_ID_BUTTON1: // Notifications sent by 'KEY2'
		  switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
				LED2_ON;
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
				LED2_OFF;
			// USER END
			break;
		   case WM_NOTIFICATION_MOVED_OUT:
			// USER START (Optionally insert code for reacting on notification message)
				LED2_OFF;
			// USER END
			break;
		  // USER START (Optionally insert additional code for further notification handling)
		  // USER END
		  }
		  break;
		case GUI_ID_BUTTON2: // Notifications sent by 'LED3'
		  switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
				SPI_FLASH_SectorErase(CALADD);
				Soft_Reset();
			//BUTTON_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0), "请按复位键");
			// USER END
			break;
		  case WM_NOTIFICATION_VALUE_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END			    
				break;
		  }
		  break;
		case GUI_ID_SLIDER0: // Notifications sent by 'LED_Slider'
		  switch(NCode) 
		  {
			  case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			  case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
					hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0);
					v = SLIDER_GetValue(hItem);
					WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0));
					WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1));
					WM_DisableWindow(hItem);
			    //printf("delay time=%d\n",v);
					i=10;
					while(i--)
					{
						LED1_ON;
						GUI_Delay(v);
						LED1_OFF;
						LED2_ON;
						GUI_Delay(v);
						LED2_OFF;
						if(UserApp_Running == 0)return;
					}
					WM_EnableWindow(hItem);
					WM_EnableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0));
					WM_EnableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1));
					LED1_OFF;LED2_OFF;
				break;
				
			  case WM_NOTIFICATION_VALUE_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
		  }
		  break;
		// USER START (Optionally insert additional code for further Ids)
		// USER END
		}
		break;
	  // USER START (Optionally insert additional message handling)
		case WM_PAINT:	
			GUI_SetBkColor(APPBKCOLOR);
			GUI_SetColor(GUI_MAGENTA);
			GUI_Clear();
			GUI_SetTextMode(GUI_TM_TRANS);
			GUI_DrawGradientV(0, 205, 230, 295, GUI_CYAN, GUI_CYAN);		
			GUI_DispStringHCenterAt(UTF8_KEY_CONTROL,110,20);
			GUI_DispStringHCenterAt(UTF8_LED,110,110);					
			GUI_DispStringHCenterAt(UTF8_TOUCH_CAL,110,215);
		break;
	  // USER END
	  default:
			WM_DefaultProc(pMsg);
		break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateKEY TEST
*/
void FUN_ICON0Clicked(void)
{
	WM_HWIN hWin;
	OS_ERR  os_err;
	
	hWin=GUI_CreateDialogBox(_aDialogCreateLED, 
															GUI_COUNTOF(_aDialogCreateLED),
															_cbDialogLED,
															WM_HBKWIN, 
															0, 
															0);
	
	APP_TRACE_DBG(("LED app create\n"));
	
	while(UserApp_Running)
	{				 				

			//等待按键
		 if(OSFlagPend ((OS_FLAG_GRP *)&key_flag_grp,                  
											(OS_FLAGS     )( EVEN_KEY1_DOWN ),
											(OS_TICK      )0,
											(OS_OPT       )OS_OPT_PEND_FLAG_SET_ALL |
																		 OS_OPT_PEND_NON_BLOCKING,
											(CPU_TS      *)0,
											(OS_ERR      *)&os_err) == EVEN_KEY1_DOWN)
		 {
		 		//等待到后清除标志
			OSFlagPost ((OS_FLAG_GRP  *)&key_flag_grp,                             
										(OS_FLAGS      )EVEN_KEY1_DOWN,
										(OS_OPT        )OS_OPT_POST_FLAG_CLR,
										(OS_ERR       *)&os_err);
			 
			 LED1_TOGGLE;

		 }
		 
		 			//等待按键
		 if(OSFlagPend ((OS_FLAG_GRP *)&key_flag_grp,                  
											(OS_FLAGS     )( EVEN_KEY2_DOWN ),
											(OS_TICK      )0,
											(OS_OPT       )OS_OPT_PEND_FLAG_SET_ALL |
																		 OS_OPT_PEND_NON_BLOCKING,
											(CPU_TS      *)0,
											(OS_ERR      *)&os_err) == EVEN_KEY2_DOWN)
		 {
		 		//等待到后清除标志
			OSFlagPost ((OS_FLAG_GRP  *)&key_flag_grp,                             
										(OS_FLAGS      )EVEN_KEY2_DOWN,
										(OS_OPT        )OS_OPT_POST_FLAG_CLR,
										(OS_ERR       *)&os_err);
			 
			 LED2_TOGGLE;

		 }
					
		//检查电容按键标志
		if(tpad_flag)WM_DeleteWindow(hWin);
		GUI_Delay(10); 				
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
