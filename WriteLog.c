/**********************************************************************
* 版权所有 (C)2019, Zhou Zhaoxiong
*
* 文件名称：WriteLog.c
* 文件标识：无
* 内容摘要：日志系统源代码文件
* 其它说明：无
* 当前版本：V1.0
* 作   者：Zhou Zhaoxiong
* 完成日期：20190905
*
**********************************************************************/
#include "WriteLog.h"

// 全局变量
unsigned int  g_iLogLevel      = 0;    // 日志等级
unsigned int  g_iLogPosition   = 0;    // 日志位置
unsigned char g_szLogFile[100] = {0};  // 带路径的日志文件名

// 函数声明
void WriteLogFile(unsigned char *pszFileName, unsigned char *pszFunctionName, unsigned int iCodeLine, unsigned int iLogLevel, unsigned char *pszContent);
unsigned char *LogLevel(unsigned int iLogLevel);
void GetTime(unsigned char *pszTimeStr);
void GetConfigValue();
void GetStringContentValue(FILE *fp, unsigned char *pszSectionName, unsigned char *pszKeyName, unsigned char *pszOutput, unsigned int iOutputLen);
void GetConfigFileStringValue(unsigned char *pszSectionName, unsigned char *pszKeyName, unsigned char *pDefaultVal, unsigned char *pszOutput, unsigned int iOutputLen, unsigned char *pszConfigFileName);
int GetConfigFileIntValue(unsigned char *pszSectionName, unsigned char *pszKeyName, unsigned int iDefaultVal, unsigned char *pszConfigFileName);


