#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>

#include "SigmaMain.h"
#include "..\includes\SigmaDefines.h"
#include "..\includes\SigmaCommon.h"
#include "SystemInfo.h"
#include "resource.h"

HINSTANCE	ghInstance					= NULL;
TCHAR		gszSigmaFrameClassName[]	= TEXT("SigmaMdiFrame");
TCHAR		gszAppName[]				= TEXT("Sigma");
HWND		ghMDIClientArea				= NULL; //This stores the MDI client area window handle

HWND		ghwndMainFrame				= NULL; // This will hold the main frame window handle
HMENU		ghMainFrameMenu				= NULL;

CSystemInfo *g_pSystemInfo				= NULL;
HANDLE		ghSystemInfoMutex			= NULL;
HMENU		ghSysInfoMenu				= NULL;

//mailslot handle
HANDLE		ghSlot						= NULL;

//sigma service monitor thread handle
HANDLE		ghMonitorServiceThread		= NULL;

HWND		ghwndStatusBar				= NULL;

int			giStatusBarHeight			= 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	ghInstance = hInstance;

	INITCOMMONCONTROLSEX dICC;
	dICC.dwSize = sizeof(INITCOMMONCONTROLSEX);
	dICC.dwICC = ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES;

	// Initialize common control library
	BOOL bInitCommonControlLib = InitCommonControlsEx(&dICC);
	if(!bInitCommonControlLib)
	{
		MessageBox(NULL, TEXT("Unable to load Common Controls!"), gszAppName, MB_ICONERROR);
	}
	
	WNDCLASSEX wndclassMainFrame;
	wndclassMainFrame.cbSize		= sizeof(WNDCLASSEX);
	wndclassMainFrame.style			= CS_HREDRAW | CS_VREDRAW;
	wndclassMainFrame.lpfnWndProc	= SigmaFrameWndProc;
	wndclassMainFrame.cbClsExtra	= 0;
	wndclassMainFrame.cbWndExtra	= 0;
	wndclassMainFrame.hInstance		= hInstance;
	wndclassMainFrame.hIcon			= LoadIcon(ghInstance, MAKEINTRESOURCE(IDI_SIGMA_MAIN_ICON));
	wndclassMainFrame.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndclassMainFrame.hbrBackground	= (HBRUSH) GetStockObject(GRAY_BRUSH);
	wndclassMainFrame.lpszMenuName	= NULL;
	wndclassMainFrame.lpszClassName	= gszSigmaFrameClassName;
	wndclassMainFrame.hIconSm		= NULL;
	
	//Register MainFrame window
	if(!RegisterClassEx(&wndclassMainFrame))
	{
		DWORD derror = GetLastError();
		MessageBox(NULL, TEXT("This program requires Windows NT!"), gszAppName, MB_ICONERROR);
		return 0;
	}

	ghMainFrameMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAINFRAME_MENU));
	ghSysInfoMenu	= LoadMenu(hInstance, MAKEINTRESOURCE(IDR_SYSINFO_MENU));
	DWORD derror	= GetLastError();


	//Create the main MDI frame window
	ghwndMainFrame = CreateWindow(gszSigmaFrameClassName, 
								gszAppName, 
								WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
								CW_USEDEFAULT,	// allows system choose an x position
								CW_USEDEFAULT,	// allows system choose a y position
								CW_USEDEFAULT,	// width, CW_USEDEFAULT allows system to choose height and width
								CW_USEDEFAULT,	// height, CW_USEDEFAULT ignores heights as this is set by setting
												// CW_USEDEFAULT in width above.
								NULL,			// handle to parent window
								ghMainFrameMenu, // handle to menu
								hInstance,	// handle to the instance of module
								NULL);		// Long pointer to a value to be passed to the window through the 
											// CREATESTRUCT structure passed in the lParam parameter the WM_CREATE message

	nShowCmd = SW_SHOW; // To show the window
	
	ShowWindow(ghwndMainFrame,  nShowCmd);
	UpdateWindow(ghwndMainFrame);
	
	//Create Status bar
	CreateStatusBar();

	// Add systrayicon
	AddSysTrayIcon();

	// setup mail slot so that service can tell when it starts up
	SetupMailslot();

	//start service monitor thread
	ghMonitorServiceThread = CreateThread(0,
					0,
					(LPTHREAD_START_ROUTINE) MonitorServiceThread,
					0,
					NULL,
					NULL);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{

			TranslateMessage(&msg) ;
			DispatchMessage(&msg) ;
	}

    //remove the notification from the tray
	RemoveSysTrayIcon();

	//close mailslot handle
	CloseHandle(ghSlot);
	return msg.wParam ;
}


