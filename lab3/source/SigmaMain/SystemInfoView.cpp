#include "SystemInfoView.h"
#include "..\includes\SigmaDefines.h"

CSystemInfoView::CSystemInfoView()
{

}

bool CSystemInfoView::Initialize(HWND hParentWnd)
{
	m_hwndParent = hParentWnd;
	return true;
}

bool CSystemInfoView::Draw()
{
	RECT rect;
	GetClientRect(m_hwndParent, &rect);

	m_hwndTreeView = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_TREEVIEW,
		"", //caption not required
		TVS_TRACKSELECT|WM_NOTIFY|WS_CHILD|TVS_HASLINES|TVS_LINESATROOT|WS_VISIBLE|TVS_HASBUTTONS,
		0,  
		0, 
		rect.right,	// width
		rect.bottom,// height
		m_hwndParent,
		NULL,
		(HINSTANCE) GetWindowLong (m_hwndParent, GWL_HINSTANCE),
		NULL);

	return true;
}

bool CSystemInfoView::FillData(CSystemInfoData &pSystemInfoData)
{
	TCHAR szTempCompName[MEDIUM_CHAR_ARRAY_LEN]			= {0};
	TCHAR szTempProcessorCount[MEDIUM_CHAR_ARRAY_LEN]	= {0};
	TCHAR szTempOSVersion[MEDIUM_CHAR_ARRAY_LEN]		= {0};

    HTREEITEM Parent; // Tree item handle
    HTREEITEM Root; 
	
	TV_INSERTSTRUCT tvinsert; // struct to config the tree control
	tvinsert.hParent = NULL; // top most level Item
	tvinsert.hInsertAfter = TVI_ROOT; // root level item attribute.                            
	tvinsert.item.mask = TVIF_TEXT |TVIF_IMAGE|TVIF_SELECTEDIMAGE; // attributes
	
	
	tvinsert.item.pszText = "System Information"; // item text 
	Root = (HTREEITEM)SendMessage(m_hwndTreeView, TVM_INSERTITEM,0,(LPARAM)&tvinsert);
	
	tvinsert.hInsertAfter = Root;
	tvinsert.hParent = Root;
	tvinsert.item.mask = TVIF_TEXT; 
	// Fill Computer Name info from CSystemInfoData component	
	tvinsert.item.pszText = "Computer Name"; 
	Parent = (HTREEITEM)SendMessage(m_hwndTreeView, TVM_INSERTITEM,0,(LPARAM)&tvinsert);	


	tvinsert.hInsertAfter = Parent;
	tvinsert.hParent = Parent;
	tvinsert.item.mask = TVIF_TEXT;
	pSystemInfoData.Get_ComputerName(szTempCompName);
	tvinsert.item.pszText = szTempCompName; 
	Parent = (HTREEITEM)SendMessage(m_hwndTreeView, TVM_INSERTITEM,0,(LPARAM)&tvinsert);
	
	tvinsert.hInsertAfter = Root;
	tvinsert.hParent = Root;
	tvinsert.item.mask = TVIF_TEXT; 
	//Fill CPU data from CSystemInfoData component
	tvinsert.item.pszText = "CPUs";  
	Parent = (HTREEITEM)SendMessage(m_hwndTreeView, TVM_INSERTITEM,0,(LPARAM)&tvinsert);
	
	tvinsert.hInsertAfter = Parent;
	tvinsert.hParent = Parent;
	tvinsert.item.mask = TVIF_TEXT; 
	pSystemInfoData.Get_ProcessorCount(szTempProcessorCount);
	tvinsert.item.pszText = szTempProcessorCount; 
	Parent = (HTREEITEM)SendMessage(m_hwndTreeView, TVM_INSERTITEM,0,(LPARAM)&tvinsert);

	tvinsert.hInsertAfter = Root;
	tvinsert.hParent = Root;
	//Fill CPU data from CSystemInfoData component
	tvinsert.item.pszText = "OS";  

	Parent = (HTREEITEM)SendMessage(m_hwndTreeView, TVM_INSERTITEM,0,(LPARAM)&tvinsert);
	tvinsert.hInsertAfter = Parent;
	tvinsert.hParent = Parent;
	pSystemInfoData.Get_OSVersionInfo(szTempOSVersion);
	tvinsert.item.pszText = szTempOSVersion;  
	Parent = (HTREEITEM)SendMessage(m_hwndTreeView, TVM_INSERTITEM,0,(LPARAM)&tvinsert);

	////////////////////////////////////////////////////////////////////////
	// Expand all tree items
	////////////////////////////////////////////////////////////////////////
	SendMessage(m_hwndTreeView, TVM_EXPAND, TVE_EXPAND, (LPARAM)Root); // Expand the root element of the tree
	
	HTREEITEM hItemChild = (HTREEITEM) SendMessage(m_hwndTreeView, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)Root); 
	if(NULL != hItemChild)
	{
		SendMessage(m_hwndTreeView, TVM_EXPAND, TVE_EXPAND, (LPARAM)hItemChild); 
	}
	
	hItemChild = (HTREEITEM) SendMessage(m_hwndTreeView, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItemChild); 
	if(NULL != hItemChild)
	{
		SendMessage(m_hwndTreeView, TVM_EXPAND, TVE_EXPAND, (LPARAM)hItemChild); 
	}
	hItemChild = (HTREEITEM) SendMessage(m_hwndTreeView, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItemChild); 
	if(NULL != hItemChild)
	{
		SendMessage(m_hwndTreeView, TVM_EXPAND, TVE_EXPAND, (LPARAM)hItemChild); 
	}	
	return true;
}


