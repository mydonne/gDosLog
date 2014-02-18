// gDosLog.cpp : ���� DLL Ӧ�ó���ĵ���������
#include "stdafx.h"

#include "LogFile.h"

static bool g_bGDosLogInit = false;
static CRITICAL_SECTION g_csModule;

// �ⲿ�ɹ�����InitLog�Ĵ��������ڵ���ExitLogʱȷ���Ƿ����ٱ�ģ��ȫ�ֱ���
static int g_nLoadCount = 0;

// ��־��
static CLogFile* g_pLogFile = 0;

static unsigned int g_unLogType = 0xFFFFFFFF;

// ��ʼ����־
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

// �ر���־
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

// дһ����־��¼
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

// дһ����־��¼
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