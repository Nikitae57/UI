#if !defined(_SYSTEM_INFO_VIEW_H_INCLUDED_)
#define _SYSTEM_INFO_VIEW_H_INCLUDED_

#include <windows.h>
#include <commctrl.h>
#include "SystemInfoData.h"

class CSystemInfoView
{
	HWND m_hwndParent;
public:
	HWND m_hwndTreeView; //TreeView control window handle
	CSystemInfoView();
	bool Initialize(HWND hParentWnd);
	bool Draw();
	bool FillData(CSystemInfoData &pSystemInfoData);
};

#endif