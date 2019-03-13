#include "resource.h"
#include "SystemInfo.h"

extern HANDLE		ghSystemInfoMutex; 
extern CSystemInfo	*g_pSystemInfo;
extern HINSTANCE	ghInstance;
extern HWND			ghMDIClientArea;
extern HMENU		ghSysInfoMenu;
extern HMENU		ghMainFrameMenu;	

CSystemInfo::CSystemInfo()
{
	m_hwndSystemInformation = NULL;
}

CSystemInfo::~CSystemInfo()
{
	m_hwndSystemInformation = NULL;
}

bool CSystemInfo::InitializeViewAndData()
{
	//Initialize the data first
	if(!m_SystemInfoData.Initialize())
	{
		return false;
	}

	//Initialize the view
	m_SystemInfoView.Initialize(m_hwndSystemInformation);
	m_SystemInfoView.Draw();
	m_SystemInfoView.FillData(m_SystemInfoData);
	return true;
}



LRESULT CALLBACK SigmaSystemInfoWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
     case WM_MDIACTIVATE:
		 {
			 HWND hwndClient = GetParent(hWnd);
			 HWND hwndFrame  = GetParent(hwndClient);

			 HMENU hSysInfoWindowMenu = GetSubMenu(ghSysInfoMenu, SIGMA_SYSINFO_WINDOW_MENU_POS) ;
			 
			 // Set the system info menu when getting activated
			 if (lParam == (LPARAM) hWnd)
			 {
				 SendMessage(hwndClient, 
							WM_MDISETMENU,
							(WPARAM) ghSysInfoMenu, 
							(LPARAM) hSysInfoWindowMenu);

				 //Gray out the system information menu item
				 EnableMenuItem(ghSysInfoMenu, ID_INFORMATION_SYSTEMINFORMATION, MF_GRAYED);
			 }
			 
			 // Set the framewindow menu if losing focus
			 if (lParam != (LPARAM) hWnd)
			 {
			 	 SendMessage(hwndClient, 
							WM_MDISETMENU, 
							(WPARAM) ghMainFrameMenu,
			 				(LPARAM) NULL) ;
			 }
			 
			 // call DrawMenuBar after the menu items are set
			 DrawMenuBar(hwndFrame);
			 
			 return 0 ;
		 }
	case WM_SIZE:
		// Redraw controls
		RECT rect;
		GetClientRect(hWnd, &rect);

		MoveWindow(g_pSystemInfo->m_SystemInfoView.m_hwndTreeView, //Handle to the window. 
					0, // [in] Specifies the new position of the left side of the window. 
					0, // [in] Specifies the new position of the top of the window. 
					rect.right,	// Specifies the new width of the window.
					rect.bottom,// Specifies the new height of the window. 
					true); // Boolean that specifies whether the window is to be repainted. 

		break; //Do not return since this should be processed by DefMDIChildProc

	case WM_CLOSE:
		if(NULL != ghSystemInfoMutex)
		{
			CloseHandle(ghSystemInfoMutex);
			ghSystemInfoMutex = NULL;
		}
		delete g_pSystemInfo;
		break; // Go on to call DefMDIChildProc

	case WM_DESTROY:
		return 0;
	}

	return DefMDIChildProc(hWnd, message, wParam, lParam); //Frame window calls DefFrameProc rather than DefWindowProc
}


HWND CSystemInfo::CreateSystemInfoWindow()
{
	//Register SystemInformation Window
	WNDCLASSEX wndclassMainFrame;
	wndclassMainFrame.cbSize		= sizeof(WNDCLASSEX);
	wndclassMainFrame.style			= CS_HREDRAW | CS_VREDRAW;
	wndclassMainFrame.lpfnWndProc	= SigmaSystemInfoWndProc;
	wndclassMainFrame.cbClsExtra	= 0;
	wndclassMainFrame.cbWndExtra	= 0;
	wndclassMainFrame.hInstance		= ghInstance;
	wndclassMainFrame.hIcon			= LoadIcon(ghInstance, MAKEINTRESOURCE(IDI_SYSTEM_INFO));
	wndclassMainFrame.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclassMainFrame.hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH);
	wndclassMainFrame.lpszMenuName	= NULL;
	wndclassMainFrame.lpszClassName	= TEXT("SigmaSystemInfoWnd");
	wndclassMainFrame.hIconSm		= NULL;
	
	if(!RegisterClassEx(&wndclassMainFrame))
	{
		// return if RegisterClassEx fails
		DWORD dw_LastError = GetLastError();
		if(ERROR_CLASS_ALREADY_EXISTS != dw_LastError)
		{
			// return if the error is other than "error class already exists" 
			return 0;
		}
	}

	// Create Window
	if(NULL != m_hwndSystemInformation)
	{
		return 0;
	}

	MDICREATESTRUCT    MDIChildCreateStruct;
	MDIChildCreateStruct.szClass = TEXT("SigmaSystemInfoWnd");
	MDIChildCreateStruct.szTitle = TEXT("System Information");
	MDIChildCreateStruct.hOwner	 = ghInstance;
	MDIChildCreateStruct.x		 = CW_USEDEFAULT;
	MDIChildCreateStruct.y		 = CW_USEDEFAULT;
	MDIChildCreateStruct.cx		 = CW_USEDEFAULT;
	MDIChildCreateStruct.cy		 = CW_USEDEFAULT;
	MDIChildCreateStruct.style	 = 0;
	MDIChildCreateStruct.lParam	 = 0;
	
	m_hwndSystemInformation = (HWND) SendMessage(ghMDIClientArea,
												WM_MDICREATE, 
												0,
												(LPARAM) (LPMDICREATESTRUCT) &MDIChildCreateStruct) ;
	
	// return if its not possible to create the child window
	if(NULL == m_hwndSystemInformation)
	{
		return 0;
	}

	return m_hwndSystemInformation;
}

