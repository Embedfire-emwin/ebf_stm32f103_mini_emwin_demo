#ifndef __COMMON_H_
#define __COMMON_H_

#include "DIALOG.h"
#include  <stdint.h>
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
/**************************************************************************************
*
*       Defines
*
***************************************************************************************
*/
//#define COMDATA_SIZE			0x1000	//4096B

//extern uint8_t comdata[COMDATA_SIZE];
extern GUI_HMEM hMem_usb;

/*--------------------  (使用U2C.exe小工具生成)  ------------------------------------*/

#define UTF8_GSMERROR        "GSM\xe6\xa8\xa1\xe5\x9d\x97\xe9\x80\x9a\xe4\xbf\xa1\xe5\xa4\xb1\xe8\xb4\xa5"//GSM模块通信失败
#define UTF8_GSMERROR1       "\xe8\xaf\xb7\xe6\xa3\x80\xe6\x9f\xa5\xe6\x8e\xa5\xe7\xba\xbf"//请检查接线      


/*********************************************************************
*
*       tpeydef
*
**********************************************************************
*/
/* WAV文件头格式 */
typedef __packed struct
{ 
	uint32_t	riff;							/* = "RIFF"	0x46464952*///4
	uint32_t	size_8;						/* 从下个地址开始到文件尾的总字节数	*///4
	uint32_t	wave;							/* = "WAVE" 0x45564157*///4
	
	uint32_t	fmt;							/* = "fmt " 0x20746d66*///4
	uint32_t	fmtSize;					/* 下一个结构体的大小(一般为16) *///4	 
	uint16_t	wFormatTag;				/* 编码方式,一般为1	*///2
	uint16_t	wChannels;				/* 通道数，单声道为1，立体声为2 *///2
	uint32_t	dwSamplesPerSec;	/* 采样率 *///4
	uint32_t	dwAvgBytesPerSec;	/* 每秒字节数(= 采样率 × 每个采样点字节数) *///4
	uint16_t	wBlockAlign;			/* 每个采样点字节数(=量化比特数/8*通道数) *///2
	uint16_t	wBitsPerSample;		/* 量化比特数(每个采样需要的bit数) *///2
																			   
	uint32_t	data;							/* = "data" 0x61746164*///4
	uint32_t	datasize;					/* 纯数据长度 *///4
} WavHead;


/*********************************************************************
*
*       data
*
**********************************************************************
*/
//extern uint8_t comdata[COMDATA_SIZE];


/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void com_data2null(uint8_t *data,uint32_t length);

void com_gbk2utf8(const char *src, char *str);
void com_utf82gbk(const char *src, char *str);
void com_createdir(const char *dir);

void ErrorDialog(WM_HWIN hParent,char *string1,char *string2);
#endif //end of __COMMON_H_