LRESULT CALLBACK SigmaFrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CLIENTCREATESTRUCT MDIClientCreateStruct; // Structure to be used for MDI client area

	switch(message)
	{
	case WM_SIZE:
		// Redraw controls
		if(NULL != ghwndStatusBar)
		{
			// re-arrange the client area so that the status bar is always visible
			if(NULL != ghMDIClientArea)
			{
				RECT rcClient;
				// Get the coordinates of the parent window's client area.
				GetClientRect(ghwndMainFrame, &rcClient);
				MoveWindow(ghMDIClientArea, 
					0, 
					0, 
					rcClient.right, //width
					rcClient.bottom - giStatusBarHeight, //height
					true);
			}
			 // size status bar
			SendMessage(ghwndStatusBar, WM_SIZE, 0, 0);
		}
		return 0; //return from here because if DefFrameProc is called, it would fill MDICLIENT fully
				  //overlapping the status bar
	case WM_CREATE:
		// On creation of main frame, create the MDI client area
		MDIClientCreateStruct.hWindowMenu	= NULL;
		MDIClientCreateStruct.idFirstChild	= IDM_FIRSTCHILD;
		
		ghMDIClientArea = CreateWindow(TEXT("MDICLIENT"), // predefined value for MDI client area
									NULL, // no caption required
									WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
									0, // No need to give any x/y or height/width since this client
									   // will just be used to get client windows created, effectively
									   // in the main window we will be seeing the mainframe window client area itself.
									0, 
									0,
									0,
									hWnd,
									NULL,
									ghInstance,
									(void *) &MDIClientCreateStruct);
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_INFORMATION_SYSTEMINFORMATION:
			if(NULL != ghSystemInfoMutex)
			{
				// return if mutex is already created.
				return 0;
			}			
			ghSystemInfoMutex = CreateMutex(NULL, false, SIGMA_SYSTEMINFO_MUTEX);

			//Create the SystemInformation Data and View
			g_pSystemInfo = new CSystemInfo();
			if(g_pSystemInfo->CreateSystemInfoWindow())
			{
				// If window is created then try to initialize view and show assiciated data
				g_pSystemInfo->InitializeViewAndData();
			}
			else
			{
				// delete and reset to NULL if its not possible to create View and Data
				delete g_pSystemInfo;
				g_pSystemInfo = NULL;
			}
			return 0;

		case ID_HELP_ABOUT:
			MessageBox(ghwndMainFrame, TEXT("Sigma by Sharath C V"), TEXT("Sigma"), MB_OK);
			return 0;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			return 0;
		case ID_WINDOWS_CASCADE:
			SendMessage(ghMDIClientArea, WM_MDICASCADE, 0, 0);
			return 0;
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefFrameProc(hWnd, ghMDIClientArea, message, wParam, lParam); //Frame window calls DefFrameProc rather than DefWindowProc
}

bool CreateStatusBar()
{
	//////////////////////////////////////////////////////////////////
	//Create the status bar
	//////////////////////////////////////////////////////////////////
	ghwndStatusBar = CreateWindowEx(0, // extended not required
							STATUSCLASSNAME, // status bar class name, equivalent to "msctls_statusbar32"
							"", //caption not required
							WS_CHILD | WS_VISIBLE,
							-100, // x 
							-100, // y
							10, // width
							10, // height
							ghwndMainFrame,
							NULL,
							(HINSTANCE) GetWindowLong (ghwndMainFrame, GWL_HINSTANCE),
							NULL);
	
	RECT rcClient;
	LPINT lpParts = NULL;
	//int i = 0;
	int nWidth = 0;
	int nParts = SIGMA_STATUSBAR_PARTS;
	
	// Get the coordinates of the parent window's client area.
	GetClientRect(ghwndMainFrame, &rcClient);
	
	// Allocate an array for holding the right edge coordinates.
	HLOCAL hloc = NULL;
	hloc = LocalAlloc(LHND, sizeof(int) * nParts);
	lpParts = (int *)LocalLock(hloc);
	
	// Calculate the right edge coordinate for each part, and
	// copy the coordinates to the array.
	nWidth = rcClient.right / nParts;
	for (int i = 0; i < nParts; i++) 
	{ 
		lpParts[i] = nWidth;
		nWidth += nWidth;
	}
	
	// Create status bar parts.
	SendMessage(ghwndStatusBar, SB_SETPARTS, (WPARAM)nParts, (LPARAM)lpParts);
	
	// Free the array
	LocalUnlock(hloc);
	LocalFree(hloc);

	// re-arrange the client area so that the status bar is always visible
	RECT rectStatusBar;
	GetClientRect(ghwndStatusBar, &rectStatusBar); // get status bar client area
	giStatusBarHeight = rectStatusBar.bottom;
	MoveWindow(ghMDIClientArea, 
		0, 
		0, 
		rcClient.right, //width
		rcClient.bottom - giStatusBarHeight, //height
		true);

	// Set a message in first part (zero based index) of the status bar
	// Each part can have maximum of 127 characters
	SendMessage(ghwndStatusBar, SB_SETTEXT,
				0, // Part 1 of status based on Zero based index
				(LPARAM)"Application started, no error detected.");  

	// Set a default message regarding Sigma Support Service
	SendMessage(ghwndStatusBar, SB_SETTEXT,
				2, // Part 3 of status based on Zero based index
				(LPARAM)"Service status unknown"); 

	return true;
}

