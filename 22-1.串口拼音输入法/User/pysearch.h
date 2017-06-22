/*
*********************************************************************************************************
*	                                  
*	模块名称 : 全拼音输入法
*	文件名称 : MainTask.c
*	版    本 : V1.0
*	说    明 : 这部分汉字表和检索函数来自网络，使用本工程请保证此文件的属性是UTF-8格式
*                                     
*	修改记录 :
*		版本号    日期          作者          说明
*
*
*********************************************************************************************************
*/

//配置使用SPI FLASH里的拼音检索表还是直接在程序里的检索表
#define PY_TABLE  USE_INER_TABLE

//拼音检索表存储在外部FALSH
#define USE_EXTERN_TABLE	0

//拼音检索表直接存储在STM32，
//注意使用这种方式时，要把pysearch.c文件使用notepad++等工具转为utf8格式
#define USE_INER_TABLE		1


typedef struct
{
	const char *PYList;  	//拼音检索字符串
	int			num;		//总汉字数
	const char *HZGB;		//以所定义的拼音组合的汉字GB码的指针
}PinYinDef;

//拼音检索表存储在STM32
#if (PY_TABLE == USE_INER_TABLE)

//拼音组合的汉字字库
//拼音检索的结构体
struct PinYinPCB
{
	char *PYSerList;  		//拼音检索字符
	const unsigned char *PYRankOneList;  	//以所定义的拼音组合的汉字字库的指针
};

#else //拼音检索表存储在外部FLASH


//拼音组合的汉字字库
//拼音检索的结构体
struct PinYinPCB
{
	char *PYSerList;  		//拼音检索字符
	unsigned short     int PYRankOneList;  	//以所定义的拼音组合的汉字字库的指针
};
#endif

char *PYSearch(unsigned char *msg , int *num);

/***************************** END OF FILE *********************************/
