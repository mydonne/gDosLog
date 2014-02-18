#pragma once
#ifndef	GDOSLOGLOADER_H_INCLUDED
#define GDOSLOGLOADER_H_INCLUDED

#include <windows.h>

#include <string>
using namespace std;

// 日志的输出形式，多选
typedef enum LogOutType
{
	LOG_OUT_TYPE_CONSOLE	= 0x000001,		/* 输出到控制台 */
	LOG_OUT_TYPE_FILE		= 0x000002,		/* 输出到文件 */
	LOG_OUT_TYPE_WINDOW		= 0x000004,		/* 输出到窗体 */
	LOG_OUT_TYPE_TRACE		= 0x000008,		/* 输出到调试“输出” */
	LOG_OUT_TYPE_NTI		= 0x000016		/* 通过NTI发送到管理服务器进行统一管理。 */
} ELogOutType;

// 日志的类型
typedef enum GDosLogType
{
	//////////////////////////////////////////////////////////////////////////
	// 开发人员用
	GDOS_LOG_TYPE_DEBUG		= 0x000001,		/* 调试 */
	GDOS_LOG_TYPE_RELEASE	= 0x000002,		/* 发布 */
	//////////////////////////////////////////////////////////////////////////
	// 输出给用户
	GDOS_LOG_TYPE_INFO		= 0x000004,		/* 普通消息 */
	GDOS_LOG_TYPE_WARNING	= 0x000008,		/* 警告 */
	GDOS_LOG_TYPE_ERROR		= 0x000016		/* 错误 */
	//////////////////////////////////////////////////////////////////////////
} EGDosLogType;

// 日志的调用程序，初始化时指定
typedef enum GDLogLogger
{
	GDOS_LOGGER_OTHER		= 1000,		/* 其他未知的使用者 */
	GDOS_LOGGER_NTICLIENT	= 1001,		/* NTIClient */
	GDOS_LOGGER_GDOSTOOLS	= 1002,		/* gDosTools */
	GDOS_LOGGER_ADMINSVR	= 1003,		/* Admin server */
	GDOS_LOGGER_VS			= 1004,		/* Visual server */
	GDOS_LOGGER_VB			= 1005		/* Visual builder */
} EGDosLogLogger;

typedef bool (__stdcall *FNLOG_INITLOG)(unsigned int unLogOutType, int nLogger, int nLoggerID, string sSvrIP, int nSvrPort, unsigned int unLogType);
typedef void (__stdcall *FNLOG_EXITLOG)();
typedef bool (__stdcall *FNLOG_WRITESTRING)(const char *fmt);
typedef bool (__stdcall *FNLOG_WRITETYPEDSTRING)(int nLogType, const char *fmt);

class CGDosLogLoader
{
public:
	/** 
	 * @brief 	初始化日志。
	 * @param	unsigned int unLogOutType，日志类型，ELogOutType，多选，用'|'分隔；
	 * @param	EGDosLogLogger logger，写日志的进程。
	 * @param	int nID，写日志的进程ID。
	 * @param	string sSvrIP，网络日志的汇报IP，若无需网络日志，此值可填""。
	 * @param	int nSvrPort，网络日志的汇报端口，若无需网络日志，此值可填-1。
	 * @param	unsigned int unLogType 日志类型，形如(GDOS_LOG_TYPE_DEBUG | GDOS_LOG_TYPE_RELEASE | GDOS_LOG_TYPE_INFO | GDOS_LOG_TYPE_WARNING | GDOS_LOG_TYPE_ERROR) 
										默认0xFFFFFFFF（全部）
	 * @return  bool，初始化成功true；失败false。
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static bool InitLog(unsigned int unLogOutType, EGDosLogLogger logger = GDOS_LOGGER_OTHER, 
				int nID = -1, string sSvrIP = "", int nSvrPort = -1, unsigned int unLogType = 0xFFFFFFFF);
	
	/** 
	 * @brief 	关闭日志
	 * @return  void
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static void ExitLog();

	/** 
	 * @brief 	写一条日志记录
	 * @param	const char * fmt，日志内容，类似pringf()。
	 * @param	...
	 * @return  bool
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static bool WriteString(const char *fmt,...);

	static bool WriteTypedString(EGDosLogType type, const char *fmt,...);

private:
	/* 载入gDosLog.dll模块 */
	static bool Load();
	/* 卸载gDosLog.dll模块 */
	static void Free();

	static HMODULE					m_hModule;
	static FNLOG_INITLOG			m_fnInitLog;
	static FNLOG_EXITLOG			m_fnExitLog;
	static FNLOG_WRITESTRING		m_fnWriteString;
	static FNLOG_WRITETYPEDSTRING	m_fnWriteTypedString;
};

#endif