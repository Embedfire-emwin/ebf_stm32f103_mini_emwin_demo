/**
  ******************************************************************************
  * @file    MessageDLG.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   短信的应用窗口
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
#include "MESSAGEBOX.h"
// USER END

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
// USER START (Optionally insert additional defines)
#define _DF1S	0x81
#define MESS_LIST_PATH 		"1:/System/messagelist.txt"
#define MESSAGE_MAX			30
// USER END


/*--------------------  (使用U2C.exe小工具生成)  ------------------------------------*/
#define UTF8_MESSAGEDLG        "\xe7\x9f\xad\xe4\xbf\xa1"//短信
#define UTF8_STATE             "\xe7\x8a\xb6\xe6\x80\x81"//状态
#define UTF8_CONTACT           "\xe8\x81\x94\xe7\xb3\xbb\xe4\xba\xba"//联系人
#define UTF8_CONTENT           "\xe5\x86\x85\xe5\xae\xb9"//内容
#define UTF8_SEND              "\xe5\x8f\x91\xe9\x80\x81"//发送
#define UTF8_UNREAD            "\xe6\x9c\xaa\xe8\xaf\xbb"//未读
#define UTF8_TO                "\xe5\x8f\x91"//发
#define UTF8_FROM              "\xe6\x94\xb6"//收
#define UTF8_CLEAR             "\xe6\xb8\x85\xe9\x99\xa4"//清除


/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
// USER START (Optionally insert additional static data)
static WM_HWIN hContacts,hSend,hCancel,hMessagelist,hContent,hText;
static uint8_t  	messageindex=0;
static uint8_t		messageNumRows=0;
static char pStat[5]   	 ={0};
static char pContacts[20]={0};
static char pContentlist[20]={0};
static char *pContent;
static const char * _aTable_[1][3] = {0};

extern FRESULT f_result; 
extern FIL     f_file;
extern UINT    f_num;

// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateMessage[] = {
  { FRAMEWIN_CreateIndirect, "Message", 0, 0, 0, 240, 320, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "MESSAGEList", GUI_ID_LISTVIEW0, 0, 0, 230, 295, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "To", GUI_ID_TEXT0, 0, 0, 60, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect, "contact", GUI_ID_EDIT0, 60, 0, 160, 20, 0, 0x1e, 0 },
  { BUTTON_CreateIndirect, "send", GUI_ID_BUTTON0, 40, 265, 60, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "cancel", GUI_ID_BUTTON1, 130, 265, 60, 25, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "edit", GUI_ID_MULTIEDIT0, 0, 22, 227, 238, 0, 0x0, 0 },
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
static void gbk2utf8(const char *src, char *str,uint32_t maxnum)
{
	uint32_t j=0,k=0;
	uint16_t gbkdata=0;
	uint16_t UCbuffer[200]={0};
	for(j=0,k=0;(j<maxnum)&&src[j]!='\0';k++)
	{
		if((uint8_t)src[j]>0x80)
		{
			gbkdata=src[j+1]+src[j]*256;
			UCbuffer[k]=ff_convert(gbkdata,1);
			j+=2;
		}
		else
		{
			UCbuffer[k]=0x00ff&src[j];
			j+=1;
		}
	}
	UCbuffer[k]='\0';
	GUI_UC_ConvertUC2UTF8(UCbuffer,2*k+2,str,k*3);
}

