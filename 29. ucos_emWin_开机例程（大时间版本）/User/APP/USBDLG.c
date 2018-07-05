/**
  ******************************************************************************
  * @file    USBDLG.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   USB应用窗口
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
#include 	"includes.h"
#include 	"hw_config.h" 
#include 	"usb_lib.h"
#include 	"usb_pwr.h"
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

/*--------------------  (使用U2C.exe小工具生成)  ------------------------------------*/
/*
*  U2C.exe小工具使用方法：新建一个TXT文本文件，输入待转换文本，另存为UTF8编码格式文件，
*  打开U2C.exe小工具，先导入文件文件，然后选择"Convert"进行转换，完成后在同路径下输出
*  同名.c文件。
*/
#define UTF8_EXTERN_U     			"\xe5\xa4\x96\xe9\x83\xa8""FLASH\xe6\xa8\xa1\xe6\x8b\x9fU\xe7\x9b\x98" //外部FLASH模拟U盘
#define UTF8_COMPUTER_READ   	"\xe5\x8f\xaf\xe5\xae\x9e\xe7\x8e\xb0\xe5\xaf\xb9\xe5\xa4\x96\xe9\x83\xa8""FLASH\xe7\x9a\x84\xe6\x96\x87\xe4\xbb\xb6\xe8\xaf\xbb\xe5\x86\x99" //可实现对外部FLASH的文件读写
#define UTF8_POWER_SUPLY     		"\xe4\xbd\xbf\xe7\x94\xa8\xe4\xb8\xb2\xe5\x8f\xa3\xe6\x8e\xa5\xe5\x8f\xa3\xe4\xbe\x9b\xe7\x94\xb5" //使用串口接口供电
#define UTF8_PRESS_BUTTON_1    "\xe7\x82\xb9\xe5\x87\xbbUSB\xe5\x9b\xbe\xe6\xa0\x87\xe5\x90\x8e\xef\xbc\x8c" //点击USB图标后,
#define UTF8_PRESS_BUTTON_2    "\xe6\x8a\x8aUSB\xe7\xba\xbf\xe6\x8e\xa5\xe5\x85\xa5USB Devic\xe6\x8e\xa5\xe5\x8f\xa3" //把USB线接入USB Devic接口
#define UTF8_RECONNECT_1      		"\xe8\x8b\xa5\xe7\x94\xb5\xe8\x84\x91\xe6\xa3\x80\xe6\xb5\x8b\xe4\xb8\x8d\xe5\x88\xb0U\xe7\x9b\x98\xef\xbc\x8c"//若电脑检测不到U盘，
#define UTF8_RECONNECT_2      		"\xe8\xaf\xb7\xe9\x87\x8d\xe6\x96\xb0\xe6\x8f\x92\xe6\x8b\x94USB\xe7\xba\xbf" //请重新插拔USB线


// USER START (Optionally insert additional static data)
extern GUI_CONST_STORAGE GUI_BITMAP bmUSBicon;
extern uint32_t *Data_Buffer;
static uint8_t usbflag=0;


// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateUSB[] = {
  { FRAMEWIN_CreateIndirect, "USB",0, 0, 0, 240, 320, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "OPEN", GUI_ID_BUTTON0, 70, 130, 100, 100, 0, 0x0, 0 },
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
static void _cbDialogUSB(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
	static uint8_t showflag = 0;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
	  case WM_DELETE:
		APP_TRACE_DBG(("USBapp delete\n"));
		LED1_OFF;
		ICON_Clicked[6] = 0;

		USB_Cable_Config(DISABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, DISABLE);
		UserApp_Running = 0;
	  WM_InvalidateWindow(WM_HBKWIN);
		tpad_flag=0;
		showflag = 0;
		
			//释放申请的空间
		GUI_ALLOC_Free(hMem_usb);

	break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'USB'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    FRAMEWIN_SetTitleHeight(hItem, 20);
    //
    // Initialization of 'OPEN'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
//    BUTTON_SetBitmapEx(hItem,BUTTON_BI_UNPRESSED,&bmUSBicon,5,5);
		BUTTON_SetFont(hItem,GUI_FONT_16B_ASCII);
    BUTTON_SetText(hItem, "USB");
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case GUI_ID_BUTTON0: // Notifications sent by 'OPEN'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
					usbflag=1;
					showflag = 1;
					WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0));				
					WM_InvalidateWindow(pMsg->hWin);

        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
	case WM_PAINT:	
		GUI_SetBkColor(APPBKCOLOR);
		GUI_SetColor(APPTEXTCOLOR);
		GUI_Clear();
		GUI_DispStringHCenterAt(UTF8_EXTERN_U,115,15);
		GUI_DispStringHCenterAt(UTF8_COMPUTER_READ,115,35);
			
		GUI_SetColor(BLUE);

		GUI_DispStringHCenterAt(UTF8_POWER_SUPLY,115,65);
		GUI_DispStringHCenterAt(UTF8_PRESS_BUTTON_1,115,85);
		GUI_DispStringHCenterAt(UTF8_PRESS_BUTTON_2,115,105);
	
		if(showflag)
		{
			GUI_DispStringHCenterAt(UTF8_RECONNECT_1,122,245);
			GUI_DispStringHCenterAt(UTF8_RECONNECT_2,115,265);
		}
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
*       CreateUSB
*/
void FUN_ICON6Clicked_not_used(void)
{
	WM_HWIN hWin;	

	hWin=GUI_CreateDialogBox(_aDialogCreateUSB, GUI_COUNTOF(_aDialogCreateUSB), _cbDialogUSB, WM_HBKWIN, 0, 0);
	APP_TRACE_DBG(("USBapp create\n"));
	
//  com_data2null(comdata,COMDATA_SIZE);
//	Data_Buffer=(uint32_t *)comdata;

	//从emwin内存中分配USB空间
	hMem_usb = GUI_ALLOC_AllocZero(4096);
	if(hMem_usb == NULL)
		printf("\r\n no enough space\r\n");
	//把空间句柄转成指针
	Data_Buffer = (uint32_t *)GUI_ALLOC_h2p(hMem_usb);
	
	
	while(UserApp_Running)
	{
		if(usbflag)
		{			
			/*初始化*/
			Set_System();
			/*设置USB时钟为48M*/
			Set_USBClock();
			/*配置USB中断*/
			USB_Interrupts_Config();
			/*USB初始化*/
			USB_Init();
			while (bDeviceState != CONFIGURED)
				{		
					GUI_Delay(200);
					if(tpad_flag)
					{
						goto delete_usb_win;
					}

				}	 //等待配置完成
			usbflag=0;
		}
		
		LED1_TOGGLE;
		if(tpad_flag)
		{
delete_usb_win:
			WM_DeleteWindow(hWin);
		}
		GUI_Delay(200);
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
