/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ucos_emWin_FatFs移植_外部编码表
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */


/*
*********************************************************************************************************
*                                             包含文件
*********************************************************************************************************
*/

#include <includes.h>


/*
*********************************************************************************************************
*                                               宏定义
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             任务控制块TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;
static  OS_TCB   AppTaskKeyScanTCB;
static  OS_TCB		BSPTaskTouchPadScanTCB;
static  OS_TCB   AppTaskDesktopTCB;


/*
*********************************************************************************************************
*                                              栈空间STACKS
*********************************************************************************************************
*/
__align(8) static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
__align(8) static  CPU_STK  BSPTaskTouchPadScanStk[BSP_TASK_TOUCHPAD_SCAN_STK_SIZE];
__align(8) static  CPU_STK  AppTaskKeyScanStk[APP_TASK_KEY_SCAN_STK_SIZE];
__align(8) static  CPU_STK  AppTaskDesktopStk[APP_TASK_DESKTOP_STK_SIZE];


/*
*********************************************************************************************************
*                                             函数声明
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);
static  void  BSPTaskCreate (void);
static  void  AppTaskCreate(void);

extern void TOUCH_Calibrate(void);



/*
*********************************************************************************************************
*                                            
*********************************************************************************************************
*/


/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int  main (void)
{
    OS_ERR  os_err;

    OSInit(&os_err);                                               		/* 初始化 uC/OS-III.                                      */

		//创建启动任务
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                // 任务TCB                               
                 (CPU_CHAR   *)"App Task Start", 								// 任务名称                             
                 (OS_TASK_PTR ) AppTaskStart,									  // 任务函数指针                                
                 (void       *) 0,																	// 可选输入数据
                 (OS_PRIO     ) APP_TASK_START_PRIO,							// 优先级
                 (CPU_STK    *)&AppTaskStartStk[0],							// 任务栈基地址
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,		// 栈“水印”限制
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,        //栈大小
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//可选配置
                 (OS_ERR     *)&os_err);															//错误代码

		 //检查错误
		if(os_err!=OS_ERR_NONE)  bsp_result|=BSP_OS;

		//开始执行任务，从此处开始由ucos系统调度
    OSStart(&os_err);  

	  //检查错误，正常的程序已由ucos调度，不应运行到此处
		if(os_err!=OS_ERR_NONE)  bsp_result|=BSP_OS;
		
		BSP_TRACE_INFO(("OS err:0x%x",bsp_result));    
		
}


/**
  * @brief  启动任务函数，主要完成ucos、BSP、GUI以及其它任务的初始化
  * @param  p_arg: OSTaskCreate创建时传入的数据指针
  * @retval 无
  */
static  void  AppTaskStart (void *p_arg)
{
    OS_ERR      os_err;

   (void)p_arg;
  	CPU_Init();	//初始化CPU

		BSP_Init();  //初始化BSP  	
		

		BSP_Tick_Init();//初始化systick

    Mem_Init(); 	//初始化存储管理器                                     

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&os_err); //计算无任务时CPU使用率                            
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif


	APP_TRACE_DBG(("\r\n正在创建驱动、应用任务..."));

	//创建驱动任务
	BSPTaskCreate();
	//创建应用任务
	AppTaskCreate();  

	//检查硬件错误
	Hardware_Error_Check();
	
 //任务死循环
	while (DEF_TRUE) 
	{                                        
//			LED2_TOGGLE ;
			APP_TRACE_DBG(("cpu使用率:%.2f%%\n",((float)OSStatTaskCPUUsage/100)));

  		//延时，所有任务函数的死循环内都应有至少1ms延时
		  //特别是高优先级的任务，若无延时，其它低优先级任务可能会无机会执行
			OSTimeDly(1000u, 
									OS_OPT_TIME_DLY,
									&os_err);
	}	
		
}

/**
  * @brief  按键任务函数
  * @param  无
  * @retval 无
  */
void  AppTaskKeyScan  (void )
{

	OS_ERR  os_err;
	
		  /* 创建事件标志组 flag_grp */
  OSFlagCreate ((OS_FLAG_GRP  *)&key_flag_grp,        //指向事件标志组的指针
                (CPU_CHAR     *)"KEY Flag",  //事件标志组的名字
                (OS_FLAGS      )0,                //事件标志组的初始值
                (OS_ERR       *)&os_err);					  //返回错误类型


	while(1)
	{
			GUI_TOUCH_Exec();
		
			if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
			{
						OSFlagPost ((OS_FLAG_GRP  *)&key_flag_grp,                             //将标志组的BIT0清0
													(OS_FLAGS      )EVEN_KEY1_DOWN,
													(OS_OPT        )OS_OPT_POST_FLAG_SET,
													(OS_ERR       *)&os_err);
			}
			else
			{
			
			}
			
			if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
			{
						OSFlagPost ((OS_FLAG_GRP  *)&key_flag_grp,                             //将标志组的BIT0清0
													(OS_FLAGS      )EVEN_KEY2_DOWN,
													(OS_OPT        )OS_OPT_POST_FLAG_SET,
													(OS_ERR       *)&os_err);
			}
			else
			{
			
			}
			
			//延时，所有任务函数的死循环内都应有至少1ms延时
		  //特别是高优先级的任务，若无延时，其它低优先级任务可能会无机会执行
			OSTimeDly(10u, 
									OS_OPT_TIME_DLY,
									&os_err);

			
	}
}
	
	
/**
  * @brief  创建驱动任务，如按键，定时扫描等
  * @param  无
  * @retval 无
  */
