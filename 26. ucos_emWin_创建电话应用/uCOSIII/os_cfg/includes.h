/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                         Evaluation Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT

//使能hardfault时直接软件复位
//#ifndef SOFTRESET
//#define SOFTRESET
//#endif



/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include	<string.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/
#include  <app_cfg.h>
#include  <bsp.h>
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./key/bsp_key.h"  
#include "./lcd/bsp_ili9341_lcd.h"
#include "./dht11/bsp_dht11.h"
#include "./ds18b20/bsp_ds18b20.h"
#include "./adc/bsp_adc.h"
#include "./rtc/bsp_rtc.h"
#include "./rtc/bsp_date.h"
#include "./rtc/bsp_calendar.h"
#include "./gsm_gprs/bsp_gsm_usart.h"
#include "./gsm_gprs/bsp_gsm_gprs.h"

#include "ff.h"

#include "GUIFont_Port.h" 
#include "GUIPicture_Port.h"   

#include "desktop.h"
#include "common.h"

/*
*********************************************************************************************************
*                                               emWin
*********************************************************************************************************
*/
#include "GUI.h"
#include "DIALOG.h"


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>


/*
*********************************************************************************************************
*                                                 ST
*********************************************************************************************************
*/

#include "stm32f10x.h"          // Modified by fire (原是 #include  <stm32f10x_lib.h>) 


/*
*********************************************************************************************************
*                                            declare
*********************************************************************************************************
*/


//SPI FLASH存储触摸校准数据的位置
#define CALADD  				(513*4096)
#define CALADD_FLAG		0x55

extern unsigned char IsCal ;

/************ 键盘输入 **************/
void keypadapp(uint16_t message,uint8_t IsEdit);
extern uint8_t  keypadflag;
extern WM_HWIN  KEYBOARD;
/************************************/

/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/


#endif

