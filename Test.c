/**********************************************************************
* 版权所有 (C)2019, Zhou Zhaoxiong
*
* 文件名称：WriteLog.c
* 文件标识：无
* 内容摘要：演示日志信息的打印方法
* 其它说明：无
* 当前版本：V1.0
* 作   者：Zhou Zhaoxiong
* 完成日期：20190909
*
**********************************************************************/
#include "WriteLog.h"

// 员工信息结构体定义
typedef struct
{
    unsigned char  szEmployeeName[128];    // 员工姓名
    int    iEmployeeAge;           // 员工年龄
} T_EmployeeInfo;

// 函数声明
void GetEmployeeInfo();


/**********************************************************************
* 功能描述：主函数
* 输入参数：无
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期        版本号       修改人            修改内容
* -------------------------------------------------------------------
* 20190909        V1.0     Zhou Zhaoxiong        创建
***********************************************************************/
int main()
{
	unsigned char  szLogContent[1024] = {0};

	 // 获取配置文件中各个配置项的值
	GetConfigValue();
	
	// 先打印版本相关信息
	snprintf(szLogContent, sizeof(szLogContent)-1, "Version [1.0], Build time[%s %s].", __DATE__, __TIME__);
    WRITELOGFILE(LOG_INFO, szLogContent);

	// 打印第一条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Fatal log info!");
    WRITELOGFILE(LOG_FATAL, szLogContent);

    // 打印第二条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Error log info!");
	WRITELOGFILE(LOG_ERROR, szLogContent);
	
	// 打印第三条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Warn log info!");
	WRITELOGFILE(LOG_WARN, szLogContent);

	// 打印第四条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Info log info!");
	WRITELOGFILE(LOG_INFO, szLogContent);
	
	// 打印第五条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Trace log info!");
	WRITELOGFILE(LOG_TRACE, szLogContent);
	
	// 打印第六条日志
	snprintf(szLogContent, sizeof(szLogContent)-1, "The Debug log info!");
	WRITELOGFILE(LOG_DEBUG, szLogContent);
	
	GetEmployeeInfo();   // 获取并打印员工信息
	
    return 0; 
}


/**********************************************************************
 * 功能描述： 获取并打印员工信息
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 其它说明： 无
 * 修改日期        版本号         修改人         修改内容
 * -------------------------------------------------------------------
 * 20190905        V1.0      Zhou Zhaoxiong      创建
 ********************************************************************/
void GetEmployeeInfo()
{
    unsigned char  szLogContent[1024] = {0};
	
    T_EmployeeInfo tEmployeeInfo = {0};
    
	// 获取并打印员工信息
	// 获取员工姓名
	GetConfigFileStringValue("EMPLOYEEINFO", "EmployeeName", "", tEmployeeInfo.szEmployeeName, sizeof(tEmployeeInfo.szEmployeeName), "Config.ini");
	
	// 获取员工年龄
	tEmployeeInfo.iEmployeeAge = GetConfigFileIntValue("EMPLOYEEINFO", "EmployeeAge", 0, "Config.ini");
	if (tEmployeeInfo.iEmployeeAge == -1)  // 判断获取到的年龄是否正确
	{
	    snprintf(szLogContent, sizeof(szLogContent)-1, "GetEmployeeInfo: Get EmployeeAge failed!");
	    WRITELOGFILE(LOG_ERROR, szLogContent);
		return;
	}
	
	// 打印读取到的员工姓名和年龄
	snprintf(szLogContent, sizeof(szLogContent)-1, "EmployeeName is %s, EmployeeAge is %d", tEmployeeInfo.szEmployeeName, tEmployeeInfo.iEmployeeAge);
	WRITELOGFILE(LOG_INFO, szLogContent);
}
