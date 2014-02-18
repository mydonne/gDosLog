#include "StdAfx.h"
#include "LogFile.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

CLogFile::CLogFile(unsigned int unLogOutType, int nLogger, int nLoggerID, string sSvrIP, int nSvrPort)
	: m_pFile(0)
{
	m_unLogOutType = unLogOutType;
	m_nLogger	= nLogger;
	m_nLoggerID = nLoggerID;

	m_sNTISvrIP = sSvrIP;
	m_nNTISvrPort = nSvrPort;
}

CLogFile::~CLogFile(void)
{
	
}

bool CLogFile::InitFile()
{
	if ((m_unLogOutType & 0x000002) == 0)
		return true;

	CloseFile();

	string strAppPath = GetLogPath();
	if (strAppPath == "")
		return false;

	m_pFile = fopen(strAppPath.c_str(), "a");
	if (m_pFile < 0)
		return false;

	return true;
}

void CLogFile::CloseFile()
{
	if (m_pFile > 0)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
}

// char buffer[80];
// int vspf(const char *fmt, ...)
// {
// 	va_list argptr;
// 	int cnt;
// 	va_start(argptr, fmt);
// 	cnt = vsprintf(buffer, fmt, argptr);
// 	va_end(argptr);
// 	return cnt;
// }

void CLogFile::WriteString(string strLog)
{
	// Write to console
	if ((m_unLogOutType & 0x001) > 0)
	printf((strLog + '\n').c_str());

	// Write to file
	if ((m_unLogOutType & 0x000002) > 0 && m_pFile > 0)
	{
		fprintf(m_pFile, (strLog + '\n').c_str());
		fflush(m_pFile);
	}

	// Write to TRACE
	if ((m_unLogOutType & 0x000008) > 0) {
		OutputDebugString((strLog + '\n').c_str());
	}

	// Write to NTI
	if ((m_unLogOutType & 0x000016) > 0) {

	}
}

void CLogFile::gDos_Log_itoa(int val, char *str, int basenum)
{
	int tmp;
	char *b , *p, temp;
	p = str;
	if (val < 0)
	{
		*p = '-';
		p++;
		val = 0 - val;
	}
	b = p;
	if (val == 0) {
		*p = 48;
		p++;
	}
	while (val > 0)
	{
		tmp = val % basenum;
		val /= basenum;
		if (tmp < 10)
		{
			*p = tmp + 48; 
		}
		else
		{
			*p = tmp + 55;
		}
		p++;
	}

	*p = '\0';
	p--;
	while (b < p)   //由于机器支持的是小端，需要转换一下，如果是大端不要后面的while循环！
	{
		temp = *p;
		*p = *b;
		*b = temp;
		b++;
	}
}

string CLogFile::GetLogPath()
{
	char   PathBuffer[512];
	GetModuleFileName(NULL, PathBuffer, 512);
	string strLogFilePath(PathBuffer);
	int pos = strLogFilePath.rfind('\\');
	string strLogFolder = strLogFilePath.substr(0, pos + 1) + "log\\";
	CreateDirectory(strLogFolder.c_str(), NULL);

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char strTimeBuff[128];
	sprintf_s(strTimeBuff, "%04d-%02d-%02d-%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour);

	strLogFilePath = strLogFolder + "gDosLog" + strTimeBuff + ".log";

	return   strLogFilePath;
}

string CLogFile::GetDateTimeString()
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char strTimeBuff[128];

	sprintf(strTimeBuff, "[%04d-%02d-%02d %02d:%02d:%02d] ", 
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	string res(strTimeBuff);

	return res;
}

string CLogFile::GetLogTypeString( unsigned int nType )
{
	if (nType == 0x000001) {		// debug 
		return "[debug] ";
	} else if (nType == 0x000002) {		// release
		return "[release] ";
	} else if (nType == 0x000004) {		// info
		return "[info] ";
	} else if (nType == 0x000008) {		// warning
		return "[warning] ";
	} else if (nType == 0x000016) {		// error
		return "[error] ";
	}

	return "[info] ";
}

string CLogFile::GetLoggerString()
{
	string strLogger = "";
	if (m_nLogger > 1000) {		
		if (m_nLogger == 1001) {		// NTIClient
			strLogger = "NTIClient";
		} else if (m_nLogger == 1002) {	// gDosTools
			strLogger = "gDosTools";
		} else if (m_nLogger == 1003) { // Admin server
			strLogger = "AdminSvr";
		} else if (m_nLogger == 1004) { // Visual server
			strLogger = "VisualSvr";
		} else if (m_nLogger == 1005) { // Visual builder
			strLogger = "VisualBdr";
		}
	}

	string strLoggerID = "";
	if (m_nLoggerID >= 0) {
		char sbuf[256];
		sprintf_s(sbuf, "%d", m_nLoggerID);
		strLoggerID = sbuf;
	}

	string res = "";
	if (strLogger != "") {
		res = "[" + strLogger;
	}
	if (strLoggerID != "") {
		res = res + ":" + strLoggerID;
	}
	if (res != "") {
		res = res + "]";
	}

	return res;
}