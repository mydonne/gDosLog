/***********************************************************************
* Project:	gDosLog
* Purpose:	提供各类型的日志接口功能
* Author:	胡晓东	huxd@irsa.ac.cn		2012-03-01
************************************************************************
* Copyright(c) All rigts reserved.
* 版权所有 (c)  保留所有权利
************************************************************************/
#pragma once
#ifndef	CLOGFILET_H_INCLUDED
#define CLOGFILET_H_INCLUDED

#include <string>
using namespace std;

/***********************************************************************
* 类：CLogFile
* 功能: 该类提供基本的文件读写功能，作为各日志功能的基类
************************************************************************/
class CLogFile
{
public:
	CLogFile(unsigned int unLogOutType, int nLogger, int nLoggerID, string sSvrIP = "", int nSvrPort = -1);
	~CLogFile(void);

public:
	/** 
	 * @brief 	初始化打开日志文件
	 * @return  bool
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	bool InitFile();

	/** 
	 * @brief 	写日志，格式类似于printf函数
	 * @param	string strLog
	 * @return  void
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	void WriteString(string strLog);

	/** 
	 * @brief 	关闭日志文件
	 * @return  void
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	void CloseFile();

	/** 
	 * @brief 	生成当前写日志的进程信息的字符串。
	 * @return  std::string
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	string GetLoggerString();

	/** 
	 * @brief 	获取当前时间，生成日志格式的字符串
	 * @return  std::string
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static string GetDateTimeString();

	/** 
	 * @brief 	返回“日志类型”的字符串
	 * @param	int nType
	 * @return  std::string
	 * 
	 * @author	Hu Xiaodong, huxd@irsa.ac.cn, 2013/04/25
	 */
	static string GetLogTypeString(unsigned int nType);

private:
	/** 
	 * @brief 	获得所执行模块的路径
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
