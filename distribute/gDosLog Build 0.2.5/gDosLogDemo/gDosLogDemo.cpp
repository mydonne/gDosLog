// gDosLogDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "..\Include\gDosLogLoader.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//if (CGDosLogLoader::InitLog(LOG_OUT_TYPE_CONSOLE | LOG_OUT_TYPE_FILE | LOG_OUT_TYPE_TRACE, GDOS_LOGGER_VS, 1, "", -1, GDOS_LOG_TYPE_DEBUG | GDOS_LOG_TYPE_INFO) == true)
	if (CGDosLogLoader::InitLog(LOG_OUT_TYPE_FILE) == true)
	{
		printf("InitLog ok. \n");

		CGDosLogLoader::WriteTypedString(GDOS_LOG_TYPE_INFO, "info log%d00000", 100);
		CGDosLogLoader::WriteTypedString(GDOS_LOG_TYPE_DEBUG, "debug log%d00000", 100);
		CGDosLogLoader::WriteTypedString(GDOS_LOG_TYPE_RELEASE, "release log%d00000", 100);
		CGDosLogLoader::WriteTypedString(GDOS_LOG_TYPE_WARNING, "warning log%d00000", 100);
		CGDosLogLoader::WriteTypedString(GDOS_LOG_TYPE_ERROR, "error log%d00000", 100);
		//CGDosLogLoader::WriteTypedString(GDOS_LOG_TYPE_INFO, "log");

		CGDosLogLoader::ExitLog();
		printf("ExitLog ok. \n");
	}

	system("pause");

	return 0;
}

