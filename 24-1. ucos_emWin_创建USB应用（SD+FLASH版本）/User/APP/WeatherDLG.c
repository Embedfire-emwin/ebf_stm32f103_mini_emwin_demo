/**
  ******************************************************************************
  * @file    WeatherDLG.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   天气的应用窗口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 MINI 开发板 
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


typedef enum
{
	TH_ERR=0,
	TH_DS18B20_OK,
	TH_DHT11_OK
}THRESULT;
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
#define UTF8_READ_H_T  								"\xe6\xb8\xa9\xe6\xb9\xbf\xe5\xba\xa6\xe8\xaf\xbb\xe5\x8f\x96" //温湿度读取
#define UTF8_INSERT_SENSOR  					"\xe8\xaf\xb7\xe5\x85\x88\xe5\xb0\x86\xe4\xbc\xa0\xe6\x84\x9f\xe5\x99\xa8\xe6\x8f\x92\xe5\x85\xa5\xe5\x8d\xa1\xe6\xa7\xbd\xe4\xb8\x8a" //请先将传感器插入卡槽上
#define UTF8_INIT_DS18B20_NORMAL  		"\xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96""DS18B20\xe6\xad\xa3\xe5\xb8\xb8" //初始化DS18B20正常
#define UTF8_TEMPERATUER  						"\xe6\xb8\xa9\xe5\xba\xa6:       \xe5\xba\xa6" //温度:      度
#define UTF8_INIT_DS18B20_ABNORMAL  "\xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96""DS18B20\xe5\xbc\x82\xe5\xb8\xb8" //初始化DS18B20异常
#define UTF8_INIT_DHT11_NORMAL  			"\xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96""DHT11\xe6\xad\xa3\xe5\xb8\xb8" //初始化DHT11正常
#define UTF8_HUMIDITY  								"\xe6\xb9\xbf\xe5\xba\xa6:      RH" //湿度:      RH
#define UTF8_INIT_DHT11_ABNORMAL  		"\xe5\x88\x9d\xe5\xa7\x8b\xe5\x8c\x96""DHT11\xe5\xbc\x82\xe5\xb8\xb8" //初始化DHT11异常

// USER START (Optionally insert additional static data)
static DHT11_Data_TypeDef DHT11_Data;
static float temp=0.0;
static THRESULT TH_res=TH_ERR;
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogWeather[] = {
  { FRAMEWIN_CreateIndirect, "Humiture",0, 0, 0, 240, 320, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Data1",  GUI_ID_TEXT0, 100, 83,  60, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Data2",  GUI_ID_TEXT1, 100, 163, 60, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Data3",  GUI_ID_TEXT2, 100, 193, 60, 30, 0, 0x64, 0 },
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
static void _cbDialogWeather(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId){
	case WM_DELETE:
		APP_TRACE_DBG(("Weather app delete\n"));
		ICON_Clicked[2] = 0;
		UserApp_Running = 0;
		WM_InvalidateWindow(WM_HBKWIN);
		tpad_flag=0;
		break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Humiture'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTitleHeight(hItem, 20);
		FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
		FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    //
    // Initialization of 'Data1'
    //
    hItem = WM_GetDialogItem(pMsg->hWin,  GUI_ID_TEXT0);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetText(hItem, "");
    TEXT_SetTextColor(hItem,GUI_BLUE);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    //
    // Initialization of 'Data2'
    //
    hItem = WM_GetDialogItem(pMsg->hWin,  GUI_ID_TEXT1);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetText(hItem, "");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem,GUI_BLUE);
		//
    // Initialization of 'Data3'
    //
    hItem = WM_GetDialogItem(pMsg->hWin,  GUI_ID_TEXT2);
    TEXT_SetFont(hItem, GUI_FONT_24B_ASCII);
    TEXT_SetText(hItem, "");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
    TEXT_SetTextColor(hItem,GUI_BLUE);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
		// USER START (Optionally insert additional message handling)
  case WM_PAINT:	
		GUI_SetBkColor(APPBKCOLOR);
		GUI_SetColor(APPTEXTCOLOR);
		GUI_Clear();
	
		GUI_DispStringHCenterAt(UTF8_READ_H_T,115,10);
		GUI_DispStringHCenterAt(UTF8_INSERT_SENSOR,115,30);
	
		GUI_SetColor(GUI_DARKMAGENTA);
		if(TH_res==TH_DS18B20_OK)
		{
			GUI_DispStringHCenterAt(UTF8_INIT_DS18B20_NORMAL,115,60);
			GUI_DispStringAt(UTF8_TEMPERATUER,60,90);
		}
		else 
			GUI_DispStringHCenterAt(UTF8_INIT_DS18B20_ABNORMAL,115,60);
		
	
	  if(TH_res==TH_DHT11_OK)
		{
			GUI_DispStringHCenterAt(UTF8_INIT_DHT11_NORMAL,115,140);
			GUI_DispStringAt(UTF8_TEMPERATUER,60,170);
			GUI_DispStringAt(UTF8_HUMIDITY,60,200);
		}
		else 
			GUI_DispStringHCenterAt(UTF8_INIT_DHT11_ABNORMAL,115,140);
	
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
*       CreateHumiture
*/
void FUN_ICON2Clicked(void) 
{
	WM_HWIN hWin;
	uint8_t i=0;
	OS_ERR      err;
	char text1[6]={0},text2[6]={0};
	APP_TRACE_DBG(("Weather app create\n"));
	
	TH_res=TH_ERR;
	
	if(DS18B20_Init()==0) 	
	{
		TH_res=TH_DS18B20_OK;							/*----  检测到DS18B20 -----*/
		temp=DS18B20_GetTemp_SkipRom();	
	}
	else 
	{
		DHT11_Init();
		if(Read_DHT11(&DHT11_Data)==1)				 					
		{
			TH_res=TH_DHT11_OK;									/*----  检测到DHT11 -----*/
		}
	}
		
	
	hWin = GUI_CreateDialogBox(_aDialogWeather, 
																GUI_COUNTOF(_aDialogWeather), 
																_cbDialogWeather, 
																WM_HBKWIN, 
																0, 
																0);
	
	while(UserApp_Running)
	{
		if(TH_res!=TH_ERR)i++;
		if(i>=120)
		{
			OSSchedLock(&err);
			/*调用Read_DHT11读取温湿度，若成功则输出该信息*/
			if(TH_res==TH_DHT11_OK)
			{
				Read_DHT11(&DHT11_Data);
				sprintf(text1,"%d.%d",DHT11_Data.temp_int,DHT11_Data.temp_deci);
				TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1),text1);
				sprintf(text2,"%d.%d",DHT11_Data.humi_int,DHT11_Data.humi_deci);
				TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2),text2);
			}
			else if(TH_res==TH_DS18B20_OK)
			{
				temp=DS18B20_GetTemp_SkipRom();
				sprintf(text1,"%0.2f",temp);
				TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0),text1);
			}
			OSSchedUnlock(&err);
			i=0;
		}
		if(tpad_flag)WM_DeleteWindow(hWin);
		GUI_Delay(10);
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
