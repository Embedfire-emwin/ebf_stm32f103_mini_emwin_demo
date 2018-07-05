/**
  ******************************************************************************
  * @file    ClockDLG.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   时钟的应用窗口
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
extern int8_t rtc_flag;
extern char TimeDisplay;
static	char text_buffer[30];
static 	struct rtc_time SrcTm;
/*时间结构体*/
extern struct rtc_time systmtime;

// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateClock[] = {
  { FRAMEWIN_CreateIndirect, "Clock", 0, 0, 0, 240, 320, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Time", GUI_ID_TEXT0, 20, 210, 100, 40, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "wday", GUI_ID_TEXT1, 120, 205, 100, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Data", GUI_ID_TEXT2, 120, 225, 100, 20, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Time Setting", GUI_ID_TEXT3, 0, 0, 110, 20, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Ok", GUI_ID_BUTTON0, 40, 140, 150, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "TIME", GUI_ID_TEXT4, 0, 20, 210, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "Year", GUI_ID_EDIT0, 20, 40, 50, 25, 0, 0x4, 0 },
  { EDIT_CreateIndirect, "Month", GUI_ID_EDIT1, 87, 40, 50, 25, 0, 0x2, 0 },
  { EDIT_CreateIndirect, "Day", GUI_ID_EDIT2, 155, 40, 50, 25, 0, 0x2, 0 },
  { TEXT_CreateIndirect, "Hour", GUI_ID_TEXT5, 0, 70, 210, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "HOUR", GUI_ID_EDIT3, 20, 90, 50, 25, 0, 0x2, 0 },
  { EDIT_CreateIndirect, "Minute", GUI_ID_EDIT4, 87, 90, 50, 25, 0, 0x2, 0 },
  { EDIT_CreateIndirect, "Edit", GUI_ID_EDIT5, 155, 90, 50, 25, 0, 0x2, 0 },
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
static void _cbDialogClock(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END
  switch (pMsg->MsgId) {
	case WM_TIMER:
		//printf("Clockapp:MY_MESSAGE_RTC\n");

		/* 转换rtc值至北京时间*/
		RTC_TimeCovr(&systmtime);
		/* 转换成字符串 */
		sprintf(text_buffer,"%02d:%02d:%02d",systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
		//printf("time:%s\n",text_buffer);
		/* 获取text句柄 */
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);//00:00:00
		/* 输出时间 */
		TEXT_SetText(hItem,text_buffer);	
		/* 转换成字符串 */
		sprintf(text_buffer,"%04d.%02d.%02d",systmtime.tm_year,systmtime.tm_mon,systmtime.tm_mday);		
		/* 获取text句柄 */
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2);//2015.04.28		
		/* 输出时间 */
		TEXT_SetText(hItem,text_buffer);		
		/* 获取text句柄 */
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);//Wednesday		
		/* 输出星期 */
		switch(systmtime.tm_wday)
		{
			case 1:
				/* 输出时间 */
				TEXT_SetText(hItem,"Monday");				
			break;		
			case 2:
				/* 输出时间 */
				TEXT_SetText(hItem,"Tuesday");				
			break;								
			case 3:
				/* 输出时间 */
				TEXT_SetText(hItem,"Wednesday");				
			break;											
			case 4:
				/* 输出时间 */
				TEXT_SetText(hItem,"Thursday");				
			break;			
			case 5:
				/* 输出时间 */
				TEXT_SetText(hItem,"Friday");				
			break;														
			case 6:
				/* 输出时间 */
				TEXT_SetText(hItem,"Saturday");				
			break;			
			case 7:
				/* 输出时间 */
				TEXT_SetText(hItem,"Sunday");				
			break;		
			default:
			  break;
		}
	WM_RestartTimer(pMsg->Data.v, 500);
	break;
  case WM_DELETE:
		APP_TRACE_DBG(("Clockapp delete\n"));
		ICON_Clicked[3] = 0;
		UserApp_Running = 0;
		keypadflag=0;
		WM_DeleteWindow(KEYBOARD);
		tpad_flag=0;
	break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Clock'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    FRAMEWIN_SetTitleHeight(hItem, 20);
    //
    // Initialization of 'Time'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
    TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "");
    //
    // Initialization of 'wday'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "");
    //
    // Initialization of 'Data'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2);
    TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
    TEXT_SetText(hItem, "");
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'Time Setting'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_20B_ASCII);
    TEXT_SetText(hItem, "Time Setting:");
    //
    // Initialization of 'Ok'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
    BUTTON_SetText(hItem, "Setting RTC");
		BUTTON_SetFocussable(hItem, 0);
    //
    // Initialization of 'TIME'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT4);
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "        Year        Month        Day");
    //
    // Initialization of 'Year'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0);
    EDIT_SetText(hItem, "2015");
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		EDIT_SetUlongMode(hItem,2015,1970,2038);
    //
    // Initialization of 'Month'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1);
    EDIT_SetText(hItem, "04");
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		EDIT_SetUlongMode(hItem,04,1,12);
    //
    // Initialization of 'Day'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT2);
    EDIT_SetText(hItem, "28");
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		EDIT_SetUlongMode(hItem,28,1,31);
    //
    // Initialization of 'Hour'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT5);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "        Hour       Minute      Second");
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    //
    // Initialization of 'HOUR'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT3);
    EDIT_SetText(hItem, "11");
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		EDIT_SetUlongMode(hItem,11,0,23);
    //
    // Initialization of 'Minute'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT4);
    EDIT_SetText(hItem, "55");
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		EDIT_SetUlongMode(hItem,55,0,59);
    //
    // Initialization of 'Edit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT5);
    EDIT_SetText(hItem, "55");
    EDIT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    EDIT_SetFont(hItem, GUI_FONT_20B_ASCII);
		EDIT_SetUlongMode(hItem,55,0,59);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case GUI_ID_BUTTON0: // Notifications sent by 'Ok'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				SrcTm.tm_year  = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0));
				SrcTm.tm_mon  = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1));
				SrcTm.tm_mday = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT2));
				SrcTm.tm_hour = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT3));
				SrcTm.tm_min = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT4));
				SrcTm.tm_sec = EDIT_GetValue(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT5));
				
				Time_Adjust(&SrcTm);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_EDIT0: // Notifications sent by 'Year'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				if(!keypadflag) keypadapp(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0),1);
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_EDIT1: // Notifications sent by 'Month'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				// USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				if(!keypadflag) keypadapp(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT1),1);
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_EDIT2: // Notifications sent by 'Day'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)				
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				if(!keypadflag) keypadapp(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT2),1);
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_EDIT3: // Notifications sent by 'HOUR'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)				
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				if(!keypadflag) keypadapp(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT3),1);
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_EDIT4: // Notifications sent by 'Minute'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)				
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				if(!keypadflag) keypadapp(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT4),1);
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case GUI_ID_EDIT5: // Notifications sent by 'Edit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)				
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
				if(!keypadflag) keypadapp(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT5),1);        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
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
	case WM_PAINT:                                            //重绘背景	
    GUI_SetBkColor(GUI_LIGHTYELLOW);
		GUI_SetColor(0XD0FFFF);
		GUI_Clear();
		GUI_FillRect(0,0,240-1,190);
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
*       CreateClock
*/
void FUN_ICON3Clicked(void)
{
	WM_HWIN hWin;
	APP_TRACE_DBG(("Clockapp create\n"));
	hWin = GUI_CreateDialogBox(_aDialogCreateClock, GUI_COUNTOF(_aDialogCreateClock), _cbDialogClock, WM_HBKWIN, 0, 0);
	/* 
	 * 创建定时器，其功能是经过指定周期后，向指定窗口发送消息。
	 * 该定时器与指定窗口相关联。 
	 */
	WM_CreateTimer(WM_GetClientWindow(hWin),  /* 接受信息的窗口的句柄 */
	               0, 	         /* 用户定义的Id。如果不对同一窗口使用多个定时器，此值可以设置为零。 */
								 500,         /* 周期，此周期过后指定窗口应收到消息*/
								 0);	         /* 留待将来使用，应为0 */
	GUI_Delay(10); 
	if(rtc_flag!=0)
		rtc_flag=RTC_CheckAndConfig(&systmtime);
	if(rtc_flag!=0)
		GUI_MessageBox("\r\n RTC Work Down!!! \r\n","error",GUI_MESSAGEBOX_CF_MOVEABLE);
	else
		WM_EnableWindow(WM_GetDialogItem(hWin, GUI_ID_BUTTON0));
	while(UserApp_Running)
	{
		if(tpad_flag)WM_DeleteWindow(hWin);
		GUI_Delay(10); 
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