static void LISTVIEW_AddItem(void)
{
	uint8_t i=0;
	messageNumRows=0;
	//result = f_unlink(MESS_LIST_PATH);//删除文件
	//result = f_open (&file, MESS_LIST_PATH,FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
	f_result = f_open (&f_file, MESS_LIST_PATH,FA_READ);
	if(f_result != FR_OK)
	{
		sprintf(pStat,"%s","To");
		sprintf(pContacts,"%s","new");
		sprintf(pContentlist,"%s","...");
		sprintf(pContent,"%-5s%-20s%s",pStat,pContacts,pContentlist);
		_aTable_[0][0]=pStat;
		_aTable_[0][1]=pContacts;
		_aTable_[0][2]=pContentlist;
		LISTVIEW_AddRow(hMessagelist, _aTable_[0]);
		messageNumRows=1;
		f_close (&f_file);
		return ;
	}
	while(f_result == FR_OK && messageNumRows<MESSAGE_MAX)
	{
		f_result = f_lseek(&f_file,messageNumRows*325);
		if(f_result != FR_OK)break;
		//result = f_write(&f_file,pContent, 325, &f_num);
		f_result = f_read(&f_file,pContent, 325, &f_num);
		if(f_num < 25)break;
		memcpy(pStat,pContent,sizeof(char)*5);
		i=0;
		while(i<5)
		{
			if(pStat[i]==0x20)
			{
				pStat[i]='\0';
				break;
			}
			i++;
		}
		memcpy(pContacts,&pContent[5],sizeof(char)*20);
		i=0;
		while(i<20)
		{
			if(pContacts[i]==0x20)
			{
				pContacts[i]='\0';
				break;
			}
			i++;
		}
		memcpy(pContentlist,&pContent[25],sizeof(char)*20);
		gbk2utf8(pContentlist,pContentlist,20);
//		printf("message:%d ::%s::%s::%s\r\n",messageNumRows,pStat,pContacts,pContentlist);
		_aTable_[0][0]=pStat;
		_aTable_[0][1]=pContacts;
		_aTable_[0][2]=pContentlist;
		LISTVIEW_AddRow(hMessagelist, _aTable_[0]);
		messageNumRows++;
	}
	f_close (&f_file);
	sprintf(pStat,"%s","To");
	sprintf(pContacts,"%s","new");
	sprintf(pContentlist,"%s","...");
	sprintf(pContent,"%-5s%-20s%s",pStat,pContacts,pContentlist);
	_aTable_[0][0]=pStat;
	_aTable_[0][1]=pContacts;
	_aTable_[0][2]=pContentlist;
	LISTVIEW_AddRow(hMessagelist, _aTable_[0]);
	messageNumRows++;
}

static void EditMessage(uint8_t index)
{
	uint16_t i=0;
	OS_ERR      err;
	char contentedit[300]={0};
	f_result = f_open(&f_file, MESS_LIST_PATH,FA_READ);
	if(f_result != FR_OK)
	{
		f_close (&f_file);
		return ;
	}
	OSSchedLock(&err);
	f_result = f_lseek(&f_file,index*325);
	f_result = f_read(&f_file,pContent, 325, &f_num);
	if(f_result == FR_OK)
	{
		memcpy(pContacts,&pContent[5],sizeof(char)*20);
		i=0;
		while(i<20)
		{
			if(pContacts[i]==0x20)
			{
				pContacts[i]='\0';
				break;
			}
			i++;
		}
		memcpy(contentedit,&pContent[25],sizeof(char)*300);
		gbk2utf8(contentedit,pContent,300);		
	}
	else
	{
		pContacts[0]='\0';
		pContent[0]='\0';
	}
	OSSchedUnlock(&err);
	f_close (&f_file);
}

static void SaveMessage(uint8_t index)
{
	OS_ERR      err;

	char contentedit[300]={0};
	uint16_t UCdata[150]={0},gbkdata=0;
	uint16_t i=0,j=0,length=0;
	i=0;
	while(i<325)
	{
		pContent[i]='\0';
		++i;
	}
	pContent[324]='\n';
	length=MULTIEDIT_GetTextSize(hContent);
	//if(MULTIEDIT_GetTextSize(hContent)==0)return;
	sprintf(pStat,"%s","To");
	EDIT_GetText(hContacts,pContacts,20);

	f_result=f_open(&f_file,MESS_LIST_PATH,FA_WRITE);
	if(f_result==FR_NO_FILE)
	{
		f_result=f_open(&f_file,MESS_LIST_PATH,FA_CREATE_NEW|FA_WRITE);
		messageindex=0;
		index=0;
	}
	f_result = f_lseek(&f_file,index*325);
	
	OSSchedLock(&err);
	MULTIEDIT_GetText(hContent,contentedit,length);
	GUI_UC_ConvertUTF82UC(contentedit,length,UCdata,length);
	i=0;
	j=0;
	while(UCdata[i]!='\0'&&j<length)
	{
		if(UCdata[i]>0x80)
		{
			gbkdata=ff_convert(UCdata[i],0);
			contentedit[j]=(char)(gbkdata>>8);
			contentedit[j+1]=(char)gbkdata;
			j+=2;
		}
		else
		{
			contentedit[j]=(char)UCdata[i];
			j+=1;
		}
		i++;
	}
	contentedit[j]='\0';
	sprintf(pContent,"%-5s%-20s%s",pStat,pContacts,contentedit);
	f_result = f_write(&f_file, pContent, 325, &f_num);
	OSSchedUnlock(&err);
	f_close(&f_file);  
}

