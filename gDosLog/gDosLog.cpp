// gDosLog.cpp : 定义 DLL 应用程序的导出函数。
#include "stdafx.h"

#include "LogFile.h"

static bool g_bGDosLogInit = false;
static CRITICAL_SECTION g_csModule;

// 外部成功调用InitLog的次数，用于调用ExitLog时确定是否销毁本模块全局变量
static int g_nLoadCount = 0;

// 日志类
static CLogFile* g_pLogFile = 0;

static unsigned int g_unLogType = 0xFFFFFFFF;

// 初始化日志
extern "C" __declspec (dllexport) bool __stdcall InitLog(unsigned int unLogOutType, int logger, int nID, string sSvrIP, int nSvrPort, unsigned int unLogType)
{
	g_bGDosLogInit = true;

	if (g_nLoadCount <= 0)
		InitializeCriticalSection(&g_csModule);

	bool bRet = true;
	EnterCriticalSection(&g_csModule);
	g_unLogType = unLogType;
	if (g_pLogFile == 0)
		g_pLogFile = new CLogFile(unLogOutType, logger, nID);

	if (!g_pLogFile)
		bRet = false;
	else
	{
		if (g_nLoadCount <= 0 && g_pLogFile->InitFile() == false)
			bRet = false;
	}
	if (bRet)
		g_nLoadCount++;
	LeaveCriticalSection(&g_csModule);

	return bRet;

	//return true;
}

// 关闭日志
extern "C" __declspec (dllexport) void __stdcall ExitLog()
{
	EnterCriticalSection(&g_csModule);
	g_nLoadCount--;
	
	if (g_nLoadCount <= 0)
	{
		g_pLogFile->CloseFile();
		delete g_pLogFile;
		g_pLogFile = 0;
	}
	LeaveCriticalSection(&g_csModule);

	//if (g_nLoadCount <= 0)
	//	DeleteCriticalSection(&g_csModule);
}

// 写一条日志记录
extern "C" __declspec (dllexport) bool __stdcall WriteTypedString(int nLogType, const char *fmt)
{
	bool bRet = true;

	EnterCriticalSection(&g_csModule);
	if ((nLogType & g_unLogType) != 0)
	{
		if (g_pLogFile == 0) {
			bRet = false;
		} else {
			string strLog(fmt);
			strLog = g_pLogFile->GetLoggerString() + CLogFile::GetDateTimeString() + CLogFile::GetLogTypeString(nLogType) + strLog;

			g_pLogFile->WriteString(strLog);
		}
	}
	LeaveCriticalSection(&g_csModule);

	return bRet;
}

// 写一条日志记录
extern "C" __declspec (dllexport) bool __stdcall WriteString(const char *fmt)
{
	if (g_nLoadCount > 0)
	{
		bool bRet = true;

		EnterCriticalSection(&g_csModule);
		if (g_pLogFile == 0) {
			bRet = false;
		} else {
 			string strLog(fmt);
			strLog = CLogFile::GetDateTimeString() + strLog;

			g_pLogFile->WriteString(strLog);
		}
		LeaveCriticalSection(&g_csModule);

		return bRet;
	}

	return false;
}