// 以下为函数具体实现
/**********************************************************************
 * 功能描述： 将内容写到日志文件中
 * 输入参数： pszFileName-代码文件名
              pszFunctionName-代码所在函数名
              iCodeLine-代码行
              iLogLevel-日志等级
              pszContent-每条日志的具体内容
 * 输出参数： 无
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期        版本号         修改人          修改内容
 * -------------------------------------------------------------------
 * 20190905        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/    
void WriteLogFile(unsigned char *pszFileName, unsigned char *pszFunctionName, unsigned int iCodeLine, unsigned int iLogLevel, unsigned char *pszContent) 
{
    FILE  *fp                 = NULL;
	unsigned char  szLogContent[2048] = {0};
	unsigned char  szTimeStr[128]     = {0};

	// 先对输入参数进行异常判断
    if (pszFileName == NULL || pszContent == NULL)
    {
        return;
    }

	// 过滤日志等级
	if (iLogLevel > g_iLogLevel)
	{
        return;
	}

    fp = fopen(g_szLogFile, "at+");      // 打开文件, 每次写入的时候在后面追加
    if (fp == NULL)
    {
        return;
    }

	// 写入日志时间
    GetTime(szTimeStr);
    fputs(szTimeStr, fp);

	// 写入日志内容
	if (g_iLogPosition == 1)    // 在日志信息中显示"文件名/函数名/代码行数"信息
	{
	    snprintf(szLogContent, sizeof(szLogContent)-1, "[%s][%s][%04d][%s]%s\n", pszFileName, pszFunctionName, iCodeLine, LogLevel(iLogLevel), pszContent);
    }
	else    // 不用在日志信息中显示"文件名/代码行数"信息
	{
	    snprintf(szLogContent, sizeof(szLogContent)-1, "[%s]%s\n", LogLevel(iLogLevel), pszContent);
	}
    fputs(szLogContent, fp);
    
    fflush(fp);     // 刷新文件
    fclose(fp);     // 关闭文件
    fp = NULL;      // 将文件指针置为空

    return;
}


/**********************************************************************
 * 功能描述： 获取对应的日志等级
 * 输入参数： iLogLevel-日志等级
 * 输出参数： 无
 * 返 回 值： 日志等级信息字符串
 * 其它说明： 无
 * 修改日期        版本号         修改人          修改内容
 * -------------------------------------------------------------------
 * 20190905        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/ 
unsigned char *LogLevel(unsigned int iLogLevel)
{
    switch (iLogLevel)
    {
        case LOG_FATAL:
		{
			return "FATAL";
		}

        case LOG_ERROR:   
		{
			return "ERROR";
		}

        case LOG_WARN :
		{
			return "WARN";
		}

        case LOG_INFO :
		{
			return "INFO";
		}

        case LOG_TRACE:   
		{
			return "TRACE";
		}

        case LOG_DEBUG:   
		{
			return "DEBUG";
		}

        default: 
		{
			return "OTHER";
		}
    }
}


/**********************************************************************
 * 功能描述： 获取时间串
 * 输入参数： pszTimeStr-时间串
 * 输出参数： pszTimeStr-时间串
 * 返 回 值： 无
 * 其它说明： 时间串样式: YYYY.MM.DD HH:MIN:SS.Usec
 * 修改日期        版本号         修改人         修改内容
 * -------------------------------------------------------------------
 * 20190905        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/ 
void GetTime(unsigned char *pszTimeStr)
{
    struct tm      tSysTime     = {0};
    struct timeval tTimeVal     = {0};
    time_t         tCurrentTime = {0};
	
    unsigned char  szUsec[20] = {0};    // 微秒
	unsigned char  szMsec[20] = {0};    // 毫秒
	
	// 先对输入参数进行异常判断
	if (pszTimeStr == NULL)
	{
	    return;
	}

    tCurrentTime = time(NULL);
    localtime_r(&tCurrentTime, &tSysTime); 

    gettimeofday(&tTimeVal, NULL);    
    sprintf(szUsec, "%06d", tTimeVal.tv_usec);  // 获取微秒
    strncpy(szMsec, szUsec, 3);                 // 微秒的前3位为毫秒(1毫秒=1000微秒)
    
    sprintf(pszTimeStr, "[%04d-%02d-%02d %02d:%02d:%02d.%3.3s]", 
            tSysTime.tm_year+1900, tSysTime.tm_mon+1, tSysTime.tm_mday,
            tSysTime.tm_hour, tSysTime.tm_min, tSysTime.tm_sec, szMsec);
}


/**********************************************************************
* 功能描述： 获取配置文件完整路径(包含文件名)
* 输入参数： pszConfigFileName-配置文件名
             pszWholePath-配置文件完整路径(包含文件名)
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 无
* 修改日期       版本号        修改人        修改内容
* ------------------------------------------------------------------
* 20190905       V1.0     Zhou Zhaoxiong     创建
********************************************************************/  
void GetCompletePath(unsigned char *pszConfigFileName, unsigned char *pszWholePath)
{
    unsigned char *pszHomePath      = NULL;
	unsigned char  szWholePath[256] = {0};
	
	// 先对输入参数进行异常判断
	if (pszConfigFileName == NULL || pszWholePath == NULL)
	{
		return;
	}

    pszHomePath = (unsigned char *)getenv("HOME");     // 获取当前用户所在的路径
    if (pszHomePath == NULL)
    {
        return;
    }

    // 拼装配置文件路径
    snprintf(szWholePath, sizeof(szWholePath)-1, "%s/test/WriteLog/%s", pszHomePath, pszConfigFileName);
	strncpy(pszWholePath, szWholePath, strlen(szWholePath));
}