static void SendMessage(void)
{
	uint16_t MessageLen;
	char contentedit[300]={0};
	MessageLen=MULTIEDIT_GetTextSize(hContent);
	EDIT_GetText(hContacts,pContacts,20);
	MULTIEDIT_GetText(hContent,contentedit,MessageLen);	
	gsm_sms_utf8(pContacts,contentedit,strlen(pContacts),MessageLen);
}

static void saveReceiveMessage(char *num,char *gbkstr)
{
	OS_ERR      err;
	uint16_t i=0;
	i=0;
	while(i<325)
	{
		pContent[i]='\0';
		++i;
	}
	pContent[324]='\n';
	sprintf(pStat,"%s","From");
	f_result=f_open(&f_file,MESS_LIST_PATH,FA_OPEN_EXISTING|FA_WRITE);
	if(f_result==FR_NO_FILE)
	{
		f_result=f_open(&f_file,MESS_LIST_PATH,FA_CREATE_NEW|FA_WRITE);
		messageNumRows=0;
	}
	f_result = f_lseek(&f_file,(messageNumRows-1)*325);
	messageNumRows++;
	OSSchedLock(&err);
	sprintf(pContent,"%-5s%-20s%s",pStat,num,gbkstr);
	f_result = f_write(&f_file, pContent, 325, &f_num);
	OSSchedUnlock(&err);
	f_close(&f_file);
	i=messageNumRows;
	while(i)
	{
		LISTVIEW_DeleteRow(hMessagelist,i-1);
		i--;
	}
	LISTVIEW_AddItem();
	GSM_CLEAN_RX();//清除接收缓存	
}	

// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogMessage(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
	uint8_t i=0;
  // USER END

  switch (pMsg->MsgId) {
	case WM_DELETE:
		APP_TRACE_DBG(("Messageapp delete\n"));
		ICON_Clicked[5] = 0;
		GSM_USART_APBxClkCmd(GSM_USART_CLK, DISABLE);
		USART_ITConfig(GSM_USARTx, USART_IT_RXNE, DISABLE);	
		USART_Cmd(GSM_USARTx, DISABLE);
		pContacts[0]='\0';
		pContent[0]='\0';
		messageindex=0;
		messageNumRows=0;
		UserApp_Running = 0;
		keypadflag=0;
		WM_DeleteWindow(KEYBOARD);
		tpad_flag=0;
	
				//释放申请的空间
		GUI_ALLOC_Free(hMem_usb);

	break;
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Message'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetTextColor(hItem,GUI_DARKGRAY);
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_ASCII);
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
	FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
    FRAMEWIN_SetTitleHeight(hItem, 20);
	//
	// Initialization of 'TO'
	//
	hText = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
	TEXT_SetTextColor(hText, GUI_BLUE);
	TEXT_SetTextAlign(hText, GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetFont(hText, &FONT_XINSONGTI_16);
	TEXT_SetText(hText, UTF8_CONTACT);
	WM_HideWindow(hText);
	//
	// Initialization of 'contacts'
	//
	hContacts = WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0);
	EDIT_SetFont(hContacts, GUI_FONT_16B_ASCII);
	EDIT_EnableBlink(hContacts,500,1);
	WM_HideWindow(hContacts);
	//
	// Initialization of 'send'
	//
	hSend = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);		
	BUTTON_SetText(hSend,"Send");
	BUTTON_SetFont(hSend, GUI_FONT_16B_ASCII);
	WM_HideWindow(hSend);
	//
	// Initialization of 'cancel'
	//
	hCancel = WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
	BUTTON_SetText(hCancel,"Cannel");
	BUTTON_SetFont(hCancel, GUI_FONT_16B_ASCII);
	WM_HideWindow(hCancel);
	//
	// Initialization of 'Content'
	//
	hContent = WM_GetDialogItem(pMsg->hWin, GUI_ID_MULTIEDIT0);
	MULTIEDIT_SetFont(hContent,&FONT_XINSONGTI_16);
	MULTIEDIT_EnableBlink(hContent,500,1);
	MULTIEDIT_SetAutoScrollV(hContent,1);
	MULTIEDIT_SetInsertMode(hContent,1);
	//MULTIEDIT_SetReadOnly(hedit,1);
	MULTIEDIT_SetWrapWord(hContent);
	WM_HideWindow(hContent);
	//
	// Initialization of 'MESSAGEList'
	//
	hMessagelist = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
	//LISTVIEW_SetDefaultFont(GUI_FONT_16B_ASCII);
	LISTVIEW_AddColumn(hMessagelist, 40, "Stats", GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_AddColumn(hMessagelist, 100, "Contacts", GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_AddColumn(hMessagelist, 90, "Content", GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_SetGridVis(hMessagelist, 1);
	LISTVIEW_SetFont(hMessagelist, &FONT_XINSONGTI_16);
	LISTVIEW_SetHeaderHeight(hMessagelist, 25);
	LISTVIEW_SetAutoScrollV(hMessagelist, 1);
	LISTVIEW_SetRowHeight(hMessagelist, 25);
	LISTVIEW_SetTextAlign(hMessagelist,0,GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_SetTextAlign(hMessagelist,1,GUI_TA_HCENTER | GUI_TA_VCENTER);
	LISTVIEW_SetTextAlign(hMessagelist,2,GUI_TA_LEFT | GUI_TA_VCENTER);
    // USER START (Optionally insert additional code for further widget initialization)
	LISTVIEW_AddItem();	
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case GUI_ID_LISTVIEW0: // Notifications sent by 'MESSAGEList'
		  switch(NCode) 
		  {
			  case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				//printf("sel:%d\n",messageindex);		  
				// USER END
				break;
			  case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				messageindex=LISTVIEW_GetSel(hMessagelist);
				if(messageindex==messageNumRows-1)
				{
					sprintf(pContacts,"%s","112");
					sprintf(pContent,"%s","Hellow World");
				}
				else
				{
					EditMessage(messageindex);
				}
				WM_HasFocus(hContacts);
				EDIT_SetText(hContacts,pContacts);
				WM_HasFocus(hContent);
				MULTIEDIT_AddText(hContent,pContent);
				WM_ShowWindow(hContent);
				WM_ShowWindow(hCancel);
				WM_ShowWindow(hSend);
				WM_ShowWindow(hText);
				WM_ShowWindow(hContacts);	
				WM_HideWindow(hMessagelist);
				WM_DisableWindow(hMessagelist);
				// USER END
				break;
			  case WM_NOTIFICATION_SEL_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			  // USER START (Optionally insert additional code for further notification handling)
			  // USER END
		  }
      break;
	  case GUI_ID_EDIT0: // Notifications sent by 'name'
		switch(NCode) 
		{
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				if(!keypadflag) keypadapp(WM_GetDialogItem(pMsg->hWin, GUI_ID_EDIT0),1);
				// USER END
			break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
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
	  case GUI_ID_MULTIEDIT0: // Notifications sent by 'content'
		switch(NCode) 
		{
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
			if(!keypadflag) keypadapp(hContent,0);			
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
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
	  case GUI_ID_BUTTON0: // Notifications sent by 'send'
		switch(NCode) 
		{
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
			SendMessage();
			SaveMessage(messageindex);
			pContacts[0]='\0';
			pContent[0]='\0';
			MULTIEDIT_SetText(hContent,0);
			WM_HideWindow(hContent);
			WM_HideWindow(hCancel);
			WM_HideWindow(hSend);
			WM_HideWindow(hText);
			WM_HideWindow(hContacts);
			WM_ShowWindow(hMessagelist);
			WM_EnableWindow(hMessagelist);
			
			i=messageNumRows;
			while(i)
			{
				LISTVIEW_DeleteRow(hMessagelist,i-1);
				i--;
			}
			LISTVIEW_AddItem();
			GSM_CLEAN_RX();//清除接收缓存
			// USER END
			break;
			// USER START (Optionally insert additional code for further notification handling)
			// USER END
		}
      break;
	  case GUI_ID_BUTTON1: // Notifications sent by 'cancel'
		switch(NCode) 
		{
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)	
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
			pContacts[0]='\0';
			pContent[0]='\0';
			MULTIEDIT_SetText(hContent,0);
			WM_HideWindow(hContent);
			WM_HideWindow(hCancel);
			WM_HideWindow(hSend);
			WM_HideWindow(hText);
			WM_HideWindow(hContacts);
			WM_ShowWindow(hMessagelist);
			WM_EnableWindow(hMessagelist);
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
*       CreateMessage
*/
void FUN_ICON5Clicked(void)
{
	WM_HWIN hWin;
	uint8_t timecount=0;
	uint8_t newmessadd=0,IsRead=0;
	char cmd[20]={0};
	char namenum[20*4]={0},gbkstr[256]={0};
	
		uint8_t check_time =0;

	
	
//	com_data2null(comdata,COMDATA_SIZE);
//	pContent=(char *)&comdata[1024];
	
		//从emwin内存中分配USB空间
	hMem_usb = GUI_ALLOC_AllocZero(1024);
	if(hMem_usb == NULL)
		printf("\r\n no enough space\r\n");
	//把空间句柄转成指针
	pContent = (char *)GUI_ALLOC_h2p(hMem_usb);

	
	APP_TRACE_DBG(("Messageapp create\n"));
	com_createdir("1:/System");
	hWin=GUI_CreateDialogBox(_aDialogCreateMessage, GUI_COUNTOF(_aDialogCreateMessage), _cbDialogMessage, WM_HBKWIN, 0, 0);
	
		/* 初始化并检测模块 */
	while(gsm_init()!= GSM_TRUE)
	{  
		//如果窗口已经关闭，返回
		if(!UserApp_Running)return;
  
		if(check_time++ > 5)
		{			
			ErrorDialog(hWin,UTF8_GSMERROR,UTF8_GSMERROR1);

			while(1)
			{
				if(tpad_flag && UserApp_Running )WM_DeleteWindow(hWin);

				 GUI_Delay(10);
			}
	 }
	}

	GSM_CLEAN_RX();//清除接收缓存
	timecount=0;
	while(UserApp_Running)
	{
		if(timecount>100)
		{
			newmessadd=IsReceiveMS();
			if(newmessadd)
			{	
				BEEP_ON();
				IsRead=readmessage(newmessadd,namenum,(char *)&pContent[512]);				 
				if(IsRead)
				{
					//hexuni2gbk(namenum,namegbk);	
					hexuni2gbk((char *)&pContent[512],gbkstr);
//					printf("new:\nFrom:%s\nContent:%s\n\r",namenum,gbkstr);
					saveReceiveMessage(namenum,gbkstr);					
				}
				sprintf(cmd,"AT+CMGD=%d\r",newmessadd);
				GSM_TX(cmd);
			}
			GSM_CLEAN_RX();//清除接收缓存
			BEEP_OFF();
			timecount=0;
		}
		timecount++;
		if(tpad_flag)WM_DeleteWindow(hWin);
		GUI_Delay(10); 
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