void AddSysTrayIcon()
{
	NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(nid);
    nid.uID = SIGMA_SYSTRAY_ICON_ID;      // 0 to 12 are reserved and should not be used.
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.hIcon = LoadIcon(ghInstance, MAKEINTRESOURCE(IDI_SIGMA_MAIN_ICON));
	strcpy(nid.szTip, "Sigma");
	nid.hWnd = ghwndMainFrame;
	nid.uCallbackMessage = WM_SIGMA_SYSTRAY_MSG;

    //Add the notification to the tray
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveSysTrayIcon()
{

	NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(nid);
    nid.uID = SIGMA_SYSTRAY_ICON_ID;      // 0 to 12 are reserved and should not be used.
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    nid.hIcon = LoadIcon(ghInstance, MAKEINTRESOURCE(IDI_SIGMA_MAIN_ICON));
	strcpy(nid.szTip, "Sigma");
	nid.hWnd = ghwndMainFrame;
	nid.uCallbackMessage = WM_SIGMA_SYSTRAY_MSG;
	
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

bool SetupMailslot()
{
	ghSlot = CreateMailslot(TEXT(MAILSLOTNAME), 
									0, // no maximum message size 
									MAILSLOT_WAIT_FOREVER, // no time-out for operations 
									(LPSECURITY_ATTRIBUTES) NULL); // default security
 
	if (ghSlot == INVALID_HANDLE_VALUE) 
	{ 
		MessageBox(NULL, TEXT("CreateMailslot failed"), TEXT("sigma"), MB_OK);
		return false; 
	} 

	return true;
}


DWORD MonitorServiceThread(LPVOID lpParam)
{
	// open mail slot file
	DWORD dw_MsgSize = 0;
	DWORD dw_MsgCount = 0;
	DWORD dw_MsgRead = 0; 
    
	LPTSTR lpszBuffer; 
    
    BOOL fResult; 
	HANDLE hEvent;
    OVERLAPPED ov;
 
    hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("SigmaMailSlot"));
    //if( NULL == hEvent )
    //    return FALSE;
    
	ov.Offset = 0;
    ov.OffsetHigh = 0;
    ov.hEvent = hEvent;
 
	while(1)
	{
		fResult = GetMailslotInfo(ghSlot, // mailslot handle 
							(LPDWORD) NULL,        // no maximum message size 
							&dw_MsgSize,           // size of next message 
							&dw_MsgCount,          // number of messages 
							(LPDWORD) NULL);       // no read time-out 
		
		if (!fResult) 
		{ 
			MessageBox(NULL, TEXT("GetMailslot failed"), TEXT("sigma"), MB_OK);
		} 
		
		while (dw_MsgCount != 0)  // retrieve all messages
		{ 
			// memory for the message. 
			lpszBuffer = (LPTSTR) GlobalAlloc(GPTR, 
				dw_MsgSize); 
			if( NULL == lpszBuffer )
				return FALSE;
			lpszBuffer[0] = '\0'; 
			
			fResult = ReadFile(ghSlot, 
				lpszBuffer, 
				dw_MsgSize, 
				&dw_MsgRead, 
				&ov); 
			
			if (!fResult) 
			{ 
				MessageBox(NULL, TEXT("ReadFile failed"), TEXT("Sigma"), MB_OK);
				GlobalFree((HGLOBAL) lpszBuffer); 
				break; 
			} 
			
			// Updated third part of status bar
			// this part is reserved for Sigma service status
			SendMessage(ghwndStatusBar, SB_SETTEXT,
						2, // Part 3 of status based on Zero based index
						(LPARAM) TEXT(lpszBuffer));  
			
			GlobalFree((HGLOBAL) lpszBuffer); 
			
			fResult = GetMailslotInfo(ghSlot,	// mailslot handle 
				(LPDWORD) NULL,					// no maximum message size 
				&dw_MsgSize,					// size of next message 
				&dw_MsgCount,                   // number of messages 
				(LPDWORD) NULL);				// no read time-out 
			
			if (!fResult) 
			{ 
				MessageBox(NULL, TEXT("GetMailslotInfo failed"), TEXT("sigma"), MB_OK);
				break; 
			} 
		} 
		Sleep(500);
	}
    return TRUE; 
}