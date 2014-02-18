/***********************************************************************
* Project:	gDosLog
* Purpose:	�ṩ�����͵���־�ӿڹ���
* Author:	������	huxd@irsa.ac.cn		2012-03-01
************************************************************************
* Copyright(c) All rigts reserved.
* ��Ȩ���� (c)  ��������Ȩ��
************************************************************************/
#pragma once
#ifndef	CLOGFILET_H_INCLUDED
#define CLOGFILET_H_INCLUDED

#include <string>
using namespace std;

/***********************************************************************
* �ࣺCLogFile
* ����: �����ṩ�������ļ���д���ܣ���Ϊ����־���ܵĻ���
************************************************************************/
class CLogFile
{
public:
	CLogFile(unsigned int unLogOutType, int nLogger, int nLoggerID, string sSvrIP = "", int nSvrPort = -1);
	~CLogFile(void);

public:
	/** 
	 * @brief 	��ʼ������־�ļ�
	 * @return  bool
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	bool InitFile();

	/** 
	 * @brief 	д��־����ʽ������printf����
	 * @param	string strLog
	 * @return  void
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	void WriteString(string strLog);

	/** 
	 * @brief 	�ر���־�ļ�
	 * @return  void
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	void CloseFile();

	/** 
	 * @brief 	���ɵ�ǰд��־�Ľ�����Ϣ���ַ�����
	 * @return  std::string
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	string GetLoggerString();

	/** 
	 * @brief 	��ȡ��ǰʱ�䣬������־��ʽ���ַ���
	 * @return  std::string
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static string GetDateTimeString();

	/** 
	 * @brief 	���ء���־���͡����ַ���
	 * @param	int nType
	 * @return  std::string
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static string GetLogTypeString(unsigned int nType);

private:
	/** 
	 * @brief 	�����ִ��ģ���·��
	 * @return  std::string
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	string GetLogPath();

	void gDos_Log_itoa(int val, char *str, int basenum);
	
private:
	FILE* m_pFile;
	string m_slogPath;	
	string m_logSubDir;
	int m_nDayOfMonth;
	int m_iLineNum;
	
	unsigned int m_unLogOutType;
	int			 m_nLogger;
	int			 m_nLoggerID;

	string		 m_sNTISvrIP;
	int			 m_nNTISvrPort;
};
#endif
