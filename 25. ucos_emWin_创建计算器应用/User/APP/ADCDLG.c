/**
  ******************************************************************************
  * @file    ADCDLG.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ADC的应用窗口
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

/*--------------------  (使用U2C.exe小工具生成)  ------------------------------------*/
/*
*  U2C.exe小工具使用方法：新建一个TXT文本文件，输入待转换文本，另存为UTF8编码格式文件，
*  打开U2C.exe小工具，先导入文件文件，然后选择"Convert"进行转换，完成后在同路径下输出
*  同名.c文件。
*/
#define UTF8_READ_ADC     "\xe7\x94\xb5\xe4\xbd\x8d\xe5\x99\xa8\xe7\x94\xb5\xe5\x8e\x8b\xe8\xaf\xbb\xe5\x8f\x96"  //电位器电压读取

// USER START (Optionally insert additional static data)
//存储adc转换结果的变量
extern __IO uint16_t ADC_ConvertedValue;

// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateADC[] = {
  { FRAMEWIN_CreateIndirect, "ADC Converte", 0, 0, 0, 240, 320, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "ADC DATA", GUI_ID_TEXT0, 70, 30, 100, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "DATA", GUI_ID_TEXT1, 30, 50, 180, 20, 0, 0x64, 0 },
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
*       _UserDraw
*
* Function description
*   This routine is called by the GRAPH object before anything is drawn
*   and after the last drawing operation.
*/
static void _UserDraw(WM_HWIN hWin, int Stage) {
  if (Stage == GRAPH_DRAW_LAST) 
  {
    char acText[] = "ADCConvertedValue (V)";
    GUI_RECT Rect;
    GUI_RECT RectInvalid;
    int FontSizeY;

    GUI_SetFont(&GUI_Font13_ASCII);
    FontSizeY = GUI_GetFontSizeY();
    WM_GetInsideRect(&Rect);
    WM_GetInvalidRect(hWin, &RectInvalid);
    Rect.x1 = Rect.x0 + FontSizeY;
    GUI_SetColor(GUI_YELLOW);
    GUI_DispStringInRectEx(acText, &Rect, GUI_TA_HCENTER, strlen(acText), GUI_ROTATE_CCW);
  }
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogADC(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  // USER START (Optionally insert additional variables)
  // USER END
  switch (pMsg->MsgId) {
  case WM_DELETE:
		APP_TRACE_DBG(("ADCapp delete\n"));
		ICON_Clicked[1] = 0;
		//停止ADC
		stopADC();
	
		UserApp_Running = 0;
		WM_DeleteWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_GRAPH0));
		tpad_flag=0;
		//恢复蜂鸣器
		Beep_Init();
	break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'ADC TEST'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTitleHeight(hItem, 20);
		FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
		FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    //
    // Initialization of 'ADC DATA'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    //
    // Initialization of 'DATA'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetFont(hItem, GUI_FONT_16B_ASCII);
    TEXT_SetText(hItem, "*********");
		TEXT_SetBkColor(hItem,GUI_WHITE);
    TEXT_SetTextColor(hItem, GUI_BLUE);
    // USER START (Optionally insert additional code for further widget initialization)
		/* enable adc1 and config adc1 to dma mode */
		ADCx_Init();
    // USER END
    break;
		// USER START (Optionally insert additional message handling)
  case WM_PAINT:	
		GUI_SetBkColor(APPBKCOLOR);
		GUI_SetColor(APPTEXTCOLOR);
		GUI_Clear();
		GUI_DispStringHCenterAt(UTF8_READ_ADC,115,10);
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
*Program Size: Code=134032 RO-data=67916 RW-data=2608 ZI-data=45824  
*       CreateADC TEST
*/
void FUN_ICON1Clicked(void)
{
	WM_HWIN 			hWin;
	WM_HWIN      hGraph;
	GRAPH_DATA_Handle  	hData;

	GRAPH_SCALE_Handle 	hScaleVert,hScaleH;
	uint8_t i=0;  
	char pValue[25]={0};
	
	
	//存储ADC计算后的浮点型数据
	float data;
	
	APP_TRACE_DBG(("ADCapp create\n"));
	
	hWin=GUI_CreateDialogBox(_aDialogCreateADC, GUI_COUNTOF(_aDialogCreateADC), _cbDialogADC, WM_HBKWIN, 0, 0);
	
	hGraph      = GRAPH_CreateEx (5, 80, 220, 200, WM_GetClientWindow(hWin), WM_CF_SHOW , 0, GUI_ID_GRAPH0);
	hData       = GRAPH_DATA_YT_Create(GUI_RED,  /* 绘制数据要使用的颜色 */
																				 250,  /* 数据项的最大数 */
																				 0,  /* 指向要添加到对象的数据的指针。该指针应指向一组I16值。*/
																				 0); /* 要添加的数据项数。*/
										
	hScaleVert  = GRAPH_SCALE_Create(25,                      /* 相对于图形小工具的左边/顶边的位置。 */        
																			 GUI_TA_RIGHT,            /* 用于绘制编号的文本对齐方式 */
																			 GRAPH_SCALE_CF_VERTICAL, /* 创建垂直刻度对象 */ 
																			 50);

	hScaleH = GRAPH_SCALE_Create(190, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);
    
	GRAPH_SCALE_SetTextColor(hScaleH, GUI_YELLOW);

	WIDGET_SetEffect(hGraph, &WIDGET_Effect_Simple);
						   
	/* 设置边框的尺寸 （上、下、左、右） */
	GRAPH_SetBorder (hGraph,   /* 句柄 */
											0,   /* 左边框的尺寸 */
											0,   /* 上边框的尺寸 */
											0,   /* 右边框的尺寸 */
											0); /* 下边框的尺寸 */
								
	/* 设置用户绘制函数。此函数由小工具在绘制过程中调用，使应用程序能够绘制用户定义的数据 */							
	GRAPH_SetUserDraw     (hGraph, _UserDraw);

	 GRAPH_SetGridDistY(hGraph, 50);
	 GRAPH_SetGridVis(hGraph, 1);
	 GRAPH_SetGridFixedX(hGraph, 1);
	/* 设置数据的对齐方式 左对齐或者右对齐 */
	GRAPH_DATA_YT_SetAlign(hData,  GRAPH_ALIGN_RIGHT);

	/* 
		置用于计算要绘制的编号的因子
	    1. 不使用因子时，刻度对象的单位为 “像素”。因此给定因子应将像素值转换为所需的单位
	*/
	GRAPH_SCALE_SetFactor   (hScaleVert, 0.02f);
	
	/*
		设置用于在正或负方向上 “移动”刻度对象的偏移。
		1. 水平刻度对象从数据区的底边开始向顶部进行标记，垂直刻度对象从左边 （水平刻度）开始向右边
		   进行标记，其中第一个位置是零点。在许多情况下，不希望第一个位置是零点。如果刻度要向正方
		   向 “移动”，应添加正偏移，要向负方向 “移动”，应添加负值。
	*/
	/* 设置用于绘制编号的文本颜色。*/
	GRAPH_SCALE_SetTextColor(hScaleVert, GUI_YELLOW);
	
	/* 将数据对象附加到现有图形小工具。 */
	GRAPH_AttachData (hGraph, hData);
	/* 将刻度对象附加到现有图形小工具。 */
	GRAPH_AttachScale(hGraph, hScaleVert);
	GRAPH_AttachScale(hGraph, hScaleH);
	
	while(UserApp_Running)
	{
		i++;
		if(i>=50)
		{
			data=(float)ADC_ConvertedValue;
			//printf("\r\n1 The current AD value = %f  \r\n", data);
			data=data*3.3/4096;		
			//printf("\r\nThe current AD value = 0x%04X -> %f\r\n", ADC_ConvertedValue,data); 
			sprintf(pValue,"0x%04X --> %fV",ADC_ConvertedValue,data);
			pValue[24]='\0';
			
			TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1), pValue);
			GRAPH_DATA_YT_AddValue(hData,data*50);
			i=0;
		}
		if(tpad_flag)WM_DeleteWindow(hWin);
		GUI_Delay(10); 
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
