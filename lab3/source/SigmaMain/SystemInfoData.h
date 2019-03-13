#if !defined(_SYSTEM_INFO_DATA_H_INCLUDED_)
#define _SYSTEM_INFO_DATA_H_INCLUDED_

#include "..\includes\SigmaDefines.h"
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

class CSystemInfoData
{
	TCHAR m_szComputerName[MEDIUM_CHAR_ARRAY_LEN];
	TCHAR m_szProcessorCount[MEDIUM_CHAR_ARRAY_LEN];
	TCHAR m_szVersionInfo[MEDIUM_CHAR_ARRAY_LEN];
public:

	CSystemInfoData();
	bool Initialize();
	bool Get_ComputerName(TCHAR* lpszComputerName);
	bool Get_ProcessorCount(TCHAR* lpszProcessorCount);
	bool Get_OSVersionInfo(TCHAR* lpszOSVersion);
};

#endif