/**********************************************************************
 * 功能描述： 获取日志配置项的值
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期        版本号         修改人         修改内容
 * -------------------------------------------------------------------
 * 20190905        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/ 
void GetConfigValue()
{
	unsigned char  szLogDir[256] = {0};

	// 日志等级
	g_iLogLevel = GetConfigFileIntValue("LOG", "LogLevel", 5, "Config.ini");
	
	// 日志位置
	g_iLogPosition = GetConfigFileIntValue("LOG", "LogPosition", 0, "Config.ini");

	// 日志文件存放目录
	GetConfigFileStringValue("LOG", "LogDir", "", szLogDir, sizeof(szLogDir), "Config.ini");
    snprintf(g_szLogFile, sizeof(g_szLogFile)-1, "%s/WriteLog.log", szLogDir);
}


/**********************************************************************
* 功能描述： 获取具体的字符串值
* 输入参数： fp-配置文件指针
             pszSectionName-段名, 如: GENERAL
             pszKeyName-配置项名, 如: EmployeeName
			 iOutputLen-输出缓存长度
* 输出参数： pszOutput-输出缓存
* 返 回 值： 无
* 其它说明： 无
* 修改日期       版本号        修改人         修改内容
* ------------------------------------------------------------------
* 20190905       V1.0     Zhou Zhaoxiong     创建
********************************************************************/
void GetStringContentValue(FILE *fp, unsigned char *pszSectionName, unsigned char *pszKeyName, unsigned char *pszOutput, unsigned int iOutputLen)
{
    unsigned char  szSectionName[100]    = {0};
	unsigned char  szKeyName[100]        = {0};
	unsigned char  szContentLine[256]    = {0};
	unsigned char  szContentLineBak[256] = {0};
	unsigned int iContentLineLen       = 0;
	unsigned int iPositionFlag         = 0;
	
    // 先对输入参数进行异常判断
	if (fp == NULL || pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL)
	{
	    printf("GetStringContentValue: input parameter(s) is NULL!\n");
		return;
	}
	
	sprintf(szSectionName, "[%s]", pszSectionName);
	strcpy(szKeyName, pszKeyName);
	
	while (feof(fp) == 0)
	{
	    memset(szContentLine, 0x00, sizeof(szContentLine));
        fgets(szContentLine, sizeof(szContentLine), fp);      // 获取段名
		
		// 判断是否是注释行(以;开头的行就是注释行)或以其他特殊字符开头的行
		if (szContentLine[0] == ';' || szContentLine[0] == '\r' || szContentLine[0] == '\n' || szContentLine[0] == '\0')
		{
			continue;
	    }
		
		// 匹配段名
		if (strncasecmp(szSectionName, szContentLine, strlen(szSectionName)) == 0)     
		{
		    while (feof(fp) == 0)
			{
			    memset(szContentLine,    0x00, sizeof(szContentLine));
				memset(szContentLineBak, 0x00, sizeof(szContentLineBak));
                fgets(szContentLine, sizeof(szContentLine), fp);     // 获取字段值
		
		        // 判断是否是注释行(以;开头的行就是注释行)
		        if (szContentLine[0] == ';')
		        {
			        continue;
	            }
				
				memcpy(szContentLineBak, szContentLine, strlen(szContentLine));
				
				// 匹配配置项名
				if (strncasecmp(szKeyName, szContentLineBak, strlen(szKeyName)) == 0)     
		        {
				    iContentLineLen = strlen(szContentLine);
					for (iPositionFlag = strlen(szKeyName); iPositionFlag <= iContentLineLen; iPositionFlag ++)
					{
					    if (szContentLine[iPositionFlag] == ' ')
						{
						    continue;
						}
						if (szContentLine[iPositionFlag] == '=')
						{
						    break;
						}
						
						iPositionFlag = iContentLineLen + 1;
						break;
					}
					
					iPositionFlag = iPositionFlag + 1;    // 跳过=的位置
					
					if (iPositionFlag > iContentLineLen)
					{
					    continue;
					}
					
					memset(szContentLine, 0x00, sizeof(szContentLine));
					strcpy(szContentLine, szContentLineBak + iPositionFlag);
					
					// 去掉内容中的无关字符
					for (iPositionFlag = 0; iPositionFlag < strlen(szContentLine); iPositionFlag ++)
					{
					    if (szContentLine[iPositionFlag] == '\r' || szContentLine[iPositionFlag] == '\n' || szContentLine[iPositionFlag] == '\0')
						{
						    szContentLine[iPositionFlag] = '\0';
							break;
						}
					}
					
					// 将配置项内容拷贝到输出缓存中
					strncpy(pszOutput, szContentLine, iOutputLen-1);
					break;
				}
				else if (szContentLine[0] == '[')
				{
					break;
				}
			}
			break;
		}
	}
}


