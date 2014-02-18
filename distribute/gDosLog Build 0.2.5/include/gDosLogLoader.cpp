#include "stdafx.h"
#include "GDosLogLoader.h"

HMODULE CGDosLogLoader::m_hModule = 0;
FNLOG_INITLOG CGDosLogLoader::m_fnInitLog = 0;
FNLOG_EXITLOG CGDosLogLoader::m_fnExitLog = 0;
FNLOG_WRITESTRING CGDosLogLoader::m_fnWriteString = 0;
FNLOG_WRITETYPEDSTRING CGDosLogLoader::m_fnWriteTypedString = 0;

bool CGDosLogLoader::Load()
{
	if (m_hModule != 0)
		return true;

	m_hModule = LoadLibrary("gDosLog.dll");
	if (m_hModule == 0)
		return false;

	m_fnInitLog = (FNLOG_INITLOG)GetProcAddress(m_hModule, "InitLog");
	if (!m_fnInitLog)
	{
		Free();
		return false;
	}

	m_fnExitLog = (FNLOG_EXITLOG)GetProcAddress(m_hModule, "ExitLog");
	if (!m_fnExitLog)
	{
		Free();
		return false;
	}

	m_fnWriteString = (FNLOG_WRITESTRING)GetProcAddress(m_hModule, "WriteString");
	if (!m_fnWriteString)
	{
		Free();
		return false;
	}

	m_fnWriteTypedString = (FNLOG_WRITETYPEDSTRING)GetProcAddress(m_hModule, "WriteTypedString");
	if (!m_fnWriteTypedString) {
		Free();
		return false;
	}

	return true;
}

void CGDosLogLoader::Free()
{
	if (m_hModule)
	{
		FreeLibrary(m_hModule);
		m_hModule = 0;
	}

	m_fnInitLog = 0;
	m_fnExitLog = 0;
	m_fnWriteString = 0;
}

bool CGDosLogLoader::InitLog( unsigned int unLogOutType, EGDosLogLogger logger, int nID, string sSvrIP, int nSvrPort, unsigned int unLogType )
{
	if (m_fnInitLog == 0)
		Load();

	if (m_fnInitLog != 0)
		return m_fnInitLog(unLogOutType, logger, nID, sSvrIP, nSvrPort, unLogType);

	return false;
}

void CGDosLogLoader::ExitLog()
{
	if (m_fnExitLog == 0)
		Load();

	if (m_fnExitLog != 0)
		m_fnExitLog();
}

bool CGDosLogLoader::WriteString( const char *fmt,... )
{
	va_list argptr;
	//int cnt;
	va_start(argptr, fmt);
	int nSize= vprintf(fmt, argptr);
	if (nSize > 0)
	{
		char* buffer=new char[nSize+1];
		memset(buffer,'\0',nSize+1);
		vsprintf(buffer, fmt, argptr);
		va_end(argptr);
		//string strLog(buffer);
		if (m_fnWriteString == 0)
			Load();

		bool ret = false;
		if (m_fnWriteString != 0)
			ret = m_fnWriteString(buffer /*strLog.c_str()*/);

		delete buffer;
		return ret;
	}

// 	// 组合字符串
// 	char buffer[1024] = "\0";
// 	va_list argptr;
// 	int cnt;
// 	va_start(argptr, fmt);
// 	cnt = vsprintf(buffer, fmt, argptr);
// 	va_end(argptr);
// 	string strLog(buffer);
// 
// 	if (m_fnWriteTypedString == 0)
// 		Load();
// 
// 	if (m_fnWriteTypedString != 0)
// 		return m_fnWriteTypedString(GDOS_LOG_TYPE_INFO, strLog.c_str());
// 
// 	return false;
}

bool CGDosLogLoader::WriteTypedString( EGDosLogType type, const char *fmt,... )
{
	va_list argptr;
	//int cnt;
	va_start(argptr, fmt);
	int nSize= vprintf(fmt, argptr);
	char* buffer=new char[nSize+1];
	memset(buffer,'\0',nSize+1);
	vsprintf(buffer, fmt, argptr);
	va_end(argptr);
	//string strLog(buffer);
	if (m_fnWriteTypedString == 0)
		Load();

	bool ret = false;
	if (m_fnWriteTypedString != 0)
		ret = m_fnWriteTypedString(type,buffer /*strLog.c_str()*/);
	
	delete buffer;
	return ret;

	// 组合字符串
// 	char buffer[1024] = "\0";
// 	va_list argptr;
// 	int cnt;
// 	va_start(argptr, fmt);
// 	cnt = vsprintf(buffer, fmt, argptr);
// 	va_end(argptr);
// 	string strLog(buffer);
// 
// 	if (m_fnWriteTypedString == 0)
// 		Load();
// 
// 	if (m_fnWriteTypedString != 0)
// 		return m_fnWriteTypedString(type, strLog.c_str());
// 
// 	return false;
}