static  void  BSPTaskCreate (void)
{
	OS_ERR  os_err;

		//创建扫描任务
	OSTaskCreate((OS_TCB     *)&AppTaskKeyScanTCB,             // 任务TCB                               
							 (CPU_CHAR   *)"Key Scan", 									// 任务名称                             
							 (OS_TASK_PTR ) AppTaskKeyScan,									  // 任务函数指针                                
							 (void       *) 0,																	// 可选输入数据
							 (OS_PRIO     ) APP_TASK_KEY_SCAN_PRIO,					// 优先级
							 (CPU_STK    *)&AppTaskKeyScanStk[0],							// 任务栈基地址
							 (CPU_STK_SIZE) APP_TASK_KEY_SCAN_STK_SIZE / 10,				// 栈“水印”限制
							 (CPU_STK_SIZE) APP_TASK_KEY_SCAN_STK_SIZE,        		//栈大小
							 (OS_MSG_QTY  ) 0u,
							 (OS_TICK     ) 0u,
							 (void       *) 0,
							 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//可选配置
							 (OS_ERR     *)&os_err);															//错误代码
							 
							 
	//创建电容触摸按键扫描任务
	OSTaskCreate((OS_TCB     *)&BSPTaskTouchPadScanTCB,             // 任务TCB                               
							 (CPU_CHAR   *)"Touch Pad", 									// 任务名称                             
							 (OS_TASK_PTR ) BSPTaskTouchPadScan,									  // 任务函数指针                                
							 (void       *) 0,																	// 可选输入数据
							 (OS_PRIO     ) BSP_TASK_TOUCHPAD_SCAN_PRIO,					// 优先级
							 (CPU_STK    *)&BSPTaskTouchPadScanStk[0],							// 任务栈基地址
							 (CPU_STK_SIZE) BSP_TASK_TOUCHPAD_SCAN_STK_SIZE / 10,				// 栈“水印”限制
							 (CPU_STK_SIZE) BSP_TASK_TOUCHPAD_SCAN_STK_SIZE,        		//栈大小
							 (OS_MSG_QTY  ) 0u,
							 (OS_TICK     ) 0u,
							 (void       *) 0,
							 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//可选配置
							 (OS_ERR     *)&os_err);			


}

/**
  * @brief  创建应用任务的函数，它会被启动任务调用以创建应用
  * @param  无
  * @retval 无
  */
static  void  AppTaskCreate (void)
{
	OS_ERR  os_err;

	//判断是否需要校准
	if (IsCal == CALADD_FLAG)
	{
			//创建应用任务,桌面任务
		OSTaskCreate((OS_TCB     *)&AppTaskDesktopTCB,             // 任务TCB                               
								 (CPU_CHAR   *)"GUI Task", 									// 任务名称                             
								 (OS_TASK_PTR ) AppTaskDesktop,									  // 任务函数指针                                
								 (void       *) 0,																	// 可选输入数据
								 (OS_PRIO     ) APP_TASK_DESKTOP_PRIO,					// 优先级
								 (CPU_STK    *)&AppTaskDesktopStk[0],							// 任务栈基地址
								 (CPU_STK_SIZE) APP_TASK_DESKTOP_STK_SIZE / 10,				// 栈“水印”限制
								 (CPU_STK_SIZE) APP_TASK_DESKTOP_STK_SIZE,        		//栈大小
								 (OS_MSG_QTY  ) 0u,
								 (OS_TICK     ) 0u,
								 (void       *) 0,
								 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//可选配置
								 (OS_ERR     *)&os_err);															//错误代码
	}
	else
	{
		//创建校准任务
		OSTaskCreate((OS_TCB     *)&AppTaskDesktopTCB,             // 任务TCB                               
								 (CPU_CHAR   *)"GUI Task", 									// 任务名称                             
								 (OS_TASK_PTR ) TOUCH_Calibrate,									  // 任务函数指针                                
								 (void       *) 0,																	// 可选输入数据
								 (OS_PRIO     ) APP_TASK_DESKTOP_PRIO,					// 优先级
								 (CPU_STK    *)&AppTaskDesktopStk[0],							// 任务栈基地址
								 (CPU_STK_SIZE) APP_TASK_DESKTOP_STK_SIZE / 10,				// 栈“水印”限制
								 (CPU_STK_SIZE) APP_TASK_DESKTOP_STK_SIZE,        		//栈大小
								 (OS_MSG_QTY  ) 0u,
								 (OS_TICK     ) 0u,
								 (void       *) 0,
								 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),//可选配置
								 (OS_ERR     *)&os_err);															//错误代码

	}

}

/*********************************************END OF FILE**********************/

