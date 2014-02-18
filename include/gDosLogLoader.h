#pragma once
#ifndef	GDOSLOGLOADER_H_INCLUDED
#define GDOSLOGLOADER_H_INCLUDED

#include <windows.h>

#include <string>
using namespace std;

// ��־�������ʽ����ѡ
typedef enum LogOutType
{
	LOG_OUT_TYPE_CONSOLE	= 0x000001,		/* ���������̨ */
	LOG_OUT_TYPE_FILE		= 0x000002,		/* ������ļ� */
	LOG_OUT_TYPE_WINDOW		= 0x000004,		/* ��������� */
	LOG_OUT_TYPE_TRACE		= 0x000008,		/* ��������ԡ������ */
	LOG_OUT_TYPE_NTI		= 0x000016		/* ͨ��NTI���͵��������������ͳһ���� */
} ELogOutType;

// ��־������
typedef enum GDosLogType
{
	//////////////////////////////////////////////////////////////////////////
	// ������Ա��
	GDOS_LOG_TYPE_DEBUG		= 0x000001,		/* ���� */
	GDOS_LOG_TYPE_RELEASE	= 0x000002,		/* ���� */
	//////////////////////////////////////////////////////////////////////////
	// ������û�
	GDOS_LOG_TYPE_INFO		= 0x000004,		/* ��ͨ��Ϣ */
	GDOS_LOG_TYPE_WARNING	= 0x000008,		/* ���� */
	GDOS_LOG_TYPE_ERROR		= 0x000016		/* ���� */
	//////////////////////////////////////////////////////////////////////////
} EGDosLogType;

// ��־�ĵ��ó��򣬳�ʼ��ʱָ��
typedef enum GDLogLogger
{
	GDOS_LOGGER_OTHER		= 1000,		/* ����δ֪��ʹ���� */
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
	 * @brief 	��ʼ����־��
	 * @param	unsigned int unLogOutType����־���ͣ�ELogOutType����ѡ����'|'�ָ���
	 * @param	EGDosLogLogger logger��д��־�Ľ��̡�
	 * @param	int nID��д��־�Ľ���ID��
	 * @param	string sSvrIP��������־�Ļ㱨IP��������������־����ֵ����""��
	 * @param	int nSvrPort��������־�Ļ㱨�˿ڣ�������������־����ֵ����-1��
	 * @param	unsigned int unLogType ��־���ͣ�����(GDOS_LOG_TYPE_DEBUG | GDOS_LOG_TYPE_RELEASE | GDOS_LOG_TYPE_INFO | GDOS_LOG_TYPE_WARNING | GDOS_LOG_TYPE_ERROR) 
										Ĭ��0xFFFFFFFF��ȫ����
	 * @return  bool����ʼ���ɹ�true��ʧ��false��
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static bool InitLog(unsigned int unLogOutType, EGDosLogLogger logger = GDOS_LOGGER_OTHER, 
				int nID = -1, string sSvrIP = "", int nSvrPort = -1, unsigned int unLogType = 0xFFFFFFFF);
	
	/** 
	 * @brief 	�ر���־
	 * @return  void
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static void ExitLog();

	/** 
	 * @brief 	дһ����־��¼
	 * @param	const char * fmt����־���ݣ�����pringf()��
	 * @param	...
	 * @return  bool
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static bool WriteString(const char *fmt,...);

	static bool WriteTypedString(EGDosLogType type, const char *fmt,...);

private:
	/* ����gDosLog.dllģ�� */
	static bool Load();
	/* ж��gDosLog.dllģ�� */
	static void Free();

	static HMODULE					m_hModule;
	static FNLOG_INITLOG			m_fnInitLog;
	static FNLOG_EXITLOG			m_fnExitLog;
	static FNLOG_WRITESTRING		m_fnWriteString;
	static FNLOG_WRITETYPEDSTRING	m_fnWriteTypedString;
};

#endif