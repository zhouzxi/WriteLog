/**********************************************************************
* 版权所有 (C)2019, Zhou Zhaoxiong
*
* 文件名称：WriteLog.h
* 文件标识：无
* 内容摘要：日志系统头文件
* 其它说明：无
* 当前版本：V1.0
* 作   者：Zhou Zhaoxiong
* 完成日期：20190905
*
**********************************************************************/
#ifndef _WRITE_LOG_H_
#define _WRITE_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

// 日志级别定义
#define LOG_FATAL       0     // 严重错误
#define LOG_ERROR       1     // 一般错误
#define LOG_WARN        2     // 警告
#define LOG_INFO        3     // 一般信息
#define LOG_TRACE       4     // 跟踪信息
#define LOG_DEBUG       5     // 调试信息

// 函数宏定义
#define WRITELOGFILE(level, msg)  WriteLogFile(__FILE__, __FUNCTION__, __LINE__, level, msg)

#endif