/**********************************************************************
* 功能描述： 从配置文件中获取字符串
* 输入参数： pszSectionName-段名, 如: GENERAL
             pszKeyName-配置项名, 如: EmployeeName
			 pDefaultVal-默认值
			 iOutputLen-输出缓存长度
             pszConfigFileName-配置文件名
* 输出参数： pszOutput-输出缓存
* 返 回 值： 无
* 其它说明： 无
* 修改日期       版本号        修改人         修改内容
* ------------------------------------------------------------------
* 20190905       V1.0     Zhou Zhaoxiong     创建
********************************************************************/  
void GetConfigFileStringValue(unsigned char *pszSectionName, unsigned char *pszKeyName, unsigned char *pDefaultVal, unsigned char *pszOutput, unsigned int iOutputLen, unsigned char *pszConfigFileName)
{
    FILE  *fp                    = NULL;
	unsigned char  szWholePath[256]      = {0};
	
	// 先对输入参数进行异常判断
	if (pszSectionName == NULL || pszKeyName == NULL || pszOutput == NULL || pszConfigFileName == NULL)
	{
	    printf("GetConfigFileStringValue: input parameter(s) is NULL!\n");
		return;
	}
	
	// 获取默认值
	if (pDefaultVal == NULL)
	{
		strcpy(pszOutput, "");
    }
	else
	{
		strcpy(pszOutput, pDefaultVal);
	}
	
	// 打开配置文件
	GetCompletePath(pszConfigFileName, szWholePath);
	fp = fopen(szWholePath, "r");
	if (fp == NULL)
	{
	    printf("GetConfigFileStringValue: open %s failed!\n", szWholePath);
		return;
    }
	
	// 调用函数用于获取具体配置项的值
	GetStringContentValue(fp, pszSectionName, pszKeyName, pszOutput, iOutputLen);
	
	// 关闭文件
	fclose(fp);
	fp = NULL;
}


/**********************************************************************
* 功能描述： 从配置文件中获取整型变量
* 输入参数： pszSectionName-段名, 如: GENERAL
             pszKeyName-配置项名, 如: EmployeeName
			 iDefaultVal-默认值
             pszConfigFileName-配置文件名
* 输出参数： 无
* 返 回 值： iGetValue-获取到的整数值, -1-获取失败
* 其它说明： 无
* 修改日期       版本号       修改人         修改内容
* ------------------------------------------------------------------
* 20190905       V1.0     Zhou Zhaoxiong     创建
********************************************************************/  
int GetConfigFileIntValue(unsigned char *pszSectionName, unsigned char *pszKeyName, unsigned int iDefaultVal, unsigned char *pszConfigFileName)
{
    unsigned char  szGetValue[512] = {0};
	int  iGetValue       = 0;
	
	// 先对输入参数进行异常判断
	if (pszSectionName == NULL || pszKeyName == NULL || pszConfigFileName == NULL)
	{
	    printf("GetConfigFileIntValue: input parameter(s) is NULL!\n");
		return -1;
	}

	GetConfigFileStringValue(pszSectionName, pszKeyName, NULL, szGetValue, sizeof(szGetValue)-1, pszConfigFileName);    // 先将获取的值存放在字符型缓存中
	
	if (szGetValue[0] == '\0' || szGetValue[0] == ';')    // 如果是结束符或分号, 则使用默认值
	{
		iGetValue = iDefaultVal;
    }
	else
	{
		iGetValue = atoi(szGetValue);
	}

	return iGetValue;
}
