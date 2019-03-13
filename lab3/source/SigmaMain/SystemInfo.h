#if !defined(_SYSTEM_INFO_H_INCLUDED_)
#define _SYSTEM_INFO_H_INCLUDED_

#include <windows.h>
#include "SystemInfoData.h"
#include "SystemInfoView.h"

class CSystemInfo
{
	HWND m_hwndSystemInformation; //System Information MDI Child window handle

public:		
	CSystemInfo();	
	~CSystemInfo();	
	CSystemInfoView	m_SystemInfoView;
	CSystemInfoData m_SystemInfoData;

	HWND CreateSystemInfoWindow();

	bool InitializeViewAndData();
};

#endif