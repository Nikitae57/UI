#include "SystemInfoData.h"
#include "..\includes\SigmaDefines.h"

CSystemInfoData::CSystemInfoData()
{
	memset((void*)m_szProcessorCount, 0, sizeof(m_szProcessorCount));
	memset((void*)m_szVersionInfo, 0, sizeof(m_szVersionInfo));
	memset((void*)m_szComputerName, 0, sizeof(m_szComputerName));
}

bool CSystemInfoData::Initialize()
{
	//Get computer name
	DWORD dw_Len = MEDIUM_CHAR_ARRAY_LEN;
	TCHAR szCompName[MEDIUM_CHAR_ARRAY_LEN] = {0};
	BOOL b = GetComputerName((char*)szCompName, &dw_Len);
	strcpy((char*)m_szComputerName, (char*)szCompName);
	
	//Get info about CPUs
	LPCTSTR pszOldVal; 
	pszOldVal = (LPTSTR) malloc(MEDIUM_CHAR_ARRAY_LEN*sizeof(TCHAR));
	
	DWORD dwRet = GetEnvironmentVariable(TEXT("NUMBER_OF_PROCESSORS"), (char*)pszOldVal, MEDIUM_CHAR_ARRAY_LEN);
	_tcscpy((unsigned short*)m_szProcessorCount, (const unsigned short *)pszOldVal);
	free((void*)pszOldVal);
	
	//Get info about OS version, flavour and Service Pack
	OSVERSIONINFO VersionInfo;
	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	if(GetVersionEx(&VersionInfo))
	{
		VersionInfo.dwMajorVersion;
		VersionInfo.dwMinorVersion;
		VersionInfo.dwBuildNumber;
		VersionInfo.szCSDVersion;
		
		TCHAR szOSName[MEDIUM_CHAR_ARRAY_LEN];
		
		if(6 == VersionInfo.dwMajorVersion)
		{
			strcpy((char*)szOSName, "Vista");
		}
		else if(0 == VersionInfo.dwMinorVersion)
		{
			strcpy((char*)szOSName, "Windows 2000");
		}
		else if(1 == VersionInfo.dwMinorVersion)
		{
			strcpy((char*)szOSName, "Windows XP");
		}
		else
		{
			strcpy((char*)szOSName, "Unknown OS");
		}
		
		sprintf((char*)m_szVersionInfo, "%s, %s", szOSName, VersionInfo.szCSDVersion); 
	}
	
	return true;
}

bool CSystemInfoData::Get_ComputerName(TCHAR* lpszComputerName)
{
	strcpy((char*)lpszComputerName, (char*)m_szComputerName);
	return true;
}

bool CSystemInfoData::Get_ProcessorCount(TCHAR* lpszProcessorCount)
{
	strcpy((char*)lpszProcessorCount, (char*)m_szProcessorCount);
	return true;
}

bool CSystemInfoData::Get_OSVersionInfo(TCHAR* lpszOSVersion)
{
	strcpy((char*)lpszOSVersion, (char*)m_szVersionInfo);
	return true;
}
